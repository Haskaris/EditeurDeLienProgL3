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

