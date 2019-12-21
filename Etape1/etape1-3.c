//Inclure les autres du .h ?
#include "etape1-3.h"


void get_section_name_3(FILE* elfFile, Elf32_Ehdr header, Elf32_Shdr section, char* name, int bigEndian){
    Elf32_Shdr table_chaine;
    fseek(elfFile, byteshift32(header.e_shoff, bigEndian) + byteshift16(header.e_shstrndx, bigEndian) * byteshift16(header.e_shentsize, bigEndian), SEEK_SET);
    fread(&table_chaine, 1, sizeof(Elf32_Shdr), elfFile);
    fseek(elfFile, byteshift32(table_chaine.sh_offset, bigEndian) + byteshift32(section.sh_name, bigEndian), SEEK_SET);
    char c=fgetc(elfFile);
    int i=0;
    while(c!='\0'){
        name[i]=c;
        i++;
        c=fgetc(elfFile);
    }
    name[i]='\0';

}

void affichage_Contenu_Section(FILE *elfFile, Elf32_Ehdr header, int bigEndian, int numSection) {
  	Elf32_Shdr section;
  	char tableName[255];

  	char buff[255];

        // Lit toutes les sections, et se décale sur la section demandée
        int i = 0;
        while (i <= byteshift16(header.e_shnum, bigEndian) && i < numSection)
          i++;

        if (i <= byteshift16(header.e_shnum, bigEndian)) {
          	//Lit la section
	  	fseek(elfFile, byteshift32(header.e_shoff, bigEndian) + i * sizeof(section), SEEK_SET);
          	fread(&section, 1, sizeof(section), elfFile);

          	//Récupère le nom de la section
          	get_section_name_3(elfFile, header, section, tableName, bigEndian);

          	printf("Vidange hexadécimale de la section << %s >> :\n", tableName);

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
               			if (j+k == byteshift32(section.sh_size, bigEndian)) {
                 			break;
               			}
               			unsigned char c;
               			fscanf(elfFile,"%c",&c);
               			if(c<32 || c>127) printf(".");
               			else printf("%c",c);
            		}
            		printf("\n");
          	}
        }
        else {
	     	printf("Numéro de section inexistante.\n");
	}
}
