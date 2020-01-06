#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>
#include "etape2-8.h"

int longueur_nom(char* nom){
	int j = 0;
	while (nom[j]!=0){
		j++;
	}
	return j;
}
void ajouter_nom(char* nom, char* table_nom,int fin_tab){
	int i=fin_tab,j;
	for(j=0;j<longueur_nom(nom);j++){
		table_nom[i]=nom[j];
		i++;
	}
}

void file_copy(FILE* file1, FILE* file2, size_t size){
	char* ptr=malloc(size);
	fread(ptr,size,1,file1);
	fwrite(ptr,size,1,file2);
	free(ptr);
}

int est_nom_dans_tab(char* nom, char* table_nom){
	int i=0;
	int j=0;
	while(table_nom[i]!=0 || table_nom[i+1]!=0){
		while(table_nom[i+j]==nom[j] && j<longueur_nom(nom)){
			j++;
		}
		if (j==longueur_nom(nom)) return 1;
		else j=0;
		i++;
	}
	return 0;
}

size_t get_size_name_table(char* table_nom){
	int k=0;
	while(table_nom[k]!=0 || table_nom[k+1]!=0){
		k++;
	}
	return k+1;
}
void fusion_reimplementation(FILE* elfFile1, FILE* elfFile2,FILE* outputFile){
	FILE * tempFile;
	uint32_t section_name=0;
	Elf32_Ehdr header1;
	Elf32_Ehdr header2;
	Elf32_Ehdr headerOutput;
	Elf32_Shdr section1;
	Elf32_Shdr section2;
	Elf32_Shdr sectionOut;

	char tabNomSection[10000]="";
	char* nom_section1=NULL;
	char* nom_section2=NULL;
	tempFile = fopen("tempFileSectionTable","w+");
	litEtInverse_Header(elfFile1,&header1);
	litEtInverse_Header(elfFile2,&header2);

	for (int i=0;i<EI_NIDENT;i++){
		headerOutput.e_ident[i]=header1.e_ident[i];
	}
	headerOutput.e_ident[5]=ELFDATA2LSB; //je triche j'écris en little endian quoiqu'il arrive
	headerOutput.e_type=header1.e_type;
	headerOutput.e_machine=header1.e_machine;
	headerOutput.e_version=header1.e_version;
	headerOutput.e_entry=0;
	headerOutput.e_phoff=0;
	headerOutput.e_shoff=sizeof(headerOutput);
	headerOutput.e_flags=header1.e_flags;
	headerOutput.e_phnum=0;
	headerOutput.e_shentsize=sizeof(Elf32_Shdr);
	headerOutput.e_shnum=0; //Initialisé à 0, incrémenté petit à petit
	int offset_sections=sizeof(headerOutput)+(header2.e_shnum+header1.e_shnum)*headerOutput.e_shentsize;
	int offset_actuel=offset_sections;
	// check so its really an elf file
	if (memcmp(header1.e_ident, ELFMAG, SELFMAG) == 0 && memcmp(header2.e_ident, ELFMAG, SELFMAG) == 0 ) {
		fseek(outputFile,sizeof(headerOutput),SEEK_SET);
		for (int i=0; i<header1.e_shnum;i++){ //parcours des sections du fichier 1
			fseek(elfFile1, header1.e_shoff + i * header1.e_shentsize, SEEK_SET);
			litEtInverse_Section(elfFile1,header1,&section1);
			nom_section1=get_section_name(elfFile1,header1,section1);
			for (int j=0;j<header2.e_shnum;j++){ //parcours des sections du fichier 2
				fseek(elfFile2, header2.e_shoff + j * header2.e_shentsize, SEEK_SET);
				litEtInverse_Section(elfFile2,header2,&section2);
				nom_section2=get_section_name(elfFile2,header2,section2);
				if(!strcmp(nom_section1,nom_section2)&&section1.sh_type!=SHT_STRTAB){ //si les sections ont le meme nom
					ajouter_nom(nom_section1,tabNomSection,section_name);
					sectionOut.sh_name=section_name;
					section_name+=longueur_nom(nom_section1)+1;
					sectionOut.sh_type=section1.sh_type;
					sectionOut.sh_flags=section1.sh_flags;
					sectionOut.sh_addr=section1.sh_addr;
					sectionOut.sh_offset=offset_actuel;
					sectionOut.sh_size=section1.sh_size+section2.sh_size;
					offset_actuel+=sectionOut.sh_size;
					sectionOut.sh_link=headerOutput.e_shnum;
					headerOutput.e_shnum+=1;
					sectionOut.sh_info=0;
					fwrite(&sectionOut,sizeof(sectionOut),1,outputFile);
					fseek(elfFile1,section1.sh_offset,SEEK_SET);
					file_copy(elfFile1,tempFile,sizeof(section1.sh_size));
					fseek(elfFile2,section2.sh_offset,SEEK_SET);
					file_copy(elfFile2,tempFile,sizeof(section2.sh_size));
				}
			}
		}
		for (int i=0; i<header1.e_shnum;i++){ //parcours des sections du fichier 1
			fseek(elfFile1, header1.e_shoff + i * header1.e_shentsize, SEEK_SET);
			litEtInverse_Section(elfFile1,header1,&section1);
			char* tmp = get_section_name(elfFile1,header1,section1);
			if(!est_nom_dans_tab(tmp,tabNomSection)&&section1.sh_type!=SHT_STRTAB){
				ajouter_nom(nom_section1,tabNomSection,section_name);
				sectionOut.sh_name=section_name;
				section_name+=longueur_nom(nom_section1)+1;
				sectionOut.sh_type=section1.sh_type;
				sectionOut.sh_flags=section1.sh_flags;
				sectionOut.sh_addr=section1.sh_addr;
				sectionOut.sh_offset=offset_actuel;
				sectionOut.sh_size=section1.sh_size;
				offset_actuel+=sectionOut.sh_size;
				sectionOut.sh_link=headerOutput.e_shnum;
				headerOutput.e_shnum+=1;
				sectionOut.sh_info=0;
				fwrite(&sectionOut,sizeof(sectionOut),1,outputFile);
				fseek(elfFile1,section1.sh_offset,SEEK_SET);
				file_copy(elfFile1,tempFile,sizeof(section1.sh_size));
			}
			free(tmp);
		}
		for (int i=0; i<header2.e_shnum;i++){ //parcours des sections du fichier 2
			fseek(elfFile2, header2.e_shoff + i * header2.e_shentsize, SEEK_SET);
			litEtInverse_Section(elfFile2,header2,&section2);
			char* tmp = get_section_name(elfFile2,header2,section2);
			if(!est_nom_dans_tab(tmp,tabNomSection)&&section2.sh_type!=SHT_STRTAB){
				ajouter_nom(nom_section2,tabNomSection,section_name);
				sectionOut.sh_name=section_name;
				section_name+=longueur_nom(nom_section2)+1;
				sectionOut.sh_type=section2.sh_type;
				sectionOut.sh_flags=section2.sh_flags;
				sectionOut.sh_addr=section2.sh_addr;
				sectionOut.sh_offset=offset_actuel;
				offset_actuel+=sectionOut.sh_size;
				sectionOut.sh_link=0;
				headerOutput.e_shnum+=1;
				sectionOut.sh_info=0;
				fwrite(&sectionOut,sizeof(sectionOut),1,outputFile);
				fseek(elfFile2,section2.sh_offset,SEEK_SET);
				file_copy(elfFile2,tempFile,sizeof(section2.sh_size));
			}
			free(tmp);
		}

		//Ajout de l'entete de section shtstr
		ajouter_nom(".shstrtab",tabNomSection,section_name);
		sectionOut.sh_name=section_name;
		sectionOut.sh_type=SHT_STRTAB;
		sectionOut.sh_flags=0;
		sectionOut.sh_addr=0;
		sectionOut.sh_offset=offset_actuel;
		sectionOut.sh_size=sizeof(sectionOut);
		fwrite(&sectionOut,sizeof(sectionOut),1,outputFile);
		headerOutput.e_shstrndx=headerOutput.e_shnum;
		headerOutput.e_shnum+=1;


		//Copie de la table des chaines des noms de section dans tempFile
		size_t sz=get_size_name_table(tabNomSection);
		fwrite(tabNomSection,sz,1,tempFile);
		//Concaténation de tempFile à outputFile
		fseek(outputFile,offset_sections,SEEK_SET);
		char ch;
		fseek(tempFile,0,SEEK_SET);
		while((ch = fgetc(tempFile)) != EOF)
        fputc(ch,outputFile);

		//Ajout du header
		fseek(outputFile,0,SEEK_SET);
		fwrite(&headerOutput,sizeof(headerOutput),1,outputFile);

		//Correction des offsets
		for (int i=0; i<headerOutput.e_shnum;i++){
			fseek(outputFile, headerOutput.e_shoff + i * headerOutput.e_shentsize, SEEK_SET);
			litEtInverse_Section(outputFile,headerOutput,&sectionOut);
			printf("%d -> %d prochain devrait être %d\n", i,sectionOut.sh_offset,sectionOut.sh_offset+sectionOut.sh_size);
		}

	}
	fclose(tempFile);
}
