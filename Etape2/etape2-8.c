#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>
#include "etape2-8.h"

uint64_t set_sh_entsize(uint32_t type){
	switch(type){
		case SHT_REL:
			return 8;
		case SHT_SYMTAB:
			return 16;
		default:
			return 0;
	}
}

uint32_t set_sh_link(uint32_t type, uint32_t section1_link, int symtab_index){
	switch(type){
		case SHT_REL:
			return symtab_index;
		case SHT_SYMTAB:
			return section1_link;
		default:
			return 0;
	}
}

int get_symtab_index(FILE* fichier,Elf32_Ehdr header){
	Elf32_Shdr section;
	for(int i=0; i < header.e_shnum; i++) {
		fseek(fichier,  header.e_shoff + i * header.e_shentsize, SEEK_SET);
		litEtInverse_Section(fichier, header, &section);

		if (section.sh_type == SHT_SYMTAB) {
			return i;
		}
	}
	return -1;
}

uint32_t set_sh_info(uint32_t type, uint32_t info, int* renumerotation_section2){
	if (type==SHT_REL){
		return renumerotation_section2[info];
	}
	return 0;
}
void file_copy(FILE* file1, FILE* file2, size_t size){
	char* ptr=malloc(size);
	fread(ptr,size,1,file1);
	fwrite(ptr,size,1,file2);
	free(ptr);
}



int fusion_header(FILE* elfFile1, FILE* elfFile2, FILE* outputFile,Elf32_Ehdr* headerOutput){
	Elf32_Ehdr header1;
	Elf32_Ehdr header2;

	Elf32_Shdr section1;
	Elf32_Shdr section2;
	char* nom_section1=NULL;
	char* nom_section2=NULL;

	litEtInverse_Header(elfFile1,&header1);
	litEtInverse_Header(elfFile2,&header2);

	for (int i=0;i<EI_NIDENT;i++){
		headerOutput->e_ident[i]=header1.e_ident[i];
	}
	headerOutput->e_ident[EI_DATA]=ELFDATA2LSB; //je triche j'écris en little endian quoiqu'il arrive
	headerOutput->e_type=header1.e_type;
	headerOutput->e_machine=header1.e_machine;
	headerOutput->e_version=header1.e_version;     //DE 				IL FAUT VERIFIER QUE LES TYPES SONT COMPATIBleS ETC.
	headerOutput->e_entry=0;												//LA
	headerOutput->e_phoff=0;												//MERDE
	headerOutput->e_shoff=sizeof(Elf32_Ehdr);
	headerOutput->e_flags=header1.e_flags;
	headerOutput->e_phnum=0;
	headerOutput->e_shentsize=sizeof(Elf32_Shdr);
	headerOutput->e_shstrndx=header1.e_shstrndx;
	headerOutput->e_shnum=header1.e_shnum+header2.e_shnum; //On l'initialise à la somme des 2 shnum
	//Calcul du shnum : on le soustrait de 1 pour chaque section en commun
	for (int i=0; i<header1.e_shnum;i++){ //parcours des sections du fichier 1
		fseek(elfFile1, header1.e_shoff + i * header1.e_shentsize, SEEK_SET);
		litEtInverse_Section(elfFile1,header1,&section1);
		nom_section1=get_section_name(elfFile1,header1,section1);
		for (int j=0;j<header2.e_shnum;j++){ //parcours des sections du fichier 2
			fseek(elfFile2, header2.e_shoff + j * header2.e_shentsize, SEEK_SET);
			litEtInverse_Section(elfFile2,header2,&section2);
			nom_section2=get_section_name(elfFile2,header2,section2);
			if(!strcmp(nom_section1,nom_section2)){  //si les sections ont le meme nom
				headerOutput->e_shnum--; //On soustrait le shnum de 1
			}
		}
	}
	//On print le header dans le fichier output
	fwrite(headerOutput,sizeof(Elf32_Ehdr),1,outputFile);
	return 1; //On retourne 1 si la fusion s'est bien passée (POUR L'INSTANT ELLE SE PASSE TJRS BIEN M'EMMERDEZ PAS)
}

