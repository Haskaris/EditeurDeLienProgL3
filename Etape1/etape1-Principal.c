#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>
#include "etape1-Principal.h"

int main(int argc, char *argv[]) {

	FILE * elfFile;
	Elf32_Ehdr header;
	int choix = -1;
	int numSection = -1;
	if (argc != 4) {
		printf("Utilisation : %s <ELF_FILE> AffichageVoulu NumSectionVoulue\n", argv[0]);
		exit(1);
	} else {
		elfFile = fopen(argv[1], "r");
		choix = (int)(*argv[2]) - '0';;
		numSection = (int)(*argv[3]) - '0';
		if (elfFile == NULL) {
			printf("Erreur lors de l'ouverture du fichier.\n");
		} else {
			// Lit l'entête
			fread(&header, sizeof(header), 1, elfFile);
			if (isbigendian(header)) {
				inversion_Header(&header);
			}
			// Est-ce que c'est bien un fichier elf ?
			if (memcmp(header.e_ident, ELFMAG, SELFMAG) == 0) {
				switch(choix){
					case 1:
						affichage_Entete_Fichier_ELF(header);
						break;
					case 2:
						affichage_Table_Sections(elfFile, header);
						break;
					case 3:
						affichage_Contenu_Section(elfFile, header, numSection);
						break;
					case 4:
						affichage_Table_Des_Symbole(elfFile, header);
						break;
					case 5:
						affichage_Table_Reimplantation(elfFile, header);
						break;
					default:
						printf("Numero invalide ");
				}
			} else {
				printf("Le fichier passé en argument n'est pas un fichier ELF\n");
			}
			fclose(elfFile);
		}
	}
}
