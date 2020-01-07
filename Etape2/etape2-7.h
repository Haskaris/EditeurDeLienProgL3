#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>
#include "../librairie/lib-bigEndian.c"
#include "../librairie/macro_section.h"
#include "../librairie/macro_symbole.h"
#include "../librairie/macro_header.h"
#include "../librairie/macro_repositionnement.h"
#include "../librairie/arbreBinaire.c"

/* Ecrit tout les symboles globaux contenu dans l'arbre
 * arguments :
 *	- Le fichier ELF destinations (elfFileDest)
 *	- L'arbre contenant tous les symboles globaux (*noeudCourant) 
*/
void ecritureSymboleGlobaux(FILE *elfFileDest, struct Noeud *noeudCourant);

/* Initialise l'arbre des symboles globaux
*/
void ArbreVariableGlobalInitialisation();

/* Vérifie les conditions sur les symboles globaux, si
 * le symbole n'y est pas il est ajouté.
 * arguments :
 * 	- le symbole global à ajouter (symbole)
*/
void verificationSymboleGlobal(Elf32_Sym symbole);

/* Ecrit le symbole dans le fichier ELF
 * arguments : 
 * 	- le fichier elf destination (elfFileDest)
 *	- le symbole à écrire dans le fichier (symbole)
*/
void ecritureSymbolFichierElf(FILE* elfFileDest, Elf32_Sym symbole);

/* PROBLEME 
 * Variable Global MAIN présente dans toutes 
 * les tables de symbôle, comment faire ???
 * Parcours la table des symboles d'un fichier
 * et donne les variables à écrire dans le fichier destination
 * arguments : 
 * 	- le fichier elf de la table des symbole à copier (elfFile)
 *	- l'entête du fichier ELF à copier (header)
 *	- la table des sections du fichier ELF à copier (section)
 *	- l'indice de la section contenant la table des symboles (indice)
 *	- le fichier ELF destination (elfFileDest)
*/
void print_symbol_table32(FILE* elfFile, Elf32_Ehdr header, Elf32_Shdr section, uint32_t indice, FILE* elfFileDest);

/* Trouve la section, du fichier ELF à copier, contenant
 * la table des symboles 
 * arguments :
 * 	- fichier ELF à copier (elfFile)
 *	- l'entête du fichier ELF à copier (header)
 *	- fichier ELF destination (elfFileDest)
*/
void stockage_table_symbol_init32(FILE *elfFile, Elf32_Ehdr header, FILE* elfFileDest);

/* Ecrit la table des symboles dans le fichier
 * ELF destination
 * arguments :
 * 	- l'entête du premier fichier ELF à copier (header1)
 *	- l'entête du deuxième fichier ELF à copier (header2)
 *	- fichier ELF destination (elfFileDest)
 * 	- premier fichier ELF source (elfFileSource)
 *	- deuxième fichier ELF source (elfFileSource1)
*/
void write_Symbol_Table(Elf32_Ehdr header1, Elf32_Ehdr header2, FILE *elfFileDest, FILE *elfFileSource, FILE *elfFileSource1);
