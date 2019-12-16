#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>

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
        for (int i = 0; i < header.e_shnum; i++)
          {
          const char* name = "";

          fseek(elfFile, header.e_shoff + i * sizeof(section), SEEK_SET);
          fread(&section, 1, sizeof(section), elfFile);

          // print section name
          //if (sectHdr.sh_name);
          //  name = SectNames + sectHdr.sh_name;
          printf("%2u %s\n", i, section.sh_name);
        }
      }
      fclose(elfFile);
    }
  }
}
