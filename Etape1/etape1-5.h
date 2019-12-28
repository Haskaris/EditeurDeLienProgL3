#ifndef __ETAPE1_5_H__
#define __ETAPE1_5_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>
#include "../librairie/utils.h"
#include "../librairie/lib-bigEndian.h"
#include "../librairie/macro_section.h"
#include "../librairie/macro_symbole.h"
#include "../librairie/macro_header.h"
#include "../librairie/macro_repositionnement.h"

/* Lit le nom des sections de la table des entêtes
 * et le renvoit
 * arguments :
 *	- le fichier ELF (elfFile) ouvert
 *	- l'entête du fichier ELF (header)
 *	- la table des sections du fichier ELF (sh_table)
*/
//void get_section_name(FILE* elfFile, Elf32_Ehdr header, Elf32_Shdr section, char* name);

/* Affiche le type de repositionnement à appliquer
 * arguments :
 *	- ELF32_R_TYPE(byteshift32(rel.r_info, bigEndian))
*/
void afficher_relocation_type(int type);

/* Affiche la table des réimplantation
 * arguments :
 *	- le fichier ELF (elfFile) ouvert
 *	- l'entête du fichier ELF (header)
*/
void affichage_Table_Reimplantation(FILE *elfFile, Elf32_Ehdr header);
#endif /* __ETAPE1_5_H__ */
