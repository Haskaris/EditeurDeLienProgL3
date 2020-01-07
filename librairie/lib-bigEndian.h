#ifndef __LIB_BIG_ENDIAN_H__
#define __LIB_BIG_ENDIAN_H__
#include <elf.h>

/* Retourne vrai si le fichier est au format BIG ENDIAN
 * arguments :
 *	- l'entête du fichier ELF (h)
*/
int isbigendian(Elf32_Ehdr h);

/* Inverse toutes les valeurs du header
 * arguments :
 *	- l'entête du fichier ELF (*header)
*/
void inversion_Header(Elf32_Ehdr *header);

/* Inverse toutes les valeurs des sections
 * arguments :
 *	- l'entête de section du fichier ELF (*section)
*/
void inversion_Sections(Elf32_Shdr *section);

/* Inverse la table des symboles
 * arguments :
 *	- table des symboles du fichier ELF (*sym_tbl)
*/
void insersion_Table_Symbole(Elf32_Sym *sym_tbl);

/* Inverse la structure de repositionnement nécessitant un additif
 * arguments :
 *	- structure de repositionnement du ficheir ELF (*rela)
*/
void inversion_Relation_Additif(Elf32_Rela *rela);

/* Inverse la structure de repositionnement ne nécessitant pas d'additif
 * arguments :
 *	- structure de repositionnement du ficheir ELF (*rela)
*/
void inversion_Relation_Sans_Additif(Elf32_Rel *rel);
#endif
