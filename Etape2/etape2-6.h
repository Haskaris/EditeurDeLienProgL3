#ifndef __ETAPE2_6_H__
#define __ETAPE2_6_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>
#include "../librairie/utils.h"

uint64_t set_sh_entsize(uint32_t type);

uint32_t set_sh_link(uint32_t type, uint32_t section1_link, int symtab_index);

int get_symtab_index(FILE* fichier, Elf32_Ehdr header);

uint32_t set_sh_info(uint32_t type, uint32_t info, int* renumerotation_section2);

int fusion_header(FILE* elfFile1, FILE* elfFile2, FILE* outputFile, Elf32_Ehdr* headerOutput);

int fusion_section(FILE* elfFile1, FILE* elfFile2, FILE* outputFile, Elf32_Ehdr* headerOutput);

int fusion_2_6(FILE * elfFile1, FILE * elfFile2, FILE * outputFile);

#endif  /* __ETAPE2_6_H__ */
