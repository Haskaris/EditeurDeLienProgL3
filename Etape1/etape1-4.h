#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>
#include "../librairie/lib-bigEndian.h"

/* Lit le nom des sections de la table des entêtes
 * et le renvoit
 * arguments :
 *	- le fichier ELF (elfFile) ouvert
 *	- la table des entêtes du fichier ELF (sh_table)
 *	- entier pour savoir si on est en bigENDIAN ou en litleENDIAN (bigEndian)
*/
char* get_section_names(FILE *elfFile, Elf32_Shdr sh_table, int bigEndian);

/* Créer la table des symboles, la lit dans le fichier ELF
 * puis écrit son contenu dans la console
 * arguments :
 * 	- le fichier ELF (elfFile) ouvert
 *	- entête du fichier ELF (eh)
 *	- table des entêtes du fichier ELF (sh_table)
 *	- l'indice de la bonne section dans la table des entêtes (indice)
 *	- entier pour savoir si on est en bigENDIAN ou en litleENDIAN (bigEndian)
*/
void print_symbol_table32(FILE* elfFile, Elf32_Ehdr eh, Elf32_Shdr sh_table, uint32_t indice, int bigEndian);

/* Sert à accéder à la bonne section dans la table des entêtes
 * puis appel la fonction print_symbol_table64 pour écrire son contenu
 * arguments : 
 *	- le fichier ELF (elfFile) ouvert
 *	- entête du fichier ELF (header)
 *	- entier pour savoir si on est en bigENDIAN ou en litleENDIAN (bigEndian)
*/
void affichage_Table_Des_Symbole(FILE *elfFile, Elf32_Ehdr header, int bigEndian);
