#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>

/* Lit la partie du descripteur de fichier fd concerné 
 * et renvoie un buffer avec le contenue du fichier
 * arguments :
 *	- le descripteur du fichier ELF fd
 *	- la table des entêtes du fichier ELF
*/
char * read_section64(int32_t fd, Elf64_Shdr sh);


/* Créer la table des symboles, la lit dans le fichier fd
 * puis écrit son contenu
 * arguments :
 * 	- le descripteur du fichier ELF fd
 *	- entête du fichier ELF
 *	- table des entêtes du fichier ELF
 *	- le symbole courant de la table des entêtes (symbol_table)
*/
void print_symbol_table64(int32_t fd, Elf64_Ehdr eh, Elf64_Shdr sh_table[], uint32_t symbol_table);

/* Sert à accéder aux bonnes sections dans la table des entêtes
 * puis appel la fonction print_symbol_table64 pour écrire son contenu
 * arguments : 
 *	- fichier elfFile ouvert
 *	- entête du fichier ELF 
 * 	- table des entêtes du fichier ELF
*/
void print_table_symbol(FILE* elfFile, Elf64_Ehdr eh, Elf64_Shdr sh_table[]);
