#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>
#include "etape2-6.h"
#include "etape2-7.h"
#include "etape2-8.h"

int fusion_2_7(FILE * elfFile1, FILE * elfFile2, FILE * outputFile) {
	//Permet de savoir si ça s'est bien passé ou non
	int deroulement = 0;
	Elf32_Ehdr headerOutput;

	deroulement = fusion_header(elfFile1, elfFile2, outputFile, &headerOutput);

	//Il y a eu un problème ?
	if (deroulement) {
		//Oui
		return 1;
	}

	deroulement = fusion_section_2_7(elfFile1, elfFile2, outputFile, &headerOutput);

	//Il y a eu un problème ?
	if (deroulement) {
		//Oui
		return 1;
	}

	return 0;
}

void insererEnTeteListe(Liste *listeLocal, Elf32_Sym table_symbole){
	NoeudLocal *nouveauNoeud = malloc(sizeof(NoeudLocal));
	nouveauNoeud->suivant = listeLocal->premier;
	nouveauNoeud->symboleCourant = table_symbole;
	listeLocal->premier = nouveauNoeud;
}

void afficheSymbole(Elf32_Sym table_symbole){
		printf("	%08x ", table_symbole.st_value);
		printf(" %u ", table_symbole.st_size);
		switch (ELF32_ST_TYPE(table_symbole.st_info)) {
			case 0:
				printf("	NOTYPE	");
				break;
			case 1:
				printf("	OBJT	");
				break;
			case 2:
				printf("	FUNC	");
				break;
			case 3:
				printf("	SECTION	");
				break;
			case 4:
				printf("	FILE	");
				break;
			default:
				printf(" Cas non géré fichier enteteEtape4.c fonction print_symbol_tabl"
								"e32 switch ELF32_ST_TYPE ");
		}
		switch(ELF32_ST_BIND(table_symbole.st_info)) {
			case 0:
				printf(" LOCAL ");
				break;
			case 1:
				printf(" GLOBAL ");
				break;
			case 2:
				printf(" WEAK ");
				break;
			case 3:
				printf("	NUM ");
				break;
			default:
				printf(" Cas non géré fichier enteteEtape4.c fonction print_symbol_tabl"
					"e32 switch ELF32_ST_BIND ");
				break;
		}
		switch(ELF32_ST_VISIBILITY(table_symbole.st_other)) {
			case 0:
				printf(" DEFAULT ");
				break;
			case 1:
				printf(" INTERNAL ");
				break;
			case 2:
				printf(" HIDDEN ");
				break;
			case 3:
				printf(" PROTECTED ");
				break;
			default:
				printf(" Cas non géré fichier enteteEtape4.c fonction print_symbol_tabl"
					"e64 switch ELF64_ST_VISIBILITY ");
				break;
		}
		printf(" %x	", table_symbole.st_shndx);
		printf(" %02x\n", table_symbole.st_name);
}

void verificationSymboleGlobal(Elf32_Sym symbole, struct Noeud *ArbreVariableGlobal){
	struct Noeud* symboleGlobal = dansArbreDeRecherche(ArbreVariableGlobal, symbole);
	if (!symboleGlobal){
		/*
			- si un symbole n’apparaı̂t que dans l’une des deux tables en entrée, il devra
				apparaı̂tre dans la table de sortie
		*/
		insereNoeudDansArbre(ArbreVariableGlobal, symbole);
	} else {
		//Deux symboles de même nom
		/*
			- si un symbole défini dans l’une des deux tables apparaît comme non défini
				dans l’autre, seule la définition devra apparaître
				dans la table de sortie
			- si deux symboles apparaissent comme non définis dans les deux tables en
				entrée, une seule entrée pour ce symbole devra être
				présente dans la table de sortie
		*/
		if(symboleGlobal->symboleCourant.st_shndx == 0){
			symboleGlobal->symboleCourant = symbole;
		} else if (symboleGlobal->symboleCourant.st_shndx != 0 || symbole.st_shndx != 0) {
			/*
				- si deux symboles de même nom sont définis dans les tables en entrée,
				l’édition de liens échoue de manière définitive
			*/
			printf("2 symboles globaux de même nom, echec de l'édition de lien (etape2-7.c)\n");
			exit(0);
		}
	}
}


