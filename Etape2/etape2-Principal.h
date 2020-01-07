#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>
#include "../librairie/lib-bigEndian.c"
#include "etape2-7.c"
#include "../librairie/macro_section.h"
#include "../librairie/macro_symbole.h"
#include "../librairie/macro_header.h"
#include "../librairie/macro_repositionnement.h"

/* Test si les fichiers donnés en 
 * arguments sont des fichiers ELF
 * retourne 1 si vrai, 0 sinon.
 * arguments :
 *	- entête du fichier ELF (header)
 *	- entête du fichier ELF 2 (header1)
*/
int test_Elf_File(Elf32_Ehdr header, Elf32_Ehdr header1);

/* Contient la fonction main qui se chargera d'appeler les 
 * fonctions voulues
*/
