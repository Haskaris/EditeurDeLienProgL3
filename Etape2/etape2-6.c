//Inclure les autres du .h ?
#include "etape2-6.h"

/*
 * A préciser / Changer pour get ?
 */
uint32_t set_sh_link(uint32_t type, uint32_t section1_link, int symtab_index) {
	switch(type) {
		case SHT_REL:
			return symtab_index;
		case SHT_SYMTAB:
			return section1_link;
		default:
			return 0;
	}
}

/*
 * A préciser
 */
int get_symtab_index(FILE* fichier, Elf32_Ehdr header) {
	Elf32_Shdr section;
	for(int i = 0; i < header.e_shnum; i++) {
		fseek(fichier, header.e_shoff + i * header.e_shentsize, SEEK_SET);
		litEtInverse_Section(fichier, header, &section);
		if (section.sh_type == SHT_SYMTAB) {
			return i;
		}
	}
	return -1;
}

/*
 * A préciser / Changer pour get ?
 */
uint32_t set_sh_info(uint32_t type, uint32_t info, int* renumerotation_section2) {
	if (type == SHT_REL){
		return renumerotation_section2[info];
	}
	return 0;
}

/*
 * Fusionne les headers de deux fichiers
 * Paramètres:
 * - Fichier à fusionner 1 (elfFile1)
 * - Fichier à fusionner 2 (elfFile2)
 * - Fichier fusionné (outputFile)
 * - Header fusionné (headerOutput)
 */
int fusion_header(FILE* elfFile1, FILE* elfFile2, FILE* outputFile, Elf32_Ehdr* headerOutput) {
	//Headers des fichiers 1 et 2
	Elf32_Ehdr header1;
	Elf32_Ehdr header2;

	//Sections des fichiers 1 et 2
	Elf32_Shdr section1;
	Elf32_Shdr section2;

	//Nom des sections
	char* nom_section1 = NULL;
	char* nom_section2 = NULL;

	litEtInverse_Header(elfFile1,&header1);
	litEtInverse_Header(elfFile2,&header2);

	if (header1.e_type == header2.e_type) {
		//Si c'est le même type
		for (int i = 0; i < EI_NIDENT; i++){
			headerOutput->e_ident[i] = header1.e_ident[i];
		}
		//TODO: A changer pour faire little et big endian
		headerOutput->e_ident[EI_DATA] = ELFDATA2LSB; //Ecriture en little endian quoi qu'il se passe

		//Ce champ de la structure identifie le type de fichier objet
		headerOutput->e_type = header1.e_type;

		//Ce champ indique l'architecture nécessaire à un fichier individuel.
		if (header1.e_machine == header2.e_machine) {
			//Si c'est la même machine
			headerOutput->e_machine = header1.e_machine;
		} else {
			//Fusion de deux machines différentes
			headerOutput->e_machine = EM_NONE;
		}

		//Ce champ indique la version du fichier
		headerOutput->e_version = header1.e_version;

		//Ce champ indique l'adresse virtuelle à laquelle le système transfère initialement le contrôle,
		//démarrant ainsi le processus. Si ce fichier ne possède pas de point d'entrée, ce champ contient zéro.
		headerOutput->e_entry = 0;
		//TODO: Est-ce que ça ne serait pas mieux ?
		/*
		if (header1.e_phoff != 0 && header2.e_phoff != 0) {
			//Chacun a un décalage, on prend le plus faible
			headerOutput->e_phoff = min(header1.e_phoff, header2.e_phoff);
		} else {
			//Un décalage ou plus est égal à zéro, on prend donc celui qui n'est potentiellement
			//pas égal à zéro (Si les deux valent zéro le max est zéro)
			headerOutput->e_phoff = max(header1.e_phoff, header2.e_phoff);
		}
		*/

		//Ce champ contient le décalage en octets de la table contenant l'entête de programme.
		//Si ce fichier ne contient pas de table d'entête de programme, ce champ contient zéro.
		headerOutput->e_phoff = 0;
		//TODO: Est-ce que ce ne serait pas mieux ?
		/*
		if (header1.e_phoff != 0 && header2.e_phoff != 0) {
			//Chacun a un décalage, on prend le plus faible
			headerOutput->e_phoff = min(header1.e_phoff, header2.e_phoff);
		} else {
			//Un décalage ou plus est égal à zéro, on prend donc celui qui n'est potentiellement
			//pas égal à zéro (Si les deux valent zéro le max est zéro)
			headerOutput->e_phoff = max(header1.e_phoff, header2.e_phoff);
		}
		*/

		//Ce champ contient le décalage en octets de la table des entêtes de sections.
		//Si ce fichier ne contient pas de table des entêtes des sections, ce champ contient zéro.
		//TODO: Changer pour ajouter la condition
		headerOutput->e_shoff = sizeof(Elf32_Ehdr);

		//Ce champ contient des drapeaux spécifiques au processeur. Le nom de ces drapeaux prend la forme :
		//« EF_machine_drapeau ». À l'heure actuelle, aucun drapeau n'a été défini.
		headerOutput->e_flags = header1.e_flags;

		//Ce champ contient le nombre d'entrées de la table d'entête de programme.
		//Ainsi, la taille en octets de la table pourra être obtenue en multipliant
		//e_phentsize par e_phnum. Si le fichier ne contient pas d'entête de programme,
		//e_phnum contiendra la valeur zéro.
		headerOutput->e_phnum = 0;
		//TODO: Est-ce que ce ne serait pas mieux ?
		/*
		headerOutput->e_phnum = header1.e_phnum + header2.e_phnum;
		*/

		//Ce champ contient la taille en octets d'un entête de section. Un entête de
		//section est une entrée de la table des entêtes de sections ; toutes les
		//entrées ont la même taille.
		headerOutput->e_shentsize = sizeof(Elf32_Shdr);

		headerOutput->e_shstrndx = header1.e_shstrndx;

		headerOutput->e_shnum = header1.e_shnum + header2.e_shnum; //On l'initialise à la somme des 2 shnum

		//Calcul du shnum : on le soustrait de 1 pour chaque section en commun
		//Parcours des sections du fichier 1
		for (int i = 0; i < header1.e_shnum; i++) {
			fseek(elfFile1, header1.e_shoff + i * header1.e_shentsize, SEEK_SET);
			litEtInverse_Section(elfFile1,header1,&section1);
			nom_section1=get_section_name(elfFile1,header1,section1);
			//Parcours des sections du fichier 2
			for (int j=0;j<header2.e_shnum;j++){
				fseek(elfFile2, header2.e_shoff + j * header2.e_shentsize, SEEK_SET);
				litEtInverse_Section(elfFile2, header2, &section2);
				nom_section2=get_section_name(elfFile2, header2, section2);
				//Si les sections ont le meme nom
				if(!strcmp(nom_section1, nom_section2)) {
					//On soustrait le shnum de 1
					headerOutput->e_shnum--;
				}
			}
		}
	} else {
		//Pas le même type de fichier
		printf("On ne peut pas fusionner deux fichiers de type différents.\n");
		return 1;
	}
	//On print le header dans le fichier output
	fwrite(headerOutput, sizeof(Elf32_Ehdr), 1, outputFile);
	return 0; //La fusion s'est bien passée
}

