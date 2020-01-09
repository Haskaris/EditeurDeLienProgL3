#ifndef __ETAPE2_6_H__
#define __ETAPE2_6_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>
#include "../librairie/utils.h"

/*
 * renvoie la bonne valeur de sh_link en fonction du type de la section
 */
uint32_t get_sh_link(uint32_t type, uint32_t section1_link, int symtab_index);

/*
 * renvoie l'indice de la section de la table des symboles
 */
int get_symtab_index(FILE* fichier, Elf32_Ehdr header);

/*
 * renvoie la bonne valeur de sh_info en fonction du type de la section
 */
uint32_t get_sh_info(uint32_t type, uint32_t info, int* renumerotation_section2);

/*
 * Fusionne les headers de deux fichiers
 * Paramètres:
 * - Fichier à fusionner 1 (elfFile1)
 * - Fichier à fusionner 2 (elfFile2)
 * - Fichier fusionné (outputFile)
 * - Header fusionné (headerOutput)
 */
int fusion_header(FILE* elfFile1, FILE* elfFile2, FILE* outputFile, Elf32_Ehdr* headerOutput);

/*
 * Fusionne les sections de deux fichiers
 * Paramètres:
 * - Fichier à fusionner 1 (elfFile1)
 * - Fichier à fusionner 2 (elfFile2)
 * - Fichier fusionné (outputFile)
 * - Header fusionné au préalable (headerOutput)
 */
int fusion_section(FILE* elfFile1, FILE* elfFile2, FILE* outputFile, Elf32_Ehdr* headerOutput);

int fusion_2_6(FILE * elfFile1, FILE * elfFile2, FILE * outputFile);

#endif  /* __ETAPE2_6_H__ */
