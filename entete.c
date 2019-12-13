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
        printf("\n  Version ABI:\t\t\t\t%d",header.e_ident[EI_ABIVERSION]);
        printf("\n  Type:\t\t\t\t");
        switch (header.e_type) {
          case ET_REL:
            printf("REL (Fichier repositionnable)");
          break;
          case ET_EXEC:
            printf("EXEC (Fichier executable)");
          break;
          case ET_DYN:
            printf("DYN (Objet pratagé)");
          break;
          case ET_CORE:
            printf("CORE (Fichier core)");
          break;
          default://ET_NONE
            printf("Type inconnu");
        }
        printf("\n  Machine:\t\t\t\t\t");
        switch (header.e_machine) {
          case EM_M32:
            printf("WE 32100 AT&T");
          break;
          case EM_SPARC:
            printf("SPARC Sun Microsystems");
          break;
          case EM_386:
            printf("80386 Intel");
          break;
          case EM_68K:
            printf("68000 Motorola");
          break;
          case EM_88K:
            printf("88000 Motorola");
          break;
          case EM_860:
            printf("80860 Intel");
          break;
          case EM_MIPS:
            printf("RS3000 MIPS (uniquement gros boutien)");
          break;
          case EM_PARISC:
            printf("HP/PA");
          break;
          case EM_SPARC32PLUS:
            printf("SPARC avec jeu d'instructions amélioré");
          break;
          case EM_PPC:
            printf("PowerPC");
          break;
          case EM_PPC64:
            printf("PowerPC 64 bits");
          break;
          case EM_S390:
            printf("S/390 IBM");
          break;
          case EM_ARM:
            printf("ARM");
          break;
          case EM_SH:
            printf("SuperH de Renesas");
          break;
          case EM_SPARCV9:
            printf("SPARC v9 64 bits");
          break;
          case EM_IA_64:
            printf("Itanium Intel");
          break;
          case EM_X86_64:
            printf("x86-64 AMD");
          break;
          case EM_VAX:
            printf("Vax DEC");
          break;
          default://EM_NONE
            printf("Machine inconnue");
        }
        printf("\n  Version:\t\t\t\t0x%01x",header.e_ident[EI_ABIVERSION]);
        switch (header.e_ident[EI_VERSION]) {
          case EV_CURRENT:
            printf("0x1");
          break;
          default://EV_NONE
            printf("Invalide");
        }
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
