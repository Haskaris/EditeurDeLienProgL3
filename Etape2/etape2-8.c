#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>
#include "etape2-8.h"


void fusion_reimplementation(FILE* elfFile1, FILE* elfFile2,FILE* output){
	FILE * tempFile;
	uint32_t section_name=0;

	Elf32_Ehdr header1,header2,headerOutput;
	Elf32_Shdr section1,section2,sectionOut;

	char nom_section1[255],nom_section2[255];
		elfFile1 = fopen(argv[1], "r");
		elfFile2 = fopen(argv[2], "r");
		outputFile = fopen(argv[3], "w");
		tempFile = fopen("tempFileSectionTable","w");
		if (elfFile1 == NULL || elfFile2==NULL || outputFile==NULL) {
		printf("Erreur d'ouverture d'un fichier.\n");
		}
		else {
			fread(&header1, sizeof(header1),1, elfFile1);
			fread(&header2, sizeof(header2),1, elfFile2);
			// check so its really an elf file
			if (memcmp(header1.e_ident, ELFMAG, SELFMAG) == 0 && memcmp(header2.e_ident, ELFMAG, SELFMAG) == 0 ) {
				fwrite(&header1,sizeof(header1),1,outputFile);
				for (int i=0; i<header1.e_shnum);i++){ //parcours des sections du fichier 1
					fseek(elfFile1, header1.e_shoff) + i * header1.e_shentsize, SEEK_SET);
					fread(&section1, 1, sizeof(section2), elfFile1);
					get_section_name(elfFile1,header1,section1,nom_section1);
					for (int j=0;j<header2.e_shnum;j++){ //parcours des sections du fichier 2
						fseek(elfFile2, header2.e_shoff + j * header2.e_shentsize, SEEK_SET);
						fread(&section2, 1, sizeof(section2), elfFile2);
						get_section_name(elfFile2,header2,section2,nom_section2);
						if(!strcmp(nom_section1,nom_section2)){ //si les sections ont le meme nom
							fprintf(tempFile,"%s",nom_section1);
							sectionOut.sh_name=section_name;
							sectionOut.sh_type=section1.sh_type;
							sectionOut.sh_flags=section1.sh_flags;
							sectionOut.sh_addr=section1.sh_addr;
							sectionOut.sh_offset=ftell(outputFile);
							sectionOut.sh_size=section1.sh_size+section2.sh_size;
						}
					}
				}
			}
		}
	fclose(tempFile);
	//remove("tempFileSectionTable");
}
