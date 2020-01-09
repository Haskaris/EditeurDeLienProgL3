//Inclure les autres du .h ?
#include "etape1-3.h"

void affichage_Contenu_Section(FILE *elfFile, Elf32_Ehdr header, int numSection) {
	Elf32_Shdr section;

	// Lit toutes les sections, et se décale sur la section demandée
	int i = 0;
	while (i <= header.e_shnum && i < numSection)
		i++;

	if (i <= header.e_shnum) {
		//Lit la section
		fseek(elfFile, header.e_shoff + i * sizeof(section), SEEK_SET);
		litEtInverse_Section(elfFile, header, &section);

		//Récupère le nom de la section
		char* tableName = NULL;
		tableName = get_section_name(elfFile, header, section);
		printf("Vidange hexadécimale de la section << %s >> :\n", tableName);

		for(int j = 0; j < section.sh_size; j+=16) {
			fseek(elfFile, section.sh_offset + j, SEEK_SET);
			printf("	0x%08x ", j);
			for (int k = 0; k < 16; k++){
				if (j+k == section.sh_size) {
					for (int o = k; o < 16; o++) {
						printf("	");
						if (o == 3 || o == 7 || o == 11) {
							printf(" ");
						}
					}
					break;
				}
				unsigned char c;
				fscanf(elfFile,"%c",&c);
				printf("%02x",c);
				if (k == 3 || k == 7 || k == 11) {
					printf(" ");
				}
			}
			printf(" ");
			fseek(elfFile, section.sh_offset + j, SEEK_SET);
			for (int k = 0; k < 16; k++){
				if (j+k == section.sh_size) {
					break;
				}
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
