#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>
#include "etape2-7.h"
#include "etape2-8.h"
void fusion_symbole(FILE * elfFile1, FILE * elfFile2, FILE * outputFile) {
	return;
}

void insererEnTeteListe(Liste *listeLocal, Elf32_Sym table_symbole){
	NoeudLocal *nouveauNoeud = malloc(sizeof(NoeudLocal));
	nouveauNoeud->suivant = listeLocal->premier;
	nouveauNoeud->symboleCourant = table_symbole;
	listeLocal->premier = nouveauNoeud;
}

void afficheSymbole(Elf32_Sym table_symbole){
		printf("	%08x ", table_symbole.VALEUR_SYMBOLE);
		printf(" %u ", table_symbole.TAILLE_SYMBOLE);
		switch (ELF32_ST_TYPE(table_symbole.TYPE_SYMBOLE_ET_ATTRIBUT_LIAISON)) {
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
		switch(ELF32_ST_BIND(table_symbole.TYPE_SYMBOLE_ET_ATTRIBUT_LIAISON)) {
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
		switch(ELF32_ST_VISIBILITY(table_symbole.VISIBILITE_SYMBOLE)) {
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
		printf(" %x	", table_symbole.INDICE_TABLE_SECTION);
		printf(" %02x\n", table_symbole.NOM_SYMBOLE);
}

void ArbreVariableGlobalInitialisation(struct Noeud *ArbreVariableGlobal){
	ArbreVariableGlobal = NULL;//malloc(sizeof(ArbreVariableGlobal));
}

void ArbreVariableLocalInitialisation(struct NoeudLocal *ArbreVariableLocal){
	ArbreVariableLocal = NULL;//malloc(sizeof(ArbreVariableLocal));
}

void verificationSymboleGlobal(Elf32_Sym symbole, struct Noeud *ArbreVariableGlobal){
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


int ecritureSymbolGlobalFichierElf(FILE* elfFileDest, Elf32_Shdr *section, Noeud *noeud, int indice){
	if (noeud == NULL){
		return indice;
	} else {
		noeud->symboleCourant.st_shndx = indice;
		afficheSymbole(noeud->symboleCourant);
		fwrite(&(noeud->symboleCourant), sizeof(noeud->symboleCourant), 1, elfFileDest);
		//On augmente la taille de la section car on a ajouté un symbole
		//section->TAILLE_SECTION += sizeof(noeud->symboleCourant);
		indice = ecritureSymbolGlobalFichierElf(elfFileDest, section, noeud->noeudGauche, indice + 1);
		indice = ecritureSymbolGlobalFichierElf(elfFileDest, section, noeud->noeudDroit, indice + 1);
	}
	return indice;
}


/*
void file_copy(FILE* file1, FILE* file2, size_t size){
	char* ptr=malloc(size);
	fread(ptr,size,1,file1);
	fwrite(ptr,size,1,file2);
	free(ptr);
}

*/

/* PROBLEME 
 * Variable Global MAIN présente dans toutes 
 * les tables de symbôle, comment faire ???
*/
int ecritureSymbolLocalFichierElf(FILE* elfFileDest, Elf32_Shdr *section, struct Liste *listeLocal, int indice){
	NoeudLocal *noeud = listeLocal->premier;
	//int i = 0;
	indice--;
	while (noeud != NULL){
		//printf("i : %d\n", i);
		//i++;
		indice++;
		noeud->symboleCourant.st_shndx=indice;
		afficheSymbole(noeud->symboleCourant);
		fwrite(&(noeud->symboleCourant), sizeof(noeud->symboleCourant), 1, elfFileDest);
		//On augmente la taille de la section car on a ajouté un symbole
		//section->TAILLE_SECTION += sizeof(noeud->symboleCourant);
		noeud = noeud->suivant;
	}
	return indice;
}

int nombreSymbolGlobalFichierElf(Noeud *noeud, int indice){
	if (noeud == NULL){
		return indice;
	} else {
		//On augmente la taille de la section car on a ajouté un symbole
		//section->TAILLE_SECTION += sizeof(noeud->symboleCourant);
		indice = nombreSymbolGlobalFichierElf(noeud->noeudGauche, indice + 1);
		indice = nombreSymbolGlobalFichierElf(noeud->noeudDroit, indice);
	}
	return indice;
}
int tailleSectionTableSymbole(struct Liste *listeLocal, struct Noeud *ArbreVariableGlobal){
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




void print_symbol_table32(FILE* elfFile, Elf32_Ehdr header, Elf32_Shdr section, uint32_t indice, struct Noeud *ArbreVariableGlobal, struct Liste *listeLocal){	
	Elf32_Sym table_symbole;
	uint32_t i, nombre_symbol;
	//accéde à la table des symboles
	/*fseek(elfFile, header.DECALAGE_TABLE_ENTETE_SECTIONS + indice * sizeof(section), SEEK_SET);
	fread(&section, 1, sizeof(section), elfFile);
  	//printf("Name : %s\n",get_section_names(elfFile, sh_table, bigEndian) + sh_table.sh_name);
	if (isbigendian(header)){
		inversion_Sections(&section);
    	}*/
	//accéde à la table des symboles
	fseek(elfFile, header.DECALAGE_TABLE_ENTETE_SECTIONS + indice * sizeof(section), SEEK_SET);

	litEtInverse_Section(elfFile, header, &section);
	// calcul du nombre de symbole   	
	nombre_symbol = section.TAILLE_SECTION / section.TAILLE_TABLE_ENTREE_FIXE;
  	fseek(elfFile, section.DECALAGE_DEBUT_FICHIER, SEEK_SET);

	//printf("La table de symboles << .symtab >> contient %d entrées :\n", nombre_symbol);
	//on affiche les informations du symbole
	for(i=0; i< nombre_symbol; i++) {
		litEtInverse_TalbeSymbole(elfFile, header, &table_symbole);
		//printf("i lecture %d\n", i);
		switch(ELF32_ST_BIND(table_symbole.TYPE_SYMBOLE_ET_ATTRIBUT_LIAISON))
    		{
       	 		case 0: //printf(" LOCAL ");
				//printf("\ni lecture %d\n",i);
				/*if (listeLocal->premier == NULL){
					NoeudLocal *nouveauNoeud = malloc(sizeof(NoeudLocal));
					nouveauNoeud->suivant = NULL;
					nouveauNoeud->symboleCourant = table_symbole;
					listeLocal->premier = nouveauNoeud;
				} else if (listeLocal->premier->suivant == NULL){
					NoeudLocal *nouveauNoeud = malloc(sizeof(NoeudLocal));
					nouveauNoeud->suivant = NULL;
					nouveauNoeud->symboleCourant = table_symbole;
					listeLocal->premier->suivant = nouveauNoeud;
				} else {
					insereNoeudDansArbreLocal(listeLocal->premier, table_symbole);
				}*/
				insererEnTeteListe(listeLocal, table_symbole);
	    			break;
        		case 1: //printf(" GLOBAL ");
				verificationSymboleGlobal(table_symbole, ArbreVariableGlobal);
            			break;
        		case 2: printf("Cas jamais trouvé, type WEAK (etape2-7.c)\n");
            			break;
        		case 3: printf("Cas jamais trouvé, type NUM (etape2-7.c)\n");
            			break;
        		default: printf(" Cas non géré fichier enteteEtape4.c fonction print_symbol_table32 switch ELF32_ST_BIND ");
        			break;
    		}
		/*switch(ELF32_ST_BIND(table_symbole.TYPE_SYMBOLE_ET_ATTRIBUT_LIAISON)) {
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
		}*/
	}
}




/*
void initTableSymbole(FILE *elfFile1, FILE *elfFile2, FILE *outputFile, Elf32_Ehdr header1, Elf32_Ehdr header2, Elf32_Shdr section1, Elf32_Shdr section2, Elf32_Shdr *sectionOut, int indice1, int indice2, int *offset_actuel, int *curseur){
	ArbreVariableGlobalInitialisation();
	ArbreVariableLocal();
	print_symbol_table32(elfFile1, header1, section1, indice1);
	print_symbol_table32(elfFile2, header2, section2, indice2);

	sections_deja_fusionnees[j]=1;
	renumerotation_section2[j]=i;
	sectionOut->sh_name=section1.sh_name;
	sectionOut->sh_type=section1.sh_type;
	sectionOut->sh_flags=section1.sh_flags;
	sectionOut->sh_addr=section1.sh_addr;
	sectionOut->sh_offset=*offset_actuel;
	sectionOut->sh_size = 0;
	sectionOut->sh_link=set_sh_link(sectionOut->sh_type,section1.sh_link,symtab_index);
	sectionOut->sh_info=section1.sh_info;
	sectionOut->sh_entsize=set_sh_entsize(sectionOut.sh_type);
	//On print la section
	fwrite(&sectionOut,sizeof(sectionOut),1,outputFile);
	*curseur=ftell(outputFile);//On sauvegarde la position du curseur
	fseek(outputFile,sectionOut->sh_offset,SEEK_SET);//On va à la position de la section et on print les 2 sections à la suite
	ecritureSymbolLocalFichierElf(outputFile, sectionOut);
	ecritureSymbolGlobalFichierElf(outputFile, sectionOut, ArbreVariableGlobal);
	*offset_actuel+=sectionOut->sh_size;

	fseek(outputFile,curseur,SEEK_SET); //On revient à la position initiale	

}
*/
