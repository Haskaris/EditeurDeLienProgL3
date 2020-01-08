#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>
#include "etape2-7.h"
#include "etape2-6.h"

void fusion_symbole(FILE * elfFile1, FILE * elfFile2, FILE * outputFile){

	Elf32_Ehdr headerOutput;

	fusion_header(elfFile1, elfFile2, outputFile, &headerOutput);

	//fusion_section_2_7(elfFile1, elfFile2, outputFile, &headerOutput);
}

void fusion_section_2_7(FILE* elfFile1, FILE* elfFile2, FILE* outputFile, Elf32_Ehdr* headerOutput){

}
