#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>
#include "etape1-4.h"

char* get_section_names(FILE *elfFile, Elf32_Shdr sh_table)
{
    //Lire les noms des sections
    char *nom = malloc(sh_table.sh_size);
    fseek(elfFile, sh_table.sh_offset, SEEK_SET);
    fread(nom, 1, sh_table.sh_size, elfFile);
    return nom;
}

void print_symbol_table32(FILE* elfFile, Elf32_Ehdr eh, Elf32_Shdr sh_table, uint32_t indice){	
	Elf32_Sym sym_tbl;
	uint32_t i, nombre_symbol;
	//accéde à la table des symboles
	fseek(elfFile, eh.e_shoff + indice * sizeof(sh_table), SEEK_SET);
	fread(&sh_table, 1, sizeof(sh_table), elfFile);
  	//printf("Name : %s\n",get_section_names(elfFile, sh_table, bigEndian) + sh_table.sh_name);
	if (isbigendian(eh)){
		inversion_Sections(&sh_table);
    	}
	// calcul du nombre de symbole   	
	nombre_symbol = sh_table.sh_size / sh_table.sh_entsize;
  	fseek(elfFile, sh_table.sh_offset, SEEK_SET);

	printf("La table de symboles << .symtab >> contient %d entrées :\n", nombre_symbol);
	printf("  Num:   Valeur     Tail  Type  Lien  Vis     Ndx Nom\n");
        
	//on affiche les informations du symbole
	for(i=0; i< nombre_symbol; i++) {
		fread(&sym_tbl, 1, sizeof(sym_tbl), elfFile);
		if (isbigendian(eh)){
			insersion_Table_Symbole(&sym_tbl);
		}
		printf("   %d:",i);
		printf("  %08x ", sym_tbl.st_value);
		printf(" %u ", sym_tbl.st_size);
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
    		printf(" %x  ", sym_tbl.st_shndx);
    		printf(" %02x\n", sym_tbl.st_name);	
	}
}


void affichage_Table_Des_Symbole(FILE *elfFile, Elf32_Ehdr header) {
	uint32_t i;
	Elf32_Shdr section;
	//parcours la table des entêtes et 
	//cherche la section contenant la table des symboles (SHT_SYMTAB)		
	for(i=0; i<header.e_shnum; i++) {
		fseek(elfFile, header.e_shoff + i * sizeof(section), SEEK_SET);
         	fread(&section, 1, sizeof(section), elfFile);
		if (isbigendian(header)){
			inversion_Sections(&section);
		}
		if (section.sh_type ==SHT_SYMTAB) {
			print_symbol_table32(elfFile, header, section, i);
			break;
		}
	}
}
