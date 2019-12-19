#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>
#include "../librairie/lib-bigEndian.c"

/* Affiche la classe du fichier ELF, 32 ou 64 bits
 * arguments : 
 *	- l'entête du fichier ELF (h)
*/
void classeArchitecture(Elf32_Ehdr h);

/* Affiche l'encodage du fichier ELF, little ou big ENDIAN
 * arguments : 
 *	- l'entête du fichier ELF (h)
*/
void encodage(Elf32_Ehdr h);

/* Affiche le numéro de version de la spécification du format ELF
 * arguments : 
 *	- l'entête du fichier ELF (h)
 *	- précise si on affiche l'adresse ou non (addressMode)
*/
void fileVersion(Elf32_Ehdr h, int addressMode);


/* Affiche le système d'exploitation et l'interface 
 * binaire des applications (ABI)
 * arguments : 
 *	- l'entête du fichier ELF (h)
*/
void osAbi(Elf32_Ehdr h);

/* Affiche le type du fichier objet
 * arguments : 
 *	- l'entête du fichier ELF (h)
 *	- entier pour savoir si on est en bigENDIAN ou en litleENDIAN (bigEndian)
*/
void fileType(Elf32_Ehdr h, int bigEndian);

/* Affiche l'architecture nécessaire à un fichier individuel
 * arguments : 
 *	- l'entête du fichier ELF (h)
 *	- entier pour savoir si on est en bigENDIAN ou en litleENDIAN (bigEndian)
*/
void machine(Elf32_Ehdr h, int bigEndian);
