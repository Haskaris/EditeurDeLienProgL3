#ifndef __ETAPE1_1_H__
#define __ETAPE1_1_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>
#include "../librairie/lib-bigEndian.h"

/* Affiche la classe du fichier ELF, 32 ou 64 bits
 * arguments :
 *	- l'entête du fichier ELF (header)
*/
void classeArchitecture(Elf32_Ehdr header);

/* Affiche l'encodage du fichier ELF, little ou big ENDIAN
 * arguments :
 *	- l'entête du fichier ELF (header)
*/
void encodage(Elf32_Ehdr header);

/* Affiche le numéro de version de la spécification du format ELF
 * arguments :
 *	- l'entête du fichier ELF (header)
 *	- précise si on affiche l'adresse ou non (addressMode)
*/
void fileVersion(Elf32_Ehdr header, int addressMode);


/* Affiche le système d'exploitation et l'interface
 * binaire des applications (ABI)
 * arguments :
 *	- l'entête du fichier ELF (header)
*/
void osAbi(Elf32_Ehdr header);

/* Affiche le type du fichier objet
 * arguments :
 *	- l'entête du fichier ELF (header)
*/
void fileType(Elf32_Ehdr header);

/* Affiche l'architecture nécessaire à un fichier individuel
 * arguments :
 *	- l'entête du fichier ELF (header)
*/
void machine(Elf32_Ehdr header);

/* Affiche l'entête d'un fichier ELF
 * arguments :
 *	- l'entête du fichier ELF (header)
*/
void affichage_Entete_Fichier_ELF(Elf32_Ehdr header);

#endif
