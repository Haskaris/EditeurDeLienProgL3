#ifndef __ETAPE1_2_H__
#define __ETAPE1_2_H__
//TODO: nettoyer les includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>
#include "../librairie/utils.h"
#include "../librairie/lib-bigEndian.h"
#include "../librairie/macro_header.h"
#include "../librairie/macro_symbole.h"
#include "../librairie/macro_section.h"

/* Affiche l'entête pour l'affichage des entêtes
*/
void headtext();

/* Affichage du bas de l'entête
*/
void foottext();

/* Affiche l'indentation nécessaire
 * arguments :
 *	- ligne courante (str)
 *	- indentation nécessaire (indent)
*/
void align(char* str, int indent);

/* Affiche le drapeau contenant l'attribut
 * arguments :
 *	- drapeau courant (flag)
*/
void flagsToString(uint32_t flag);

/* Affiche le contenu et la sémantique de la section
 * arguments :
 *	- le type de la section (type)
*/
void typeToString(uint32_t type);

/* Affiche la table des sections et les détails relatif
 * à chaque section
 * arguments :
 *	- le fichier ELF (elfFile) ouvert
 *	- entête du fichier ELF (header)
*/
void affichage_Table_Sections(FILE *elfFile, Elf32_Ehdr header);
#endif  /* __ETAPE1_2_H__ */
