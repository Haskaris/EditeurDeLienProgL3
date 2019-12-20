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


int main(int argc, char* argv[]){
    FILE * elfFile1;
    FILE * elfFile2;
    FILE * outputFile;
    FILE * tempFile;

    Elf32_Ehdr header1,header2,headerOutput;
    Elf32_Shdr section1,section2,sectionOut;

    char nom_section1[255],nom_section2[255];
    if (argc != 4) {
      printf("Utilisation : %s <ELF_FILE1> <ELF_FILE2> <OUTPUT_FILE>\n", argv[0]);
      exit(1);
    }
    else {
      elfFile1 = fopen(argv[1], "r");
      elfFile2 = fopen(argv[2], "r");
      outputFile = fopen(argv[3], "w");
      tempFile = fopen("tempFileSectionTable","w");
      if (elfFile1 == NULL || elfFile2==NULL || outputFile==NULL) {
        printf("Erreur d'ouverture d'un fichier.\n");
      }
      else {
          fread(&header1, sizeof(header1),1, elfFile1);
          fread(&header2, sizeof(header2),1, elfFile2);
          // check so its really an elf file
          if (memcmp(header1.e_ident, ELFMAG, SELFMAG) == 0 && memcmp(header2.e_ident, ELFMAG, SELFMAG) == 0 ) {
              fwrite(&header1,sizeof(header1),1,outputFile);
              for (int i=0; i<byteshift16(header1.e_shnum);i++){
                  fseek(elfFile1, byteshift32(header1.e_shoff) + i * byteshift16(header1.e_shentsize), SEEK_SET);
                  fread(&section1, 1, sizeof(section2), elfFile1);
                  get_section_name(elfFile1,header1,section1,nom_section1);
                  for (int j=0;j<byteshift16(header2.e_shnum);j++){
                      fseek(elfFile2, byteshift32(header2.e_shoff) + j * byteshift16(header2.e_shentsize), SEEK_SET);
                      fread(&section2, 1, sizeof(section2), elfFile2);
                      get_section_name(elfFile2,header2,section2,nom_section2);
                      if(!strcmp(nom_section1,nom_section2)){
                          fprintf(tempFile,"%s",nom_section1);
                      }
                  }
              }
          }
      }
      fclose(elfFile1);
      fclose(elfFile2);
      fclose(outputFile);
      fclose(tempFile);
      //remove("tempFileSectionTable");
  }

}
