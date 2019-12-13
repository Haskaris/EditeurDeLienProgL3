#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>

int main(int argc, char *argv[]) {
  FILE * elfFile;

  Elf64_Ehdr header;

  char buff[255];
  if (argc == 1) {
    printf("Tu l'utilises pas comme ça.\n");
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
         printf("VALIDE");
      }
      fclose(elfFile);
    }
  }
}
