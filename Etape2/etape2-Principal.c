#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>
#include "etape2-Principal.h"

int test_Elf_File(Elf32_Ehdr header, Elf32_Ehdr header1){
	// Est-ce que c'est bien un fichier elf ?
	if ((memcmp(header.TAB_OCTET_INTERPRETATION_FICHIER, ELFMAG, SELFMAG) == 0) && \
 			(memcmp(header1.TAB_OCTET_INTERPRETATION_FICHIER, ELFMAG, SELFMAG) == 0)) {
		return 1;			
	} else {
		printf("Le fichier passe en argument n'est pas un fichier ELF\n");
		return 0;
	}
}

int main(int argc, char *argv[]) {

	FILE * elfFileSource;
	FILE * elfFileSource1;

	FILE * elfFileDest;

  	Elf32_Ehdr header;
  	Elf32_Ehdr header1;

  	char buff[255];
  	if (argc != 4) {
  		printf("Utilisation : %s <ELF_FILE_SOURCE> <ELF_FILE_SOURCE> <ELF_FILE_DESTINATION>\n", argv[0]);
    		exit(1);
  	}
  	else {
    		elfFileSource = fopen(argv[1], "r");
    		if (elfFileSource == NULL) {
      			printf("Erreur lors de l'ouverture du fichier.\n");
    		}
    		else {
			elfFileSource1 = fopen(argv[2], "r");
    			if (elfFileSource1 == NULL) {
      				printf("Erreur lors de l'ouverture du fichier.\n");
    			}
    			else {
				elfFileDest = fopen(argv[3], "w");
    				if (elfFileDest == NULL) {
      					printf("Erreur lors de l'ouverture du fichier.\n");
    				}
    				else {
		      			// Lit l'entête
		      			fread(&header, sizeof(header), 1, elfFileSource);
		      			if (isbigendian(header)){
						inversion_Header(&header);	
		      			} 
					fread(&header1, sizeof(header), 1, elfFileSource1);
		      			if (isbigendian(header1)){
						inversion_Header(&header1);	
		      			} 

		      			if (test_Elf_File(header, header1)){
						
					}
					fclose(elfFileDest);
				}
				fclose(elfFileSource1);
			}
	      		fclose(elfFile);
    		}
  	}
}
