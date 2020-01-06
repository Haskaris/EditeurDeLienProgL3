#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>
#include "etape2-6.h"

void fusion_section() {}

int main(int argc, char *argv[]) {
  FILE * fileToJoin1;
  FILE * fileToJoin2;
  FILE * outFile;

  /*Elf32_Ehdr header;
  Elf32_Shdr section;
  int bigEndian = 0;
  char buff[255];*/


  if (argc != 4) {
    printf("Utilisation : %s <ELF_FILE1> <ELF_FILE2> <OUT_FILE>\n", argv[0]);
    exit(1);
  }
  else {
    //Main
  }
}
