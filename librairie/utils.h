#ifndef __UTILS_H__
#define __UTILS_H__
#include <elf.h>
#include "../librairie/macro_header.h"
#include "../librairie/macro_symbole.h"
#include "../librairie/macro_section.h"

/* Récupére le nom des sections de la table des sections
 * et le renvoit
 * arguments :
 *	- le fichier ELF (elfFile) ouvert
 *	- entête du fichier ELF (header)
 *	- la table des sections du fichier ELF (section)
*/
char* get_section_name(FILE* elfFile, Elf32_Ehdr header, Elf32_Shdr section);

/* Inverse l'ordre des octets de n pour 16b
 * arguments :
 *	- l'entier à modifier (n)
*/
uint16_t reverseByte16(uint16_t n);

/*Inverse l'ordre des octets de n pour 32b
 * arguments :
 *	- l'entier à modifier (n)
*/
uint32_t reverseByte32(uint32_t n);

/*Inverse l'ordre des octets de n pour 64b
 * arguments :
 *	- l'entier à modifier (n)
*/
uint64_t reverseByte64(uint64_t n);
#endif
