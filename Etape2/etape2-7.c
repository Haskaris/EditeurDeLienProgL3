#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>
#include "etape2-7.h"


void fusionSymbole32(int bigEndian){

	return;
}

void lectureTableSymbol32(int bigEndian){

	return;
}

Elf32_Sym copie_Sym_Tab(Elf32_Sym element){
	Elf32_Sym res = malloc(sizeof(Elf32_Sym));
	res.
	return res;
}


Elf32_Sym* print_symbol_table32(FILE* elfFile, Elf32_Ehdr eh, Elf32_Shdr sh_table, uint32_t indice, int bigEndian){	
	Elf32_Sym sym_tbl;
	uint32_t i, nombre_symbol;
	//accéde à la table des symboles
	fseek(elfFile, eh.e_shoff + indice * sizeof(sh_table), SEEK_SET);
	fread(&sh_table, 1, sizeof(sh_table), elfFile);
  	//printf("Name : %s\n",get_section_names(elfFile, sh_table, bigEndian) + sh_table.sh_name);
	
	// calcul du nombre de symbole   	
	nombre_symbol = byteshift32(sh_table.sh_size, bigEndian) / byteshift32(sh_table.sh_entsize, bigEndian);
  	fseek(elfFile, byteshift32(sh_table.sh_offset, bigEndian), SEEK_SET);

	printf("La table de symboles << .symtab >> contient %d entrées :\n", nombre_symbol);
	printf("  Num:   Valeur     Tail  Type  Lien  Vis     Ndx Nom\n");
	
	//resultat à retourner
	Elf32_Sym *res = malloc(sizeof(Elf32_Sym) * nombre_symbol);
        
	//on affiche les informations du symbole
	for(i=0; i< nombre_symbol; i++) {
		fread(&sym_tbl, 1, sizeof(sym_tbl), elfFile);
		printf("   %d:",i);
		printf("  %08x ", byteshift32(sym_tbl.st_value, bigEndian));
		printf(" %u ", byteshift32(sym_tbl.st_size, bigEndian));
		switch (ELF32_ST_TYPE(sym_tbl.st_info)) {
        		case 0:
            			printf("  NOTYPE  ");
            			break;
       			case 1:
            			printf("  OBJT  ");
            			break;
        		case 2:
            			printf("  FUNC  ");
            			break;
        		case 3:
            			printf("  SECTION  ");
            			break;
        		case 4:
            			printf("  FILE  ");
            			break;

       			default: printf(" Cas non géré fichier enteteEtape4.c fonction print_symbol_table32 switch ELF32_ST_TYPE ");
            			break;
    		}
		switch(ELF32_ST_BIND(sym_tbl.st_info))
    		{
       	 		case 0: printf(" LOCAL ");
            			break;
        		case 1: printf(" GLOBAL ");
            			break;
        		case 2: printf(" WEAK ");
            			break;
        		case 3: printf("  NUM ");
            			break;
        		default: printf(" Cas non géré fichier enteteEtape4.c fonction print_symbol_table32 switch ELF32_ST_BIND ");
        			break;
    		}
		//printf(" %x  ", sym_tbl.st_other);
		switch(ELF32_ST_VISIBILITY(sym_tbl.st_other)){
			case 0: printf(" DEFAULT ");
				break;
			case 1: printf(" INTERNAL ");
				break;
			case 2: printf(" HIDDEN ");
				break;
			case 3: printf(" PROTECTED ");
				break;
			default: printf(" Cas non géré fichier enteteEtape4.c fonction print_symbol_table64 switch ELF64_ST_VISIBILITY ");
				break;
		}
    		printf(" %x  ", byteshift16(sym_tbl.st_shndx, bigEndian));
    		printf(" %02x\n", byteshift32(sym_tbl.st_name, bigEndian));	
	}
	return res;
}

Elf32_Sym* stockage_table_symbol_init32(FILE* elfFile, Elf32_Ehdr eh, int bigEndian){
	uint32_t i;
	Elf32_Shdr section;

	//parcours la table des entêtes et 
	//cherche la section contenant la table des symboles (SHT_SYMTAB)		
	for(i=0; i<byteshift16(eh.e_shnum, bigEndian); i++) {
		fseek(elfFile, byteshift32(eh.e_shoff, bigEndian) + i * sizeof(section), SEEK_SET);
         	fread(&section, 1, sizeof(section), elfFile);
		if (byteshift32(section.sh_type, bigEndian)==SHT_SYMTAB) {
			return print_symbol_table32(elfFile, eh, section, i, bigEndian);
		}
	}
}

int main(int argc, char *argv[]){
	FILE * elfFile;
	FILE * elfFile2;

  	Elf32_Ehdr header;
	Elf32_Ehdr header2;
	Elf32_Sym tableFinal = malloc(sizeof(Elf32_Sym));
	
	//Format des fichier 1 et 2
	int bigEndian = 0;
	int bigEndian2 = 0;

  	if (argc != 3) {
    		printf("Utilisation : %s <ELF_FILE> <ELF_FILE>\n", argv[0]);
    		exit(1);
  	}
  	else {
  	  	elfFile = fopen(argv[1], "r");
    		if (elfFile == NULL) {
     	 		printf("Erreur d'ouverture du fichier 1.\n");
    		}
    		else {
			elfFile2 = fopen(argv[2], "r");
    			if (elfFile2 == NULL) {
     	 			printf("Erreur d'ouverture du fichier 2.\n");
    			}
    			else {
      				//lit l'entête du premier fichier
      				fread(&header, 1, sizeof(header), elfFile);
				//lit l'entête du deuxième fichier
				fread(&header2, 1, sizeof(header2), elfFile2);

				if (isbigendian(header)){
					bigEndian = 1; 
				}
				if (isbigendian(header2){
					bigEndian2 = 1;
				}
				
				

				Elf32_Sym *Sym_tab = stockage_table_symbol_init32(elfFile, header, bigEndian);
				ELf32_Sym *Sym_tab2 = stockage_table_symbol_init32(elfFile2, header2, bigEndian2);

				//On ferme les 2 fichier ELF
				fclose(elfFile);
				fclose(elfFile2);
			}
    		}
  	}
	return 0;
}