int ecritureSymbolGlobalFichierElf(FILE* elfFileDest, Elf32_Shdr *section, Noeud *noeud, int indice){
	if (noeud == NULL){
		return indice;
	} else {
		afficheSymbole(noeud->symboleCourant);
		fwrite(&(noeud->symboleCourant), sizeof(noeud->symboleCourant), 1, elfFileDest);
		//On augmente la taille de la section car on a ajouté un symbole
		indice = ecritureSymbolGlobalFichierElf(elfFileDest, section, noeud->noeudGauche, indice + 1);
		indice = ecritureSymbolGlobalFichierElf(elfFileDest, section, noeud->noeudDroit, indice + 1);
	}
	return indice;
}

/* PROBLEME
 * Variable Global MAIN présente dans toutes
 * les tables de symbôle, comment faire ???
*/
int ecritureSymbolLocalFichierElf(FILE* elfFileDest, Elf32_Shdr *section, struct Liste *listeLocal, int indice){
	NoeudLocal *noeud = listeLocal->premier;
	indice--;
	while (noeud != NULL){
		if (noeud->suivant == NULL){
			return indice;
		}
		indice++;
		afficheSymbole(noeud->symboleCourant);
		fwrite(&(noeud->symboleCourant), sizeof(noeud->symboleCourant), 1, elfFileDest);
		//On augmente la taille de la section car on a ajouté un symbole
		noeud = noeud->suivant;
	}
	return indice;
}

int nombreSymbolGlobalFichierElf(Noeud *noeud, int indice){
	if (noeud == NULL){
		return indice;
	} else {
		//On augmente la taille de la section car on a ajouté un symbole
		indice = nombreSymbolGlobalFichierElf(noeud->noeudGauche, indice + 1);
		indice = nombreSymbolGlobalFichierElf(noeud->noeudDroit, indice);
	}
	return indice;
}

int tailleSectionTableSymbole(struct Liste *listeLocal, struct Noeud *ArbreVariableGlobal) {
	int nb_symbole = 0;
	NoeudLocal *noeud = listeLocal->premier;
	while (noeud != NULL){
		nb_symbole++;
		noeud = noeud->suivant;
	}
	printf("nombre symbole local %d\n", nb_symbole);
	nb_symbole = nombreSymbolGlobalFichierElf(ArbreVariableGlobal, nb_symbole);
	return nb_symbole;
}

void print_symbol_table32(FILE* elfFile, Elf32_Ehdr header, Elf32_Shdr section,
		uint32_t indice, struct Noeud *ArbreVariableGlobal, struct Liste *listeLocal) {
	Elf32_Sym table_symbole;
	uint32_t i, nombre_symbol;
	//accéde à la table des symboles
	fseek(elfFile, header.e_shoff + indice * sizeof(section), SEEK_SET);

	litEtInverse_Section(elfFile, header, &section);
	// calcul du nombre de symbole
	nombre_symbol = section.sh_size / section.sh_entsize;
  	fseek(elfFile, section.sh_offset, SEEK_SET);

	//on affiche les informations du symbole
	for(i=0; i< nombre_symbol; i++) {
		litEtInverse_TalbeSymbole(elfFile, header, &table_symbole);
		switch(ELF32_ST_BIND(table_symbole.st_info)) {
			case 0:
				insererEnTeteListe(listeLocal, table_symbole);
				break;
			case 1:
				verificationSymboleGlobal(table_symbole, ArbreVariableGlobal);
				break;
			case 2:
				printf("Cas jamais trouvé, type WEAK (etape2-7.c)\n");
				break;
			case 3:
				printf("Cas jamais trouvé, type NUM (etape2-7.c)\n");
				break;
			default:
				printf(" Cas non géré fichier enteteEtape4.c fonction print_symbol_table32 switch ELF32_ST_BIND ");
				break;
		}
	}
}

