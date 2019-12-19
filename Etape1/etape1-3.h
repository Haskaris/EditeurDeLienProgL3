#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>
#include "../librairie/lib-bigEndian.c"

/* Récupère le nom de la section
 * arguments :
 *	- le fichier ELF (elfFile) ouvert
 *	- entête du fichier ELF (header)
 *	- la table des entêtes du fichier ELF (section)
 *	- nom de la table (name)
 *	- entier pour savoir si on est en bigENDIAN ou en litleENDIAN (bigEndian)
*/
void get_section_name(FILE* elfFile, Elf32_Ehdr header, Elf32_Shdr section, char* name, int bigEndian);
