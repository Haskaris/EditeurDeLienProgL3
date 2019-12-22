#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>
#include "etape1-3.h"


void get_section_name_3(FILE* elfFile, Elf32_Ehdr header, Elf32_Shdr section, char* name){
    Elf32_Shdr table_chaine;
    fseek(elfFile, header.e_shoff + header.e_shstrndx * header.e_shentsize, SEEK_SET);
    fread(&table_chaine, 1, sizeof(Elf32_Shdr), elfFile);
    //Inversion de la table si elle n'est pas en litle Endian
    if (isbigendian(header)){
	inversion_Sections(&table_chaine);
    }
    fseek(elfFile, table_chaine.sh_offset + section.sh_name, SEEK_SET);
    char c=fgetc(elfFile);
    int i=0;
    while(c!='\0'){
        name[i]=c;
        i++;
        c=fgetc(elfFile);
    }
    name[i]='\0';

}

void affichage_Contenu_Section(FILE *elfFile, Elf32_Ehdr header, int numSection) {
  	Elf32_Shdr section;
  	char tableName[255];

  	char buff[255];

        // Lit toutes les sections, et se décale sur la section demandée
        int i = 0;
        while (i <= header.e_shnum && i < numSection)
          i++;

        if (i <= header.e_shnum) {
          	//Lit la section
	  	fseek(elfFile, header.e_shoff + i * sizeof(section), SEEK_SET);
          	fread(&section, 1, sizeof(section), elfFile);
		//Inversion de la table si elle n'est pas en litle Endian
   	 	if (isbigendian(header)){
			inversion_Sections(&section);
    		}
          	//Récupère le nom de la section
          	get_section_name_3(elfFile, header, section, tableName);
	
          	printf("Vidange hexadécimale de la section << %s >> :\n", tableName);

          	for(int j = 0; j < section.sh_size; j+=16) {
            		fseek(elfFile, section.sh_offset + j, SEEK_SET);
            		printf("  0x%08x ", j);
            		for (int k = 0; k < 16; k++){
              			if (j+k == section.sh_size) {
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
        }
        else {
	     	printf("Numéro de section inexistante.\n");
	}
}
