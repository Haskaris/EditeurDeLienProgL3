#ifndef __ETAPE1_2_H__
#define __ETAPE1_2_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>
#include "../librairie/lib-bigEndian.h"
#include "../librairie/utils.h"

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
 *	- entier pour savoir si on est en bigENDIAN ou en litleENDIAN (bigEndian)
*/
void affichage_Table_Sections(FILE *elfFile, Elf32_Ehdr header, int bigEndian);
#endif
