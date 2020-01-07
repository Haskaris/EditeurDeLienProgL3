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

void init_header(Elf32_Ehdr* headerOutput, Elf32_Ehdr header1, Elf32_Ehdr header2) {
	for (int i=0;i<EI_NIDENT;i++){
		headerOutput->e_ident[i]=header1.e_ident[i];
	}
	headerOutput->e_ident[EI_DATA]=ELFDATA2LSB; //je triche j'écris en little endian quoiqu'il arrive

	//Ce champ de la structure identifie le type de fichier objet
	if (header1.e_type == header2.e_type) {
		//Si c'est le même type
		headerOutput->e_type = header1.e_type;
	} else {
		//Fusion de deux types différents
		headerOutput->e_type = ET_NONE;
	}

	//Ce champ indique l'architecture nécessaire à un fichier individuel.
	if (header1.e_machine == header2.e_machine) {
		//Si c'est la même machine
		headerOutput->e_machine = header1.e_machine;
	} else {
		//Fusion de deux machines différentes
		headerOutput.e_machine = EM_NONE;
	}

	//Ce champ indique la version du fichier
	headerOutput->e_version = EV_CURRENT;

	//Ce champ indique l'adresse virtuelle à laquelle le système transfère initialement le contrôle,
	//démarrant ainsi le processus. Si ce fichier ne possède pas de point d'entrée, ce champ contient zéro.
	/*headerOutput.e_entry=0;*/
	if (header1.e_entry != 0 && header2.e_entry != 0) {
		//Chacun a une entrée, on prend la plus faible
		headerOutput->e_entry = min(header1.e_entry, header2.e_entry);
	} else {
		//Une entrée ou plus est égale à zéro, on prend donc celle qui n'est potentiellement
		//pas égale à zéro (Si les deux valent zéro le max est zéro)
		headerOutput->e_entry = max(header1.e_entry, header2.e_entry);
	}

	//Ce champ contient le décalage en octets de la table contenant l'entête de programme.
	//Si ce fichier ne contient pas de table d'entête de programme, ce champ contient zéro.
	/*headerOutput.e_phoff=0;*/
	if (header1.e_phoff != 0 && header2.e_phoff != 0) {
		//Chacun a un décalage, on prend le plus faible
		headerOutput->e_phoff = min(header1.e_phoff, header2.e_phoff);
	} else {
		//Un décalage ou plus est égal à zéro, on prend donc celui qui n'est potentiellement
		//pas égal à zéro (Si les deux valent zéro le max est zéro)
		headerOutput->e_phoff = max(header1.e_phoff, header2.e_phoff);
	}

	//Ce champ contient le décalage en octets de la table des entêtes de sections.
	//Si ce fichier ne contient pas de table des entêtes des sections, ce champ contient zéro.
	/*headerOutput.e_shoff=sizeof(headerOutput);*/
	if (header1.e_shoff != 0 && header2.e_shoff != 0) {
		//Chacun a un décalage, on prend le plus faible
		headerOutput->e_shoff = min(header1.e_shoff, header2.e_shoff);
	} else {
		//Un décalage ou plus est égal à zéro, on prend donc celui qui n'est potentiellement
		//pas égal à zéro (Si les deux valent zéro le max est zéro)
		headerOutput->e_shoff = max(header1.e_shoff, header2.e_shoff);
	}
	//Ce champ contient des drapeaux spécifiques au processeur. Le nom de ces drapeaux prend la forme :
	//« EF_machine_drapeau ». À l'heure actuelle, aucun drapeau n'a été défini.
	headerOutput->e_flags=header1.e_flags;

	//Ce champ contient la taille de l'entête ELF en octets.
	headerOutput->e_ehsize = sizeof(Elf32_Ehdr);

	//Ce champ contient la taille en octets d'une entrée de la table d'entête de
	// programme ; toutes les entrées ont la même taille.
	headerOutput->e_phentsize = max(header1.e_phentsize, header2.e_phentsize);

	//Ce champ contient le nombre d'entrées de la table d'entête de programme.
	//Ainsi, la taille en octets de la table pourra être obtenue en multipliant
	//e_phentsize par e_phnum. Si le fichier ne contient pas d'entête de programme,
	//e_phnum contiendra la valeur zéro.
	/*headerOutput.e_phnum=0;*/
	headerOutput->e_phnum = header1.e_phnum + header2.e_phnum;

	//Ce champ contient la taille en octets d'un entête de section. Un entête de
	//section est une entrée de la table des entêtes de sections ; toutes les
	//entrées ont la même taille.
	headerOutput->e_shentsize=sizeof(Elf32_Shdr);

	headerOutput->e_shnum=0; //Initialisé à 0, incrémenté petit à petit
}

