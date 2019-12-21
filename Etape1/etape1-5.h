#ifndef __ETAPE1_5_H__
#define __ETAPE1_5_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>
#include "../librairie/lib-bigEndian.h"

/* Lit le nom des sections de la table des entêtes
 * et le renvoit
 * arguments :
 *	- le fichier ELF (elfFile) ouvert
 *	- l'entête du fichier ELF (header)
 *	- la table des sections du fichier ELF (sh_table)
 *	- entier pour savoir si on est en bigENDIAN ou en litleENDIAN (bigEndian)
*/
void get_section_name(FILE* elfFile, Elf32_Ehdr header, Elf32_Shdr section, char* name, int bigEndian);

/* Affiche le type de repositionnement à appliquer
 * arguments :
 *	- ELF32_R_TYPE(byteshift32(rel.r_info, bigEndian))
*/
void afficher_relocation_type(int type);

/* Affiche la table des réimplantation
 * arguments :
 *	- le fichier ELF (elfFile) ouvert
 *	- l'entête du fichier ELF (header)
 *	- entier pour savoir si on est en bigENDIAN ou en litleENDIAN (bigEndian)
*/
void affichage_Table_Reimplantation(FILE *elfFile, Elf32_Ehdr header, int bigEndian);
#endif
