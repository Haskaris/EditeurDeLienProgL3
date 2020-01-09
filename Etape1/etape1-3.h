#ifndef __ETAPE1_3_H__
#define __ETAPE1_3_H__
//TODO: nettoyer les includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>
#include "../librairie/utils.h"
#include "../librairie/lib-bigEndian.h"

/* Affiche le contenu d'une section
 * arguments :
 *	- le fichier ELF (elfFile) ouvert
 *	- entête du fichier ELF (header)
 *	- entier qui correspond au numéro de la section que l'on veut afficher (numSection)
*/
void affichage_Contenu_Section(FILE *elfFile, Elf32_Ehdr header, int numSection);
#endif  /* __ETAPE1_3_H__ */
