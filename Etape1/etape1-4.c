//Inclure les autres du .h ?
#include "etape1-4.h"

void print_symbol_table32(FILE* elfFile, Elf32_Ehdr header, Elf32_Shdr section, uint32_t indice) {
	Elf32_Sym table_symbole;
	uint32_t i;
	uint32_t nombre_symbol;
	uint32_t indice_table_chaine=section.sh_link;
	//accéde à la table des symboles
	fseek(elfFile, header.e_shoff + indice * sizeof(section), SEEK_SET);

	litEtInverse_Section(elfFile, header, &section);

	// calcul du nombre de symbole
	nombre_symbol = section.sh_size / section.sh_entsize;
	fseek(elfFile, section.sh_offset, SEEK_SET);

	printf("La table de symboles << .symtab >> contient %d entrées :\n",
		nombre_symbol);
	printf("	Num:	Valeur	Tail	Type	Lien	Vis	 Ndx	Nom\n");

	//on affiche les informations du symbole
	for(i=0; i< nombre_symbol; i++) {
		litEtInverse_TalbeSymbole(elfFile, header, &table_symbole);

		printf("	 %d:",i);
		printf("	%08x ", table_symbole.st_value);
		printf(" %u ", table_symbole.st_size);
		switch (ELF32_ST_TYPE(table_symbole.st_info)) {
			case 0:
				printf("	NOTYPE	");
				break;
			case 1:
				printf("	OBJT	");
				break;
			case 2:
				printf("	FUNC	");
				break;
			case 3:
				printf("	SECTION	");
				break;
			case 4:
				printf("	FILE	");
				break;
			default:
				printf(" Cas non géré fichier enteteEtape4.c fonction print_symbol_tabl"
								"e32 switch ELF32_ST_TYPE ");
		}
		switch(ELF32_ST_BIND(table_symbole.st_info)) {
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
				printf("	NUM ");
				break;
			default:
				printf(" Cas non géré fichier enteteEtape4.c fonction print_symbol_tabl"
					"e32 switch ELF32_ST_BIND ");
				break;
		}
		switch(ELF32_ST_VISIBILITY(table_symbole.st_other)) {
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

		printf(" %x	", table_symbole.st_shndx);
		printf(" %s\n", get_symbol_name(elfFile, header, indice_table_chaine, table_symbole.st_name));
	}
}

void affichage_Table_Des_Symbole(FILE *elfFile, Elf32_Ehdr header) {
	uint32_t i;
	Elf32_Shdr section;
	//parcours la table des entêtes et
	//cherche la section contenant la table des symboles (SHT_SYMTAB)
	for(i=0; i < header.e_shnum; i++) {
		fseek(elfFile, header.e_shoff + i * sizeof(section), SEEK_SET);
		litEtInverse_Section(elfFile, header, &section);

		if (section.sh_type == SHT_SYMTAB) {
			print_symbol_table32(elfFile, header, section, i);
			break;
		}
	}
}
