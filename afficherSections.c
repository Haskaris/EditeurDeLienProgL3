#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>

#include <unistd.h>

void get_section_name(int32_t fd, FILE* elfFile, Elf32_Ehdr header, Elf32_Shdr section, char* name){
    Elf32_Shdr table_chaine;
    //fseek(elfFile,header.e_shoff+header.e_shstrndx*sizeof(Elf32_Shdr),SEEK_SET);
    //fread(&table_chaine, 1, sizeof(section), elfFile);
    //fseek(elfFile,table_chaine.sh_offset+section.sh_name,SEEK_SET);

    lseek(fd, header.e_shoff+header.e_shstrndx*sizeof(Elf32_Shdr), SEEK_SET);
    read(fd, (void *)&table_chaine, sizeof(Elf32_Shdr));
    lseek(fd, table_chaine.sh_offset+section.sh_name, SEEK_SET);

    /*char c=fgetc(elfFile);
    int i=0;
    while(c!='\0'){
        name[i]=c;
        i++;
        c=fgetc(elfFile);
    }*/
    name[/*i*/0]='\0';
}

void read_section_header_table(int32_t fd, Elf32_Ehdr eh, Elf32_Shdr sh_table[])
{
	uint32_t i;

	lseek(fd, (off_t)eh.e_shoff, SEEK_SET);

	for(i=0; i<eh.e_shnum; i++) {
		read(fd, (void *)&sh_table[i], eh.e_shentsize);
	}

}

char * read_section(int32_t fd, Elf32_Shdr sh) {
	char* buff = malloc(sh.sh_size);
	if(!buff) {
		printf("%s:Failed to allocate %dbytes\n",
				__func__, sh.sh_size);
	}

	lseek(fd, (off_t)sh.sh_offset, SEEK_SET);
	read(fd, (void *)buff, sh.sh_size);

	return buff;
}

void read_elf_header(int32_t fd, Elf32_Ehdr *elf_header)
{
	lseek(fd, (off_t)0, SEEK_SET);
	read(fd, (void *)elf_header, sizeof(Elf32_Ehdr));
}

int main(int argc, char *argv[]) {
  FILE * elfFile;

  Elf32_Ehdr header;
  Elf32_Shdr section;

  Elf32_Shdr* sh_tbl;	/* section-header table is variable size */

	char tableName[255];

  char buff[255];
  if (argc != 3) {
    printf("Utilisation : %s num <ELF_FILE>\n", argv[0]);
    exit(1);
  }
  else {
    printf("Fichier à ouvrir : %s\n", argv[2]);
    elfFile = fopen(argv[2], "r");
    if (elfFile == NULL) {
      printf("Erreur d'ouverture du fichier.\n");
    }
    else {
      // Lit l'entête
      //fread(&header, 1, sizeof(Elf32_Ehdr), elfFile);
      int32_t fd = (int32_t)fileno(elfFile);
      //lseek(fd, (off_t)0, SEEK_SET);
      //read(fd, (void *)&header, sizeof(Elf32_Ehdr));
      read_elf_header(fd, &header);

      // Vérifie que le fichier est bien un ELF
      if (memcmp(header.e_ident, ELFMAG, SELFMAG) == 0) {
        sh_tbl = malloc(header.e_shentsize * header.e_shnum);

        //Rempli la table des sections
        read_section_header_table(fd, header, sh_tbl);

        char* sh_str = read_section(fd, sh_tbl[header.e_shstrndx]);

        printf("Sections names : %s\n", sh_str/* + sh_tbl[atoi(argv[1])].sh_name*/);
        // read all section headers
        /*int i = 0;
        printf("Il devrait y avoir %d sections\n", header.e_shnum);
        while (i <= header.e_shnum && i < atoi(argv[1])) {
          i++;
        }
        printf("Passé en argument : %d \n", atoi(argv[1]));
        if (i <= header.e_shnum) {
					//fseek(elfFile, header.e_shoff + i * sizeof(Elf32_Shdr), SEEK_SET);
          //fread(&section, 1, sizeof(Elf32_Shdr), elfFile);
          lseek(fd, header.e_shoff + i * sizeof(Elf32_Shdr), SEEK_SET);
          read(fd, (void *)&section, sizeof(Elf32_Ehdr));

          get_section_name(fd, elfFile, header, section, tableName);

          printf("Vidange hexadécimale de la section << %s >> :\n", tableName);
          printf("Il y a %d sections\n", (int)section.sh_size);
          for(int j = 0; j < section.sh_size; j+=16) {
            //fseek(elfFile,section.sh_offset+j,SEEK_SET);
            lseek(fd, section.sh_offset+j, SEEK_SET);
            printf("  0x%08x ", j);
            for (int k = 0; k < 16; k++){
             unsigned char c;
             fscanf(elfFile,"%c",&c);
             printf("%02x",c);
               if (k == 3 || k == 7 || k == 11)
                 printf(" ");
             }
             printf(" ");
             //fseek(elfFile,section.sh_offset+j,SEEK_SET);
             lseek(fd, section.sh_offset+j, SEEK_SET);
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
					printf("Numéro de section inexistante.\n");}*/
      } else {
        printf("Pas un fichier elf.\n");
      }
      fclose(elfFile);
    }
  }
}
