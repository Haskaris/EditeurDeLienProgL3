#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>

void get_section_name(FILE* elfFile,Elf64_Ehdr header,Elf64_Shdr section, char* name){
    Elf64_Shdr table_chaine;
    fseek(elfFile,header.e_shoff+header.e_shstrndx*sizeof(section),SEEK_SET);
    fread(&table_chaine, 1, sizeof(section), elfFile);
    fseek(elfFile,table_chaine.sh_offset+section.sh_name,SEEK_SET);
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

  Elf64_Ehdr header;
  Elf64_Shdr section;

  char buff[255];
  if (argc != 2) {
    printf("Utilisation : %s <ELF_FILE>\n", argv[0]);
    exit(1);
  }
  else {
    elfFile = fopen(argv[1], "r");
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
        char nom_section[255];
        for (int i = 0; i < header.e_shnum; i++)
          {

          fseek(elfFile, header.e_shoff + i * sizeof(section), SEEK_SET);
          fread(&section, 1, sizeof(section), elfFile);
          if(section.sh_type==SHT_RELA){
              get_section_name(elfFile,header,section,nom_section);
              printf("%s\n",nom_section);
          }

        }
      }
      fclose(elfFile);
    }
  }
}
