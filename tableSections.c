#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>

int isbigendian(){
    return 1;
}

uint16_t byteshift16(uint16_t n){
    if (isbigendian()){
        return ((n>>8)&0xff) | ((n<<8)&0xff00);
    }
    else{
        return n;
    }
}

uint32_t byteshift32(uint32_t n) {
    if (isbigendian()){
          return ((n>>24)&0xff) | ((n<<8)&0xff0000) | ((n>>8)&0xff00) | ((n<<24)&0xff000000);
    }
    else{
        return n;
    }
}


void headtext(){

  printf("Il y a 11 en-têtes de section, débutant à l'adresse de décalage 0x7b8:\n\nEn-têtes de section :\n[Nr] Nom               Type             Adresse           Décalage\n       Taille            TaillEntrée   Fanion  Lien  Info  Alignement\n");
}

void foottext(){
  printf("Clé des fanions :\n  W (écriture), A (allocation), X (exécution), M (fusion), S (chaînes), I (info),\n  L (ordre des liens), O (traitement supplémentaire par l'OS requis), G (groupe),\n  T (TLS), C (compressé), x (inconnu), o (spécifique à l'OS), E (exclu),\n  l (grand), p (processor specific)\n");

}

void get_section_name(FILE* elfFile,Elf32_Ehdr header,Elf32_Shdr section, char* name){
    Elf32_Shdr table_chaine;
    fseek(elfFile,byteshift32(header.e_shoff)+byteshift16(header.e_shstrndx)*byteshift16(header.e_shentsize),SEEK_SET);
    fread(&table_chaine, 1, sizeof(Elf32_Shdr), elfFile);
    fseek(elfFile,byteshift32(table_chaine.sh_offset)+byteshift32(section.sh_name),SEEK_SET);
    char c=fgetc(elfFile);
    int i=0;
    while(c!='\0'){
        name[i]=c;
        i++;
        c=fgetc(elfFile);
    }
    name[i]='\0';
}

void align(char* str, int indent) {
  int length = strlen(str);

  //printf("AAAAAAAAAAAAAAAAAAA %d : %s AAAAAAAAAAAAAAAAAA", length, str);
  if(length >= indent) return;
  for(int i = length; i < indent; i++) {
    printf(" ");
  }
}

void flagsToString(uint32_t flag) {
  int i=0;
  if((flag&SHF_WRITE) != 0) {
    //Ajouter W
    printf("W");
  } else i++;
  if((flag&SHF_ALLOC) != 0) {
    //Ajouter A
    printf("A");
  } else i++;
  if((flag&SHF_EXECINSTR) != 0) {
    //Ajouter X
    printf("X");
  } else i++;
  if((flag&16) != 0) {
    //Ajouter M
    printf("M");
  } else i++;
  if((flag&32) != 0) {
    //Ajouter S
    printf("S");
  } else i++;
  if((flag&64) != 0) {
    //Ajouter I
    printf("I");
  } else i++;
  if((flag&128) != 0) {
    //Ajouter I
    printf("L");
  } else i++;
  if((flag&256) != 0) {
    //Ajouter I
    printf("O");
  } else i++;
  for (int count = 0; count < i; count++) printf(" ");
  printf("\t");
}
void typeToString(uint32_t type) {
  switch (type) {
    case SHT_NULL:
      printf("NULL             ");
      break;
    case SHT_PROGBITS:
      printf("PROGBITS         ");
      break;
    case SHT_SYMTAB:
      printf("SYMTAB           ");
      break;
    case SHT_STRTAB:
      printf("STRTAB           ");
      break;
    case SHT_RELA:
      printf("RELA             ");
      break;
    case SHT_HASH:
      printf("HASH             ");
      break;
    case SHT_DYNAMIC:
      printf("DYNAMIC          ");
      break;
    case SHT_NOTE:
      printf("NOTE             ");
      break;
    case SHT_NOBITS:
      printf("NOBITS           ");
      break;
    case SHT_REL:
      printf("REL              ");
      break;
    case SHT_SHLIB:
      printf("SHLIB            ");
      break;
    case SHT_DYNSYM:
      printf("DYNSYM           ");
      break;
    case SHT_LOPROC:
      printf("LOPROC           ");
      break;
    case SHT_HIPROC:
      printf("HIPROC           ");
      break;
    case SHT_LOUSER:
      printf("LOUSER           ");
      break;
    case SHT_HIUSER:
      printf("HIUSER           ");
      break;
    default :
      printf("Cas non traité   ");
  }
}

int main(int argc, char *argv[]) {
  FILE * elfFile;

  Elf32_Ehdr header;
  Elf32_Shdr section;

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
        printf("Il  y a %d sections dans le fichier.", byteshift16(header.e_shnum));
        headtext();
        for (int i = 0; i < byteshift16(header.e_shnum); i++)
          {
            if(i > 20) exit(0);
          const char* name = "";

          fseek(elfFile, byteshift32(header.e_shoff) + i * sizeof(section), SEEK_SET);
          fread(&section, 1, sizeof(section), elfFile);

          // print section name
          printf("[%2u] ", i);
          //if (sectHdr.sh_name)
          //  name = SectNames + sectHdr.sh_name;

          char nom_section[255];



          get_section_name(elfFile,header,section,nom_section);
          printf("%s ",nom_section);
          align(nom_section, 16);

          typeToString(byteshift32(section.sh_type));
          printf("%016x ", byteshift32(section.sh_addr));
          printf("%08x\n     ", byteshift32(section.sh_offset));
          printf("%016x ", byteshift32(section.sh_size));
          printf("%016x ", byteshift32(section.sh_entsize));
          flagsToString(byteshift32(section.sh_flags));
          printf("%u\t", byteshift32(section.sh_link));
          printf("%u\t", byteshift32(section.sh_info));
          printf("%u\n", byteshift32(section.sh_addralign));
        }
        foottext();
      }
      fclose(elfFile);
    }
  }
}