// chaque appel, alloue de la mémoire
void add_table(char* table, char* nom) {
	strncat(table, nom);
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
	char* tableChaineEntete = malloc(sizeof(char));
	uint32_t positionDansTable = 0;
	char* nom_section1=NULL;
	char* nom_section2=NULL;
	tempFile = fopen("tempFileSectionTable","w+");
	litEtInverse_Header(elfFile1,&header1);
	litEtInverse_Header(elfFile2,&header2);

	init_header(&headerOutput, header1, header2);

	//Calcul des offsets
	int offset_sections = /*sizeof(headerOutput)+*/(header2.e_shnum+header1.e_shnum) * headerOutput.e_shentsize;
	int offset_actuel = offset_sections;

	//parcours des sections du fichier 1
	for (int i=0; i<header1.e_shnum; i++) {
		fseek(elfFile1, header1.e_shoff + i * header1.e_shentsize, SEEK_SET);
		litEtInverse_Section(elfFile1, header1, &section1);
		nom_section1 = get_section_name(elfFile1, header1, section1);

		//parcours des sections du fichier 2
		for (int j=0; j<header2.e_shnum; j++) {
			fseek(elfFile2, header2.e_shoff + j * header2.e_shentsize, SEEK_SET);
			litEtInverse_Section(elfFile2, header2, &section2);
			nom_section2 = get_section_name(elfFile2, header2, section2);

			//Si ce sont les mêmes sections
			if (!strcmp(nom_section1, nom_section2)) {

				//On ajoute la section dans la table
				add_table(tableChaineEntete, nom_section1);

				/*Ce champ indique le nom de la section. Sa valeur est un indice de la
				 * table des chaînes des entêtes de sections, contenant l'emplacement
				 * d'une chaîne terminée par un octet nul.
				 */
				sectionOut.sh_name = positionDansTable;

				//Calcul la position du prochain nom dans la table (+1 pour '\0')
				positionDansTable += strlen(nom_section1) + 1;

				//Ce champ définit le contenu et la sémantique de la section.
				sectionOut.sh_type = section1.sh_type;

				/*Les sections contiennent des drapeaux binaires décrivant divers
				 * attributs. Si, dans sh_flags, le bit correspondant à un drapeau est
				 * positionné, l'attribut est activé pour cette section. Sinon, l'attribut
				 * est désactivé ou ne s'applique pas. Les attributs non définis sont mis
				 * à zéro.
				 */
				sectionOut.sh_flags = section1.sh_flags;

				/*Si cette section apparaît dans l'image mémoire d'un processus,
				 * ce champ contient l'adresse à laquelle le premier octet de la section
				 * doit se trouver. Sinon, ce champ contient zéro.
				 */
				sectionOut.sh_addr = section1.sh_addr;

				/*La valeur de ce champ indique le décalage du premier octet de la
				 * section par rapport au début du fichier. Une section de type SHT_NOBITS,
				 * n'occupe pas de place dans le fichier et son champ sh_offset indique
				 * son placement théorique dans le fichier.
				 */
				sectionOut.sh_offset = offset_actuel;

				/*Ce champ contient la taille de la section en octets. À moins que cette
				 * section ne soit de type SHT_NOBITS, elle occupe sh_size octets dans le
				 * fichier. Une section de type SHT_NOBITS peut avoir une taille non nulle,
				 * mais elle n'occupera cependant aucune place dans le fichier.
				 */
				sectionOut.sh_size = section1.sh_size + section2.sh_size;

				//Calcul du nouvel offset
				offset_actuel+=sectionOut.sh_size;

				/*Ce champ contient un lien vers un indice de la table des entêtes de
				 * sections, dont l'interprétation dépend du type de section.
				 */
				sectionOut.sh_link = headerOutput.e_shnum;

				//Mise à jour du nombre de section
				headerOutput.e_shnum+=1;

				/*Ce champ contient des informations complémentaires, dont
				 * l'interprétation dépend du type de section.
				 */
				sectionOut.sh_info = section1.sh_info;
				fwrite(&sectionOut,sizeof(Elf32_Shdr),1,tempFile);
				/*fseek(elfFile1, section1.sh_offset, SEEK_SET);
				file_copy(elfFile1, tempFile, sizeof(section1.sh_size));
				fseek(elfFile2, section2.sh_offset, SEEK_SET);
				file_copy(elfFile2, tempFile, sizeof(section2.sh_size));*/
			}
		}
	}

	/*int offset_sections=sizeof(headerOutput)+(header2.e_shnum+header1.e_shnum)*headerOutput.e_shentsize;
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
		char ch;////
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

	}*/
			//Ajout du header
			fseek(outputFile,0,SEEK_SET);
			fwrite(&headerOutput,sizeof(headerOutput),1,outputFile);
	fclose(tempFile);
}
