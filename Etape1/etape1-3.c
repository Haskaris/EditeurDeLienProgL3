#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>
#include "etape1-3.h"


void get_section_name(FILE* elfFile, Elf32_Ehdr header, Elf32_Shdr section, char* name, int bigEndian){
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

int main(int argc, char *argv[]) {
  FILE * elfFile;

  Elf32_Ehdr header;
  Elf32_Shdr section;
  int bigEndian = 0;
  char tableName[255];

  char buff[255];
  if (argc != 3) {
    printf("Utilisation : %s num <ELF_FILE>\n", argv[0]);
    exit(1);
  }
  else {
    printf("Fichier à ouvrir : %s\n", argv[2]);
    elfFile = fopen(argv[2], "r");
    if (elfFile == NULL) {
      printf("Erreur d'ouverture du fichier.\n");
    }
    else {
      // Lit le header
      fread(&header, 1, sizeof(header), elfFile);
      if (isbigendian(header)){
		bigEndian = 1;
      } 

      // Est-ce que c'est vraiment un fichier ELF ?
      if (memcmp(header.e_ident, ELFMAG, SELFMAG) == 0) {

        // Lit toutes les sections, et se décale sur la section demandée
        int i = 0;
        while (i <= byteshift16(header.e_shnum, bigEndian) && i < atoi(argv[1]))
          i++;

        if (i <= byteshift16(header.e_shnum, bigEndian)) {
          //Lit la section
	  fseek(elfFile, byteshift32(header.e_shoff, bigEndian) + i * sizeof(section), SEEK_SET);
          fread(&section, 1, sizeof(section), elfFile);

          //Récupère le nom de la section
          get_section_name(elfFile, header, section, tableName, bigEndian);

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
	     printf("Numéro de section inexistante.\n");}
      } else {
        printf("Pas un fichier elf.\n");
      }
      fclose(elfFile);
    }
  }
}