int fusion_section_2eme_tentative(FILE* elfFile1, FILE* elfFile2, FILE* outputFile, Elf32_Ehdr* headerOutput){
	Elf32_Ehdr header1;
	Elf32_Ehdr header2;

	Elf32_Shdr section1;
	Elf32_Shdr section2;
	Elf32_Shdr sectionOut;
	int j,k;
	int offset_actuel = sizeof(Elf32_Ehdr)+headerOutput->e_shnum*headerOutput->e_shentsize;
	int curseur;
	int symtab_index;
	size_t taille_shtstrtab;
	char* nom_section1;
	char* nom_section2;
	fseek(elfFile1,0,SEEK_SET);
	fseek(elfFile2,0,SEEK_SET);
	litEtInverse_Header(elfFile1,&header1);
	litEtInverse_Header(elfFile2,&header2);

	int sections_deja_fusionnees[header2.e_shnum];
	for (int i=0;i<header2.e_shnum;i++){
		sections_deja_fusionnees[i]=0;
	}

	int renumerotation_section2[header2.e_shnum];//Si i est l'ancien indice, nouvel indice est égal à renumerotation_section2[i]
	for (int i=0;i<header2.e_shnum;i++){
		renumerotation_section2[i]=-1;
	}

	symtab_index=get_symtab_index(elfFile1,header1);
	for (int i=0; i<header1.e_shnum;i++){ //parcours des sections du fichier 1
		fseek(elfFile1, header1.e_shoff + i * header1.e_shentsize, SEEK_SET);
		litEtInverse_Section(elfFile1,header1,&section1);
		nom_section1=get_section_name(elfFile1,header1,section1);
		j=0;
		if (header1.e_shstrndx==i){
			taille_shtstrtab=section1.sh_size;
		}

		fseek(elfFile2, header2.e_shoff + j * header2.e_shentsize, SEEK_SET);
		litEtInverse_Section(elfFile2,header2,&section2);
		nom_section2=get_section_name(elfFile2,header2,section2);
		while(strcmp(nom_section1,nom_section2)&&j<header2.e_shnum){ //parcours des sectons du fichier 2
			j++;
			fseek(elfFile2, header2.e_shoff + j * header2.e_shentsize, SEEK_SET);
			litEtInverse_Section(elfFile2,header2,&section2);
			nom_section2=get_section_name(elfFile2,header2,section2);
		}
		if (j<header2.e_shnum){ //si une section de meme nom a été trouvée
			if(section1.CONTENU_SEMANTIQUE == SHT_SYMTAB){
				struct Noeud *ArbreVariableGlobal = malloc(sizeof(struct Noeud));
				struct Liste *listeLocal = malloc(sizeof(struct Liste));
				listeLocal->premier = NULL;
				

				//ArbreVariableGlobalInitialisation(ArbreVariableGlobal);
				//ArbreVariableLocalInitialisation(ArbreVariableLocal);
				print_symbol_table32(elfFile1, header1, section1, i, ArbreVariableGlobal, listeLocal);
				print_symbol_table32(elfFile2, header2, section2, j, ArbreVariableGlobal, listeLocal);
	
				sections_deja_fusionnees[j]=1;
				renumerotation_section2[j]=i;
				sectionOut.sh_name=section1.sh_name;
				sectionOut.sh_type=section1.sh_type;
				sectionOut.sh_flags=section1.sh_flags;
				sectionOut.sh_addr=section1.sh_addr;
				sectionOut.sh_offset=offset_actuel;
				sectionOut.sh_size = 0;
				sectionOut.sh_link=set_sh_link(sectionOut.sh_type, section1.sh_link,symtab_index);
				sectionOut.sh_info=section1.sh_info;
				sectionOut.sh_entsize=set_sh_entsize(sectionOut.sh_type);
				//On print la section
				fwrite(&sectionOut,sizeof(sectionOut),1,outputFile);
				curseur=ftell(outputFile);//On sauvegarde la position du curseur
				fseek(outputFile,sectionOut.sh_offset,SEEK_SET);//On va à la position de la section et on print les 2 sections à la suite
				ecritureSymbolLocalFichierElf(outputFile, &sectionOut, listeLocal);
				ecritureSymbolGlobalFichierElf(outputFile, &sectionOut, ArbreVariableGlobal);
				offset_actuel+=sectionOut.sh_size;

				fseek(outputFile,curseur,SEEK_SET); //On revient à la position initiale	

			} else {
				sections_deja_fusionnees[j]=1;
				renumerotation_section2[j]=i;
				sectionOut.sh_name=section1.sh_name;
				sectionOut.sh_type=section1.sh_type;
				sectionOut.sh_flags=section1.sh_flags;
				sectionOut.sh_addr=section1.sh_addr;
				sectionOut.sh_offset=offset_actuel;
				sectionOut.sh_size=section1.sh_size+section2.sh_size;
				offset_actuel+=sectionOut.sh_size;
				sectionOut.sh_link=set_sh_link(sectionOut.sh_type, section1.sh_link,symtab_index);
				sectionOut.sh_info=section1.sh_info;
				sectionOut.sh_entsize=set_sh_entsize(sectionOut.sh_type);
				//On print la section
				fwrite(&sectionOut,sizeof(sectionOut),1,outputFile);
				curseur=ftell(outputFile);//On sauvegarde la position du curseur
				fseek(outputFile,sectionOut.sh_offset,SEEK_SET);//On va à la position de la section et on print les 2 sections à la suite
				fseek(elfFile1,section1.sh_offset,SEEK_SET);
				file_copy(elfFile1,outputFile,section1.sh_size);
				fseek(elfFile2,section2.sh_offset,SEEK_SET);
				file_copy(elfFile2,outputFile,section2.sh_size);
				fseek(outputFile,curseur,SEEK_SET); //On revient à la position initiale	
			}
			
		}
		else{
			sectionOut.sh_name=section1.sh_name;
			sectionOut.sh_type=section1.sh_type;
			sectionOut.sh_flags=section1.sh_flags;
			sectionOut.sh_addr=section1.sh_addr;
			sectionOut.sh_offset=offset_actuel;
			sectionOut.sh_size=section1.sh_size;
			offset_actuel+=sectionOut.sh_size;
			sectionOut.sh_link=set_sh_link(sectionOut.sh_type,section1.sh_link,symtab_index);
			sectionOut.sh_info=section1.sh_info;
			sectionOut.sh_entsize=set_sh_entsize(sectionOut.sh_type);
			//On print la section
			fwrite(&sectionOut,sizeof(sectionOut),1,outputFile);
			curseur=ftell(outputFile);//On sauvegarde la position du curseur
			fseek(outputFile,sectionOut.sh_offset,SEEK_SET);//On va à la position de la section et on print les 2 sections à la suite
			fseek(elfFile1,section1.sh_offset,SEEK_SET);
			file_copy(elfFile1,outputFile,section1.sh_size);
			fseek(outputFile,curseur,SEEK_SET); //On revient à la position initiale
		}
	}
	k=0;
	for (int j=0;j<header2.e_shnum;j++){ //parcours des sections du fichier 2
		if (sections_deja_fusionnees[j]==0){
			renumerotation_section2[j]=k+header1.e_shnum;
			fseek(elfFile2, header2.e_shoff + j * header2.e_shentsize, SEEK_SET);
			litEtInverse_Section(elfFile2,header2,&section2);

			sectionOut.sh_name=section2.sh_name+taille_shtstrtab;
			sectionOut.sh_type=section2.sh_type;
			sectionOut.sh_flags=section2.sh_flags;
			sectionOut.sh_addr=section2.sh_addr;
			sectionOut.sh_offset=offset_actuel;
			sectionOut.sh_size=section2.sh_size;
			offset_actuel+=sectionOut.sh_size;
			sectionOut.sh_link=set_sh_link(sectionOut.sh_type, section1.sh_link, symtab_index);
			sectionOut.sh_info = set_sh_info(sectionOut.sh_type, section2.sh_info, renumerotation_section2);
			sectionOut.sh_entsize=set_sh_entsize(sectionOut.sh_type);
			fwrite(&sectionOut,sizeof(sectionOut),1,outputFile);
			curseur=ftell(outputFile);//On sauvegarde la position du curseur
			fseek(outputFile,sectionOut.sh_offset,SEEK_SET);//On va à la position de la section et on print les 2 sections à la suite
			fseek(elfFile2,section2.sh_offset,SEEK_SET);
			file_copy(elfFile2,outputFile,section2.sh_size);
			fseek(outputFile,curseur,SEEK_SET); //On revient à la position initiale

			k++;
		}
	}
	return 1;
}

void fusion_reimplementation(FILE* elfFile1, FILE* elfFile2,FILE* outputFile){
	Elf32_Ehdr headerOutput;
	fusion_header(elfFile1,elfFile2,outputFile,&headerOutput);
	fusion_section_2eme_tentative(elfFile1,elfFile2,outputFile,&headerOutput);
}
