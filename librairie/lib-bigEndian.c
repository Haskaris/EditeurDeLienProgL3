#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>
#include "lib-bigEndian.h"

int isbigendian(Elf32_Ehdr h){
	switch (h.e_ident[EI_DATA]) {
		case ELFDATA2LSB:
  			return 0;
		case ELFDATA2MSB:
  			return 1;
		default://ELFDATANONE
  			printf("isbigEndian problème pas d'encodage\n");
				exit(1);
  	}
}

uint16_t byteshift16(uint16_t n, int bigEndian){
    if (bigEndian){
        return ((n>>8)&0xff) | ((n<<8)&0xff00);
    }
    else{
        return n;
    }
}

uint32_t byteshift32(uint32_t n, int bigEndian) {
    if (bigEndian){
          return ((n>>24)&0xff) | ((n<<8)&0xff0000) | ((n>>8)&0xff00) | ((n<<24)&0xff000000);
    }
    else{
        return n;
    }
}

uint64_t byteshift64(uint64_t n, int bigEndian) {
    if (bigEndian){
          return ((n>>24)&0xff0000) | ((n<<8)&0xff00000000) | ((n>>8)&0xff000000) | ((n<<24)&0xff0000000000) | ((n>>40)&0xff00) | ((n<<40)&0xff000000000000) | ((n<<56)&0xff00000000000000) | ((n>>56)&0xff);
    }
    else{
        return n;
    }
}
