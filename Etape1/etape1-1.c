#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>
#include "etape1-1.h"

void classeArchitecture(Elf32_Ehdr header) {
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
}

void encodage(Elf32_Ehdr header) {
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
}

void fileVersion(Elf32_Ehdr header, int addressMode) {
  switch (header.e_ident[EI_VERSION]) {
    case EV_CURRENT:
      if (!addressMode)
        printf("1 (current)");
      else
        printf("0x1");
    break;
    default://EV_NONE
      printf("Invalide");
  }
}

void osAbi(Elf32_Ehdr header) {
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
}

void fileType(Elf32_Ehdr header) {
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
}

void machine(Elf32_Ehdr header) {
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
}

void affichage_Entete_Fichier_ELF(Elf32_Ehdr header) {
  	char buff[255];

  	printf("En-tête ELF:\n  Magique:\t");
  	for (int i = 0; i < 16; i++)
          	printf("%02x ",header.e_ident[i]);

        printf("\n  Classe:\t\t\t\t");
        classeArchitecture(header);

        printf("\n  Données:\t\t\t\t");
        encodage(header);

        printf("\n  Version:\t\t\t\t");
        fileVersion(header, 0);

        printf("\n  OS/ABI:\t\t\t\t");
        osAbi(header);

        printf("\n  Version ABI:\t\t\t\t%d",header.e_ident[EI_ABIVERSION]);

        printf("\n  Type:\t\t\t\t");
        fileType(header);

        printf("\n  Machine:\t\t\t\t\t");
        machine(header);

        printf("\n  Version:\t\t\t\t");
        fileVersion(header, 1);

        printf("\n  Adresse du point d'entrée:\t\t%01x", header.e_ident[EI_PAD]);
        printf("\n  Début des en-tête de programme:\t%u", header.e_phoff);
        printf("\n  Début des en-tête de section:\t\t%u", header.e_shoff);
        printf("\n  Fanions:\t\t\t\t0x%01x", header.e_flags);
        printf("\n  Taille de cet en-tête:\t\t%d (octets)", header.e_ehsize);
        printf("\n  Taille de l'en-tête du programme:\t%d (octets)", header.e_phentsize);
        printf("\n  Nombre d'en-tête du programme:\t%d", header.e_phnum);
        printf("\n  Taille des en-têtes de section:\t%d (octets)", header.e_shentsize);
        printf("\n  Nombre d'en-têtes de section:\t\t%d", header.e_shnum);
        printf("\n  Table d'indexes des chaînes d'en-tête de section:\t%d\n", header.e_shstrndx);
}