int fusion_section_2_7(FILE* elfFile1, FILE* elfFile2, FILE* outputFile, Elf32_Ehdr* headerOutput) {
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

	litEtInverse_Header(elfFile1, &header1);
	litEtInverse_Header(elfFile2, &header2);

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
		int j=-1;
		fseek(elfFile1, header1.e_shoff + i * header1.e_shentsize, SEEK_SET);
		litEtInverse_Section(elfFile1, header1, &section1);
		nom_section1 = get_section_name(elfFile1, header1, section1);

		if (header1.e_shstrndx == i) {
			taille_shtstrtab = section1.sh_size;
		}

		//Parcours des sections du fichier 2
		//Tant que les sections n'ont pas le même nom et pas le même type
		do {
			j++;
			fseek(elfFile2, header2.e_shoff + j * header2.e_shentsize, SEEK_SET);
			litEtInverse_Section(elfFile2, header2, &section2);
			nom_section2 = get_section_name(elfFile2, header2, section2);
		} while((strcmp(nom_section1, nom_section2) || section1.sh_type != section2.sh_type) && (j < header2.e_shnum));

		//Si une section de même nom et de même type a été trouvée
		if (j < header2.e_shnum) {
			if(section1.sh_type == SHT_SYMTAB) {
				printf("Avant ici\n");
				struct Noeud *ArbreVariableGlobal = malloc(sizeof(struct Noeud));
				struct Liste *listeLocal = malloc(sizeof(struct Liste));
				listeLocal->premier = NULL;

				print_symbol_table32(elfFile1, header1, section1, i, ArbreVariableGlobal, listeLocal);
				print_symbol_table32(elfFile2, header2, section2, j, ArbreVariableGlobal, listeLocal);
				printf("111111111111111111111111\n");

				sections_deja_fusionnees[j]=1;
				renumerotation_section2[j]=i;
				sectionOut.sh_name = section1.sh_name;
				sectionOut.sh_type = section1.sh_type;
				sectionOut.sh_flags = section1.sh_flags;
				sectionOut.sh_addr = section1.sh_addr;
				sectionOut.sh_offset = offset_actuel;
				sectionOut.sh_link = get_sh_link(sectionOut.sh_type, section1.sh_link,symtab_index);
				sectionOut.sh_entsize = section1.sh_entsize;
				sectionOut.sh_addralign = section1.sh_addralign;
				printf("33333333333333333333\n");
				int nombre_symbol =  tailleSectionTableSymbole(listeLocal, ArbreVariableGlobal);
				printf("3.1111111111\n");
				sectionOut.sh_info = nombre_symbol;
				sectionOut.sh_size = nombre_symbol * sizeof(Elf32_Sym);
				//On print la section

				printf("44444444444444444444\n");
				fwrite(&sectionOut,sizeof(sectionOut),1,outputFile);
				//On sauvegarde la position du curseur
				curseur = ftell(outputFile);
				fseek(outputFile, sectionOut.sh_offset, SEEK_SET);
				printf("55555555555555555555\n");
				int indice = 0;
				indice = ecritureSymbolLocalFichierElf(outputFile, &sectionOut, listeLocal, indice);
				indice = ecritureSymbolGlobalFichierElf(outputFile, &sectionOut, ArbreVariableGlobal->noeudGauche, indice);
				indice = ecritureSymbolGlobalFichierElf(outputFile, &sectionOut, ArbreVariableGlobal->noeudDroit, indice);
				offset_actuel += sectionOut.sh_size;
				fseek(outputFile, curseur, SEEK_SET); //On revient à la position initiale
				printf("99999999999999999999\n");
			} else {
				sections_deja_fusionnees[j] = 1;
				renumerotation_section2[j] = i;
				sectionOut.sh_name = section1.sh_name;
				sectionOut.sh_type = section1.sh_type;
				sectionOut.sh_flags = section1.sh_flags;
				sectionOut.sh_addr = section1.sh_addr;
				sectionOut.sh_offset = offset_actuel;
				sectionOut.sh_size = section1.sh_size + section2.sh_size;
				offset_actuel += sectionOut.sh_size;
				sectionOut.sh_link = get_sh_link(sectionOut.sh_type,section1.sh_link,symtab_index);
				sectionOut.sh_info = section1.sh_info;
				sectionOut.sh_entsize = section1.sh_entsize;
				sectionOut.sh_addralign = section1.sh_addralign;

				//On print la section
				fwrite(&sectionOut, sizeof(sectionOut), 1, outputFile);
				//On sauvegarde la position du curseur
				curseur = ftell(outputFile);
				//On va à la position de la section et on print les 2 sections à la suite
				fseek(outputFile, sectionOut.sh_offset, SEEK_SET);
				fseek(elfFile1, section1.sh_offset, SEEK_SET);
				file_copy(elfFile1, outputFile, section1.sh_size);
				fseek(elfFile2, section2.sh_offset, SEEK_SET);
				file_copy(elfFile2, outputFile, section2.sh_size);
				//On revient à la position initiale
				fseek(outputFile, curseur, SEEK_SET);
			}
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
			sectionOut.sh_link = get_sh_link(sectionOut.sh_type, section1.sh_link, symtab_index);
			sectionOut.sh_info = get_sh_info(sectionOut.sh_type, section2.sh_info, renumerotation_section2);
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
