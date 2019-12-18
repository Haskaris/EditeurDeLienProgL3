#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>

/* Retourne vrai si le fichier est au format BIG ENDIAN
 * arguments :
 *	- l'entête du fichier ELF (h)
*/
int isbigendian(Elf32_Ehdr h);


/* Inverse l'ordre des octets de n pour 16b
 * arguments :
 *	- l'entier à modifier (n)
 *	- entier pour savoir si on est en bigENDIAN ou en litleENDIAN (bigEndian)
*/
uint16_t byteshift16(uint16_t n, int bigEndian);


/*Inverse l'ordre des octets de n pour 32b
 * arguments :
 *	- l'entier à modifier (n)
 *	- entier pour savoir si on est en bigENDIAN ou en litleENDIAN (bigEndian)
*/
uint32_t byteshift32(uint32_t n, int bigEndian);


/*Inverse l'ordre des octets de n pour 64b
 * arguments :
 *	- l'entier à modifier (n)
 *	- entier pour savoir si on est en bigENDIAN ou en litleENDIAN (bigEndian)
*/
uint64_t byteshift64(uint64_t n, int bigEndian);

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
 *	- entête du fichier ELF (eh)
 *	- entier pour savoir si on est en bigENDIAN ou en litleENDIAN (bigEndian)
*/
void print_table_symbol(FILE* elfFile, Elf32_Ehdr eh, int bigEndian);
