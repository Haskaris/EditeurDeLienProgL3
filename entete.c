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
        printf("En-tête ELF:\n  Magique:\t");
        for (int i = 0; i < 16; i++)
          printf("%02x ",header.e_ident[i]);

        printf("\n  Classe:\t\t\t\t");
        switch (header.e_ident[EI_CLASS]) {
          case ELFCLASS32:
            printf("ELF32");
          break;
          case ELFCLASS64:
            printf("ELF64");
          break;
          default://ELFCLASSNONE
            printf("Invalide");
        }

        printf("\n  Données:\t\t\t\t");
        switch (header.e_ident[EI_DATA]) {
          case ELFDATA2LSB:
            printf("Complément à deux, petit boutien (little endians)");
          break;
          case ELFDATA2MSB:
            printf("Complément à deux, gros boutien (big endians)");
          break;
          default://ELFDATANONE
            printf("Invalide");
        }

        printf("\n  Version:\t\t\t\t");
        switch (header.e_ident[EI_VERSION]) {
          case EV_CURRENT:
            printf("1 (current)");
          break;
          default://EV_NONE
            printf("Invalide");
        }

        printf("\n  OS/ABI:\t\t\t\t");
        switch (header.e_ident[EI_OSABI]) {
          case ELFOSABI_HPUX:
            printf("HP-UX");
          break;
          case ELFOSABI_NETBSD:
            printf("NetBSD");
          break;
          case ELFOSABI_LINUX:
            printf("Linux");
          break;
          case ELFOSABI_SOLARIS:
            printf("Solaris");
          break;
          case ELFOSABI_IRIX:
            printf("IRIX");
          break;
          case ELFOSABI_FREEBSD:
            printf("FreeBSD");
          break;
          case ELFOSABI_TRU64:
            printf("UNIX TRU64");
          break;
          case ELFOSABI_ARM:
            printf("Architecture ARM");
          break;
          case ELFOSABI_STANDALONE:
            printf("autonome (intégrée)");
          break;
          default://ELFOSABI_NONE ou ELFOSABI_SYSV
            printf("UNIX System V");
        }
        printf("\n  Version ABI:\t\t\t\t0x%01x",header.e_ident[EI_ABIVERSION]);
        printf("\n  Adresse du point d'entrée:\t\t0x%01x", header.e_ident[EI_PAD]);
        printf("\n  Début des en-tête de programme:\t%lu", header.e_phoff);
        printf("\n  Début des en-tête de section:\t\t%lu", header.e_shoff);
        printf("\n  Fanions:\t\t\t\t0x%01x", header.e_flags);
        printf("\n  Taille de cet en-tête:\t\t%d (octets)", header.e_ehsize);
        printf("\n  Taille de l'en-tête du programme:\t%d (octets)", header.e_phentsize);
        printf("\n  Nombre d'en-tête du programme:\t%d", header.e_phnum);
        printf("\n  Taille des en-têtes de section:\t%d (octets)", header.e_shentsize);
        printf("\n  Nombre d'en-têtes de section:\t\t%d", header.e_shnum);
        printf("\n  Table d'indexes des chaînes d'en-tête de section:\t%d\n", header.e_shstrndx);
      }
      fclose(elfFile);
    }
  }
}
