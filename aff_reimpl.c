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
              Elf64_Rela rela;
              get_section_name(elfFile,header,section,nom_section);
              int nb_entree=(int)section.sh_size/sizeof(Elf64_Rela);
              printf("Section de réadressage '%s' à l'adresse de décalage 0x%04lx contient %d entrées\n",nom_section,section.sh_offset,nb_entree);
              fseek(elfFile,section.sh_offset,SEEK_SET);
              for (int i=0;i<nb_entree;i++){
                  fread(&rela,1,sizeof(rela),elfFile);
                  printf("décalage : %012lx  ",rela.r_offset);
                  printf("type : %lu  ",ELF64_R_TYPE(rela.r_info));
                  printf("index : %lu \n",ELF64_R_SYM(rela.r_info));
              }
              printf("\n");
          }

        }
      }
      fclose(elfFile);
    }
  }
}
