#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>
#include "etape2-7.h"
void fusion_symbole(FILE * elfFile1, FILE * elfFile2, FILE * outputFile) {
	return;
}

/* Arbre contenant toutes les variables 
 * globals de la table des symboles
*/
struct Noeud *ArbreVariableGlobal;

void ecritureSymboleGlobaux(FILE *elfFileDest, struct Noeud *noeudCourant){
	if (!noeudCourant){
		return;
	} else {
		ecritureSymbolFichierElf(elfFileDest, noeudCourant->symboleCourant);
		ecritureSymboleGlobaux(elfFileDest, noeudCourant->noeudGauche);
		ecritureSymboleGlobaux(elfFileDest, noeudCourant->noeudDroit);
	}
}

void ArbreVariableGlobalInitialisation(){
	ArbreVariableGlobal = malloc(sizeof(ArbreVariableGlobal));
}

void verificationSymboleGlobal(Elf32_Sym symbole){
	struct Noeud* symboleGlobal = dansArbreDeRecherche(ArbreVariableGlobal, symbole);
	if (!symboleGlobal){
		/*
			- si un symbole n’apparaı̂t que dans l’une des deux tables en entrée, il devra 
				apparaı̂tre dans la table de sortie
		*/
		insereNoeudDansArbre(ArbreVariableGlobal, symbole);
		//ecritureSymbolFichier(elfFileDest, table_symbole);
	} else {
		//2 symboles de même nom
		/*
			- si un symbole défini dans l’une des deux tables apparaı̂t comme non défini 	
				dans l’autre, seule la définition devra apparaı̂tre
				dans la table de sortie
			- si deux symboles apparaissent comme non définis dans les deux tables en 
				entrée, une seule entrée pour ce symbole devra être
				présente dans la table de sortie
		*/
		if(symboleGlobal->symboleCourant.INDICE_TABLE_SECTION == 0){
			symboleGlobal->symboleCourant = symbole;
		} else if (symboleGlobal->symboleCourant.INDICE_TABLE_SECTION != 0 || symbole.INDICE_TABLE_SECTION != 0) {
			/*
				- si deux symboles de même nom sont définis dans les tables en entrée, 	
				l’édition de liens échoue de manière définitive
			*/
			printf("2 symboles globaux de même nom, echec de l'édition de lien (etape2-7.c)\n");
			exit(0);
		}
	}
}

/* PROBLEME 
 * Variable Global MAIN présente dans toutes 
 * les tables de symbôle, comment faire ???
*/
void ecritureSymbolFichierElf(FILE* elfFileDest, Elf32_Sym symbole){
	int nombre_symbol;
	//voir comment obtenir le header du fichier résultat et la section
	Elf32_Ehdr header = recupererEnteteFichierSource(elfFileDest);
	Elf32_Shdr section = recupererSectionFichierSource(elfFileDest);
		
	fseek(elfFileDest, header.DECALAGE_TABLE_ENTETE_SECTIONS + indice * sizeof(section), SEEK_SET);
	fread(&section, 1, sizeof(section), elfFile);
  	//printf("Name : %s\n",get_section_names(elfFile, sh_table, bigEndian) + sh_table.sh_name);
	if (isbigendian(header)){
		inversion_Sections(&section);
    	}
	// calcul du nombre de symbole   	
	nombre_symbol = section.TAILLE_SECTION / section.TAILLE_TABLE_ENTREE_FIXE;
  	fseek(elfFileDest, section.DECALAGE_DEBUT_FICHIER + sizeof(Elf32_Sym) * nombre_symbol, SEEK_SET);
	
	//verification !!!	
	fwrite(&symbole, 1, sizeof(symbole), elfFileDest);
	//On augmente la taille de la section car on a ajouté un symbole
	section.TAILLE_SECTION += sizeof(symbole);
}

void print_symbol_table32(FILE* elfFile, Elf32_Ehdr header, Elf32_Shdr section, uint32_t indice, FILE* elfFileDest){	
	Elf32_Sym table_symbole;
	uint32_t i, nombre_symbol;
	//accéde à la table des symboles
	fseek(elfFile, header.DECALAGE_TABLE_ENTETE_SECTIONS + indice * sizeof(section), SEEK_SET);
	fread(&section, 1, sizeof(section), elfFile);
  	//printf("Name : %s\n",get_section_names(elfFile, sh_table, bigEndian) + sh_table.sh_name);
	if (isbigendian(header)){
		inversion_Sections(&section);
    	}
	// calcul du nombre de symbole   	
	nombre_symbol = section.TAILLE_SECTION / section.TAILLE_TABLE_ENTREE_FIXE;
  	fseek(elfFile, section.DECALAGE_DEBUT_FICHIER, SEEK_SET);

	printf("La table de symboles << .symtab >> contient %d entrées :\n", nombre_symbol);
	printf("  Num:   Valeur     Tail  Type  Lien  Vis     Ndx Nom\n");
        
	//on affiche les informations du symbole
	for(i=0; i< nombre_symbol; i++) {
		fread(&table_symbole, 1, sizeof(table_symbole), elfFile);
		if (isbigendian(header)){
			insersion_Table_Symbole(&table_symbole);
		}
		switch(ELF32_ST_BIND(table_symbole.TYPE_SYMBOLE_ET_ATTRIBUT_LIAISON))
    		{
       	 		case 0: printf(" LOCAL ");
				ecritureSymbolFichier(elfFileDest, table_symbole);
            			break;
        		case 1: printf(" GLOBAL ");
				verificationSymboleGlobal(table_symbole);
            			break;
        		case 2: printf("Cas jamais trouvé, type WEAK (etape2-7.c)\n");
            			break;
        		case 3: printf("Cas jamais trouvé, type NUM (etape2-7.c)\n");
            			break;
        		default: printf(" Cas non géré fichier enteteEtape4.c fonction print_symbol_table32 switch ELF32_ST_BIND ");
        			break;
    		}
	}
}


void stockage_table_symbol_init32(FILE *elfFile, Elf32_Ehdr header, FILE* elfFileDest) {
	uint32_t i;
	Elf32_Shdr section;
	//parcours la table des entêtes et 
	//cherche la section contenant la table des symboles (SHT_SYMTAB)		
	for(i=0; i < header.NOMBRE_ENTREE_TABLE_SECTIONS; i++) {
		fseek(elfFile, header.DECALAGE_TABLE_ENTETE_SECTIONS + i * sizeof(section), SEEK_SET);
         	fread(&section, 1, sizeof(section), elfFile);
		if (isbigendian(header)){
			inversion_Sections(&section);
		}
		if (section.CONTENU_SEMANTIQUE ==SHT_SYMTAB) {
			print_symbol_table32(elfFile, header, section, i, elfFileDest);
			break;
		}
	}
}

void write_Symbol_Table(Elf32_Ehdr header1, Elf32_Ehdr header2, FILE *elfFileDest, FILE *elfFileSource, FILE *elfFileSource1){
	/*
		Ecrit la table des symboles et la stocke dans le fichier destination
	*/
	// Init ArbreVariableGlobal
	ArbreVariableGlobalInitialisation();
	stockage_table_symbol_init32(elfFileSource, header1, elfFileDest);
	stockage_table_symbol_init32(elfFileSource1, header2, elfFileDest);
	//écriture de tout les symbole globaux dans le fichier
	ecritureSymboleGlobaux(elfFileDest, ArbreVariableGlobal);
	return;
}

