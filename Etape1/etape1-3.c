//Inclure les autres du .h ?
#include "etape1-3.h"

void affichage_Contenu_Section(FILE *elfFile, Elf32_Ehdr header, int bigEndian, int numSection) {
	Elf32_Shdr section;

	// Lit toutes les sections, et se décale sur la section demandée
	int i = 0;
	while (i <= byteshift16(header.e_shnum, bigEndian) && i < numSection)
		i++;

	if (i <= byteshift16(header.e_shnum, bigEndian)) {
		//Lit la section
		fseek(elfFile, byteshift32(header.e_shoff, bigEndian) + i * sizeof(section), SEEK_SET);
		fread(&section, 1, sizeof(section), elfFile);

		//Récupère le nom de la section
		//Penser à free
		char* tableName = get_section_name(elfFile, header, section, bigEndian);
		printf("Vidange hexadécimale de la section << %s >> :\n", tableName);
		free(tableName);
		
		for(int j = 0; j < byteshift32(section.sh_size, bigEndian); j+=16) {
			fseek(elfFile,byteshift32(section.sh_offset, bigEndian)+j,SEEK_SET);
			printf("  0x%08x ", j);
			for (int k = 0; k < 16; k++){
				if (j+k == byteshift32(section.sh_size, bigEndian)) {
					for (int o = k; o < 16; o++) {
						printf("  ");
						if (o == 3 || o == 7 || o == 11)
	  					printf(" ");
					}
					break;
				}
				unsigned char c;
				fscanf(elfFile,"%c",&c);
				printf("%02x",c);
				if (k == 3 || k == 7 || k == 11)
					printf(" ");
			}
			printf(" ");
			fseek(elfFile,byteshift32(section.sh_offset, bigEndian)+j,SEEK_SET);
			for (int k = 0; k < 16; k++){
				if (j+k == byteshift32(section.sh_size, bigEndian))
	 				break;
				unsigned char c;
				fscanf(elfFile,"%c",&c);
				if(c<32 || c>127) printf(".");
				else printf("%c",c);
			}
			printf("\n");
		}
	} else {
		printf("Numéro de section inexistante.\n");
	}
}
