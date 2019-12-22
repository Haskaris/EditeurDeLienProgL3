#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>

/* Récupére le nom des sections de la table des sections
 * et le renvoit
 * arguments :
 *	- le fichier ELF (elfFile) ouvert
 *	- entête du fichier ELF (header)
 *	- la table des sections du fichier ELF (section)
 *	- entier pour savoir si on est en bigENDIAN ou en litleENDIAN (bigEndian)
*/
char* get_section_name(FILE* elfFile, Elf32_Ehdr header, Elf32_Shdr section, int bigEndian);
