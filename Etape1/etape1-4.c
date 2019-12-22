//Inclure les autres du .h ?
#include "etape1-4.h"

void print_symbol_table32(FILE* elfFile, Elf32_Ehdr eh, Elf32_Shdr sh_table,
														uint32_t indice, int bigEndian){
	Elf32_Sym sym_tbl;
	uint32_t i, nombre_symbol;
	//accéde à la table des symboles
	fseek(elfFile, eh.e_shoff + indice * sizeof(sh_table), SEEK_SET);
	fread(&sh_table, 1, sizeof(sh_table), elfFile);

	// calcul du nombre de symbole
	nombre_symbol = byteshift32(sh_table.sh_size, bigEndian) /
										byteshift32(sh_table.sh_entsize, bigEndian);
	fseek(elfFile, byteshift32(sh_table.sh_offset, bigEndian), SEEK_SET);

	printf("La table de symboles << .symtab >> contient %d entrées :\n",
		nombre_symbol);
	printf("  Num:   Valeur     Tail  Type  Lien  Vis     Ndx Nom\n");

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
			default:
				printf(" Cas non géré fichier enteteEtape4.c fonction print_symbol_tabl"
				"e32 switch ELF32_ST_TYPE ");
				break;
		}
		switch(ELF32_ST_BIND(sym_tbl.st_info)) {
			case 0:
				printf(" LOCAL ");
				break;
			case 1:
				printf(" GLOBAL ");
				break;
			case 2:
				printf(" WEAK ");
				break;
			case 3:
				printf("  NUM ");
				break;
			default:
				printf(" Cas non géré fichier enteteEtape4.c fonction print_symbol_tabl"
				"e32 switch ELF32_ST_BIND ");
				break;
		}
		switch(ELF32_ST_VISIBILITY(sym_tbl.st_other)) {
			case 0:
				printf(" DEFAULT ");
				break;
			case 1:
				printf(" INTERNAL ");
				break;
			case 2:
				printf(" HIDDEN ");
				break;
			case 3:
				printf(" PROTECTED ");
				break;
			default:
				printf(" Cas non géré fichier enteteEtape4.c fonction print_symbol_tabl"
					"e64 switch ELF64_ST_VISIBILITY ");
				break;
		}
		printf(" %x  ", byteshift16(sym_tbl.st_shndx, bigEndian));
		printf(" %02x\n", byteshift32(sym_tbl.st_name, bigEndian));
	}
}

void affichage_Table_Des_Symbole(FILE *elfFile, Elf32_Ehdr header,
																	int bigEndian) {
	uint32_t i;
	Elf32_Shdr section;
	//parcours la table des entêtes et cherche la section contenant
	//la table des symboles (SHT_SYMTAB)
	for(i=0; i<byteshift16(header.e_shnum, bigEndian); i++) {
		fseek(elfFile, byteshift32(header.e_shoff, bigEndian) + i * sizeof(section),
					SEEK_SET);
		fread(&section, 1, sizeof(section), elfFile);
		if (byteshift32(section.sh_type, bigEndian)==SHT_SYMTAB) {
			print_symbol_table32(elfFile, header, section, i, bigEndian);
			break;
		}
	}
}
