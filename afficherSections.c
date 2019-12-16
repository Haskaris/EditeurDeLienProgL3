#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>

static inline Elf64_Shdr *elf_sheader(Elf64_Ehdr *hdr) {
	return (Elf64_Shdr *)((int)hdr + hdr->e_shoff);
}

static inline Elf64_Shdr *elf_section(Elf64_Ehdr *hdr, int idx) {
	return &elf_sheader(hdr)[idx];
}

static inline char *elf_str_table(Elf64_Ehdr *hdr) {
	if(hdr->e_shstrndx == SHN_UNDEF) return NULL;
	return (char *)hdr + elf_section(hdr, hdr->e_shstrndx)->sh_offset;
}

static inline char *elf_lookup_string(Elf64_Ehdr *hdr, int offset) {
	char *strtab = elf_str_table(hdr);
	if(strtab == NULL) return NULL;
	return strtab + offset;
}

int main(int argc, char *argv[]) {
  FILE * elfFile;

  Elf64_Ehdr header;
  Elf64_Shdr section;

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

        while (i <= header.e_shnum && i < atoi(argv[1])) {
          i++;
        }

        if (i <= header.e_shnum) {
          fseek(elfFile, header.e_shoff + i * sizeof(section), SEEK_SET);
          fread(&section, 1, sizeof(section), elfFile);
          //
          // char *strName = elf_lookup_string(&header, i * sizeof(section));
          // printf("OUI\n");
          // printf("%s\n", strName);
          for(int j = 0; j < section.sh_size; j+=16) {
            fseek(elfFile,section.sh_offset+j,SEEK_SET);
            printf("0x%08x ", j);
            for (int k = 0; k < 16; k++){
             unsigned char c;
             fscanf(elfFile,"%c",&c);
             printf("%02x",c);
               if (k == 3 || k == 7 || k == 11)
                 printf(" ");
             }
             printf(" ");
             fseek(elfFile,section.sh_offset+j,SEEK_SET);
             for (int k = 0; k < 16; k++){
              unsigned char c;
              fscanf(elfFile,"%c",&c);
              if(c<32 || c>127) printf(".");
              else printf("%c",c);
              }

            printf("\n");
          }
        }
        else printf("Numéro de section inexistante.\n");
      }
      fclose(elfFile);
    }
  }
}
