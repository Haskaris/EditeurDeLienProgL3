#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>
#include "etape2-Principal.h"

int main(int argc, char *argv[]) {

	FILE * elfFile1;
	FILE * elfFile2;
	FILE * outputFile;
	int choix = -1;
	if (argc != 4) {
		printf("Utilisation : %s <ELF_FILE1> <ELF_FILE2> <OUTPUT_FILE> <choixNumero>\n", argv[0]);
		exit(1);
	} else {
		elfFile1 = fopen(argv[1], "r");
		elfFile2 = fopen(argv[2], "r");
		outputFile = fopen(argv[3], "w");
		choix = (int)(*argv[4]) - '0';
		if (elfFile1 == NULL || elfFile2 == NULL) {
			printf("Erreur lors de l'ouverture du fichier.\n");
		} else {
			switch(choix){
				case 1:
					fusion_section(elfFile1, elfFile2, outputFile);
					break;
				case 2:
					fusion_symbole(elfFile1, elfFile2, outputFile);
					break;
				case 3:
					fusion_reimplementaton(elfFile1, elfFile2, outputFile);
					break;
				case 4:
					printf("Etape 9 non implémenté");
					break;
				default:
					printf("Numero invalide ");
			}
			fclose(elfFile1);
			fclose(elfFile2);
			fclose(outputFile);
		}
	}
}
