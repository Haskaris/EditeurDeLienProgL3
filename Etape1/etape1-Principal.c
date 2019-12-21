#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>
#include "etape1-Principal.h"

int main(int argc, char *argv[]) {

	FILE * elfFile;
  	Elf32_Ehdr header;
  	char buff[255];
  	int bigEndian = 0;
	int choix = -1;
	int numSection = -1;
  	if (argc != 4) {
  		printf("Utilisation : %s <ELF_FILE> AffichageVoulu NumSectionVoulue\n", argv[0]);
    		exit(1);
  	}
  	else {
    		elfFile = fopen(argv[1], "r");
		choix = (int)(*argv[2]) - '0';;
		numSection = (int)(*argv[3]) - '0';
    		if (elfFile == NULL) {
      			printf("Erreur lors de l'ouverture du fichier.\n");
    		}
    		else {
      			// Lit l'entête
      			fread(&header, sizeof(header), 1, elfFile);
      			if (isbigendian(header)){
				bigEndian = 1;
				//faire changement si litle ou big
      			} 

      			// Est-ce que c'est bien un fichier elf ?
      			if (memcmp(header.e_ident, ELFMAG, SELFMAG) == 0) {
				//int numSection = 0;
				switch(choix){
					case 1: affichage_Entete_Fichier_ELF(header, bigEndian);
						break;
					case 2: affichage_Table_Sections(elfFile, header, bigEndian);
						break;
					case 3:	//printf("Saisir le nombre de la section voulue ");
						//scanf("%d", &numSection);
						affichage_Contenu_Section(elfFile, header, bigEndian, numSection);
						break;
					case 4: affichage_Table_Des_Symbole(elfFile, header, bigEndian);
						break;
					case 5: affichage_Table_Reimplantation(elfFile, header, bigEndian);
						break;
					default:
						printf("Numero invalide ");
						break;
				}		
      			}
			else {
				printf("Le fichier passe en argument n'est pas un fichier ELF\n");
			}
      			fclose(elfFile);
    		}
  	}
}
