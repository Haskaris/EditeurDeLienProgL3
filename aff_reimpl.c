#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>

void get_section_name(FILE* elfFile,Elf64_Ehdr header,Elf64_Shdr section, char* name){
    Elf64_Shdr table_chaine;
    fseek(elfFile,header.e_shoff+header.e_shstrndx*header.e_shentsize,SEEK_SET);
    fread(&table_chaine, 1, sizeof(Elf64_Shdr), elfFile);
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

void afficher_relocation_type(int type){
    switch(type){
        case 0:
            printf("R_ARM_NONE");
            break;
        case 1:
            printf("R_ARM_PC24");
            break;
        case 2:
            printf("R_ARM_ABS32");
            break;
        case 3:
            printf("R_ARM_REL32");
            break;
        case 4:
            printf("R_ARM_LDR_PC_G0");
            break;
        case 5:
            printf("R_ARM_ABS16");
            break;
        case 6:
            printf("R_ARM_ABS12");
            break;
        case 7:
            printf("R_ARM_THM_ABS5");
            break;
        case 8:
            printf("R_ARM_ABS8");
            break;
        case 9:
            printf("R_ARM_SBREL32");
            break;
        case 10:
            printf("R_ARM_THM_CALL");
            break;
        default:
            printf("TYPE INCONNU");
            break;
    }
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
        if(!header.e_shoff) printf("AH !");
        // read all section headers
        char nom_section[255];
        printf("\n%d  %d\n",SHT_REL,SHT_RELA);
        for (int i = 0; i < header.e_shnum; i++)
          {
          fseek(elfFile, header.e_shoff + i * header.e_shentsize, SEEK_SET);
          fread(&section, 1, sizeof(Elf64_Shdr), elfFile);
          printf("TYPE : %d\n",section.sh_type);
          get_section_name(elfFile,header,section,nom_section);
          printf("NOM : %s",nom_section);
          if(section.sh_type==SHT_RELA){
              Elf64_Rela rela;
              get_section_name(elfFile,header,section,nom_section);
              int nb_entree=(int)section.sh_size/sizeof(Elf64_Rela);
              printf("Section de réadressage '%s' à l'adresse de décalage 0x%04lx contient %d entrées\n",nom_section,section.sh_offset,nb_entree);
              fseek(elfFile,section.sh_offset,SEEK_SET);
              for (int i=0;i<nb_entree;i++){
                  fread(&rela,1,sizeof(rela),elfFile);
                  printf("décalage : %012lx  ",rela.r_offset);
                  printf("type : ");
                  afficher_relocation_type(ELF64_R_TYPE(rela.r_info));
                  printf("  ");
                  printf("index : %lu \n",ELF64_R_SYM(rela.r_info));
              }
              printf("\n");
          }
          else if(section.sh_type==SHT_REL){
              printf("SH");
              Elf64_Rel rel;
              get_section_name(elfFile,header,section,nom_section);
              int nb_entree=(int)section.sh_size/sizeof(Elf64_Rel);
              printf("Section de réadressage '%s' à l'adresse de décalage 0x%04lx contient %d entrées\n",nom_section,section.sh_offset,nb_entree);
              fseek(elfFile,section.sh_offset,SEEK_SET);
              for (int i=0;i<nb_entree;i++){
                  fread(&rel,1,sizeof(rel),elfFile);
                  printf("décalage : %012lx  ",rel.r_offset);
                  printf("type : ");
                  afficher_relocation_type(ELF64_R_TYPE(rel.r_info));
                  printf("  ");
                  printf("index : %lu \n",ELF64_R_SYM(rel.r_info));
              }
              printf("\n");
          }

        }
      }
      fclose(elfFile);
    }
  }
}
