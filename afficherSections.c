#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>
#include <assert.h>

int isbigendian(){
    return 1;
}

uint16_t byteshift16(uint16_t n){
    if (isbigendian()){
        return ((n>>8)&0xff) | ((n<<8)&0xff00);
    }
    else{
        return n;
    }
}

uint32_t byteshift32(uint32_t n) {
    if (isbigendian()){
          return ((n>>24)&0xff) | ((n<<8)&0xff0000) | ((n>>8)&0xff00) | ((n<<24)&0xff000000);
    }
    else{
        return n;
    }
}


void get_section_name(FILE* elfFile,Elf32_Ehdr header,Elf32_Shdr section, char* name){
    Elf32_Shdr table_chaine;
    fseek(elfFile,byteshift32(header.e_shoff)+byteshift16(header.e_shstrndx)*byteshift16(header.e_shentsize),SEEK_SET);
    fread(&table_chaine, 1, sizeof(Elf32_Shdr), elfFile);
    fseek(elfFile,byteshift32(table_chaine.sh_offset)+byteshift32(section.sh_name),SEEK_SET);
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

	char tableName[255];

  char buff[255];
  if (argc != 3) {
    printf("Utilisation : %s num <ELF_FILE>\n", argv[0]);
    exit(1);
  }
  else {
    elfFile = fopen(argv[2], "r");
    if (elfFile == NULL) {
      printf("Erreur d'ouverture du fichier.\n");
    }
    else {
      // read the header
      fread(&header, 1, sizeof(header), elfFile);

      // check so its really an elf file
      if (memcmp(header.e_ident, ELFMAG, SELFMAG) == 0) {
        //SectNames = malloc(sectHdr.sh_size);
        //fseek(ElfFile, sectHdr.sh_offset, SEEK_SET);
        //fread(SectNames, 1, sectHdr.sh_size, ElfFile);

        // read all section headers
        int i = 0;

        while (i <= byteshift16(header.e_shnum) && i < atoi(argv[1])) {
          i++;
        }

        if (i <= byteshift16(header.e_shnum)) {
					fseek(elfFile, byteshift32(header.e_shoff) + i * sizeof(section), SEEK_SET);
          fread(&section, 1, sizeof(section), elfFile);

          get_section_name(elfFile, header, section, tableName);

          printf("Vidange hexadécimale de la section << %s >> :\n", tableName);

          for(int j = 0; j < byteshift32(section.sh_size); j+=16) {
            fseek(elfFile,byteshift32(section.sh_offset)+j,SEEK_SET);
            printf("  0x%08x ", j);
            for (int k = 0; k < 16; k++){
             unsigned char c;
             fscanf(elfFile,"%c",&c);
             printf("%02x",c);
               if (k == 3 || k == 7 || k == 11)
                 printf(" ");
             }
             printf(" ");
             fseek(elfFile,byteshift32(section.sh_offset)+j,SEEK_SET);
             for (int k = 0; k < 16; k++){
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
      }
      fclose(elfFile);
    }
  }
}
