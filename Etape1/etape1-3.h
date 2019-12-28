#ifndef __ETAPE1_3_H__
#define __ETAPE1_3_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>
#include "../librairie/lib-bigEndian.h"
#include "../librairie/macro_section.h"
#include "../librairie/macro_symbole.h"
#include "../librairie/macro_header.h"

/* Récupère le nom de la section
 * arguments :
 *	- le fichier ELF (elfFile) ouvert
 *	- entête du fichier ELF (header)
 *	- la table des entêtes du fichier ELF (section)
 *	- nom de la table (name)
*/
void get_section_name_3(FILE* elfFile, Elf32_Ehdr header, Elf32_Shdr section, char* name);

/* Affiche le contenu d'une section
 * arguments :
 *	- le fichier ELF (elfFile) ouvert
 *	- entête du fichier ELF (header)
 *	- entier qui correspond au numéro de la section que l'on veut afficher (numSection)
*/
void affichage_Contenu_Section(FILE *elfFile, Elf32_Ehdr header, int numSection);
#endif
