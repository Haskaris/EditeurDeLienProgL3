#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>
#include "../librairie/lib-bigEndian.c"

/* Affiche l'entête pour l'affichage des entêtes
*/
void headtext();

/* Affichage du bas de l'entête
*/
void foottext();

/* Récupére le nom des sections de la table des sections
 * arguments : 
 *	- le fichier ELF (elfFile) ouvert
 *	- entête du fichier ELF (header)
 *	- la table des sections du fichier ELF (section)
 *	- tableau des noms (name)
 *	- entier pour savoir si on est en bigENDIAN ou en litleENDIAN (bigEndian)
*/
void get_section_name(FILE* elfFile, Elf32_Ehdr header, Elf32_Shdr section, char* name, int bigEndian);

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