/*
 * Fusionne les sections de deux fichiers
 * Paramètres:
 * - Fichier à fusionner 1 (elfFile1)
 * - Fichier à fusionner 2 (elfFile2)
 * - Fichier fusionné (outputFile)
 * - Header fusionné au préalable (headerOutput)
 */
int fusion_section(FILE* elfFile1, FILE* elfFile2, FILE* outputFile, Elf32_Ehdr* headerOutput) {
	//Headers des fichiers 1 et 2
	Elf32_Ehdr header1;
	Elf32_Ehdr header2;

	//Sections des fichiers 1 et 2 ; Section de sortie
	Elf32_Shdr section1;
	Elf32_Shdr section2;
	Elf32_Shdr sectionOut;

	//Décalage
	int offset_actuel = sizeof(Elf32_Ehdr) + headerOutput->e_shnum * headerOutput->e_shentsize;
	//Permet de sauvegarder la position du curseur dans le fichier
	int curseur = 0;
	//Permet de sauvegarder l'index de symtab_index
	int symtab_index = 0;


	//Permet de sauvegarder la taille de shtstrtab
	size_t taille_shtstrtab;

	//Nom des sections
	char* nom_section1 = NULL;
	char* nom_section2 = NULL;

	//On se repositionne au début de chaque fichier
	fseek(elfFile1, 0, SEEK_SET);
	fseek(elfFile2, 0, SEEK_SET);

	litEtInverse_Header(elfFile1,&header1);
	litEtInverse_Header(elfFile2,&header2);

	//Permet de mémoriser les sections déja fusionnées du deuxième fichier
	int sections_deja_fusionnees[header2.e_shnum];
	//Permet de connaitre l'indice recherché
	//Si i est l'ancien indice, nouvel indice est égal à renumerotation_section2[i]
	int renumerotation_section2[header2.e_shnum];

	//Initialise des tableau à 0 et -1
	for (int i = 0; i < header2.e_shnum; i++) {
		sections_deja_fusionnees[i] = 0;
		renumerotation_section2[i] = -1;
	}

	symtab_index = get_symtab_index(elfFile1,header1);

	//Parcours des sections du fichier 1
	for (int i = 0; i < header1.e_shnum; i++) {
		int j=0;
		fseek(elfFile1, header1.e_shoff + i * header1.e_shentsize, SEEK_SET);
		litEtInverse_Section(elfFile1, header1, &section1);
		nom_section1 = get_section_name(elfFile1, header1, section1);

		if (header1.e_shstrndx == i) {
			taille_shtstrtab = section1.sh_size;
		}

		fseek(elfFile2, header2.e_shoff + j * header2.e_shentsize, SEEK_SET);
		litEtInverse_Section(elfFile2, header2, &section2);
		nom_section2 = get_section_name(elfFile2, header2, section2);

		//Parcours des sections du fichier 2
		//Tant que les sections n'ont pas le même nom et pas le même type
		while((strcmp(nom_section1, nom_section2) || section1.sh_type != section2.sh_type)
						&& (j < header2.e_shnum)) {
			j++;
			fseek(elfFile2, header2.e_shoff + j * header2.e_shentsize, SEEK_SET);
			litEtInverse_Section(elfFile2, header2, &section2);
			nom_section2 = get_section_name(elfFile2, header2, section2);
		}
		//Si une section de même nom et de même type a été trouvée
		if (j < header2.e_shnum) {
			sections_deja_fusionnees[j] = 1;
			renumerotation_section2[j] = i;
			sectionOut.sh_name = section1.sh_name;
			sectionOut.sh_type = section1.sh_type;
			sectionOut.sh_flags = section1.sh_flags;
			sectionOut.sh_addr = section1.sh_addr;
			sectionOut.sh_offset = offset_actuel;
			sectionOut.sh_size = section1.sh_size + section2.sh_size;
			offset_actuel += sectionOut.sh_size;
			sectionOut.sh_link = set_sh_link(sectionOut.sh_type,section1.sh_link,symtab_index);
			sectionOut.sh_info = section1.sh_info;
			sectionOut.sh_entsize = section1.sh_entsize;
			sectionOut.sh_addralign=section1.sh_addralign;

			//On print la section
			fwrite(&sectionOut, sizeof(sectionOut), 1, outputFile);
			//On sauvegarde la position du curseur
			curseur = ftell(outputFile);
			//On va à la position de la section et on print les 2 sections à la suite
			fseek(outputFile,sectionOut.sh_offset,SEEK_SET);
			fseek(elfFile1,section1.sh_offset,SEEK_SET);
			file_copy(elfFile1,outputFile,section1.sh_size);
			fseek(elfFile2,section2.sh_offset,SEEK_SET);
			file_copy(elfFile2,outputFile,section2.sh_size);
			//On revient à la position initiale
			fseek(outputFile,curseur,SEEK_SET);
		} else {
			//Pas de section de même nom et même type
			//On print la section 1
			section1.sh_offset = offset_actuel;
			offset_actuel += section1.sh_size;
			fwrite(&section1,sizeof(section1),1,outputFile);
			//On sauvegarde la position du curseur
			curseur = ftell(outputFile);
			//On va à la position de la section
			fseek(outputFile, section1.sh_offset, SEEK_SET);
			fseek(elfFile1, section1.sh_offset, SEEK_SET);
			file_copy(elfFile1, outputFile, section1.sh_size);
			//On revient à la position initiale
			fseek(outputFile, curseur, SEEK_SET);
		}
	}
	int k = 0;
	//Parcours des sections du fichier 2
	for (int j = 0; j < header2.e_shnum; j++) {
		//Si la section n'est pas déjà fusionnée
		if (sections_deja_fusionnees[j] == 0) {
			renumerotation_section2[j] = k + header1.e_shnum;
			fseek(elfFile2, header2.e_shoff + j * header2.e_shentsize, SEEK_SET);
			litEtInverse_Section(elfFile2, header2, &section2);

			//On print la section 2 avec quelques modifications.
			sectionOut.sh_name = section2.sh_name + taille_shtstrtab;
			sectionOut.sh_type = section2.sh_type;
			sectionOut.sh_flags = section2.sh_flags;
			sectionOut.sh_addr = section2.sh_addr;
			sectionOut.sh_offset = offset_actuel;
			sectionOut.sh_size = section2.sh_size;
			offset_actuel += sectionOut.sh_size;
			sectionOut.sh_link = set_sh_link(sectionOut.sh_type, section1.sh_link, symtab_index);
			sectionOut.sh_info = set_sh_info(sectionOut.sh_type, section2.sh_info, renumerotation_section2);
			sectionOut.sh_entsize = section2.sh_entsize;
			sectionOut.sh_addralign=section2.sh_addralign;

			fwrite(&sectionOut, sizeof(sectionOut), 1, outputFile);
			//On sauvegarde la position du curseur
			curseur = ftell(outputFile);
			//On va à la position de la section
			fseek(outputFile, sectionOut.sh_offset, SEEK_SET);
			fseek(elfFile2, section2.sh_offset, SEEK_SET);
			file_copy(elfFile2, outputFile, section2.sh_size);
			//On revient à la position initiale
			fseek(outputFile, curseur, SEEK_SET);
			k++;
		}
	}
	return 0;
}

int fusion_2_6(FILE * elfFile1, FILE * elfFile2, FILE * outputFile) {
	//Permet de savoir si ça s'est bien passé ou non
	int deroulement = 0;
	//TODO: Le changer en pointeur ?
	Elf32_Ehdr headerOutput;

	deroulement = fusion_header(elfFile1, elfFile2, outputFile, &headerOutput);

	//Il y a eu un problème ?
	if (deroulement) {
		//Oui
		return 1;
	}

	deroulement = fusion_section(elfFile1, elfFile2, outputFile, &headerOutput);

	//Il y a eu un problème ?
	if (deroulement) {
		//Oui
		return 1;
	}

	return 0;
}
