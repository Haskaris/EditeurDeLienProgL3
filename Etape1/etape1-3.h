#ifndef __ETAPE1_3_H__
#define __ETAPE1_3_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>
#include "../librairie/lib-bigEndian.h"

/* Récupère le nom de la section
 * arguments :
 *	- le fichier ELF (elfFile) ouvert
 *	- entête du fichier ELF (header)
 *	- la table des entêtes du fichier ELF (section)
 *	- nom de la table (name)
 *	- entier pour savoir si on est en bigENDIAN ou en litleENDIAN (bigEndian)
*/
void get_section_name_3(FILE* elfFile, Elf32_Ehdr header, Elf32_Shdr section, char* name, int bigEndian);

/* Affiche le contenu d'une section
 * arguments :
 *	- le fichier ELF (elfFile) ouvert
 *	- entête du fichier ELF (header)
 *	- entier pour savoir si on est en bigENDIAN ou en litleENDIAN (bigEndian)
 *	- entier qui correspond au numéro de la section que l'on veut afficher (numSection)
*/
void affichage_Contenu_Section(FILE *elfFile, Elf32_Ehdr header, int bigEndian, int numSection);
#endif
