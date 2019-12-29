//Inclure les autres du .h ?
#include "etape1-4.h"

void print_symbol_table32(FILE* elfFile, Elf32_Ehdr header, Elf32_Shdr section, uint32_t indice){
	Elf32_Sym table_symbole;
	uint32_t i;
	uint32_t nombre_symbol;

	//accéde à la table des symboles
	fseek(elfFile, header.DECALAGE_TABLE_ENTETE_SECTIONS + indice * sizeof(section), SEEK_SET);
	fread(&section, 1, sizeof(section), elfFile);

	//Inversion de la table si elle n'est pas en litle Endian
	if (isbigendian(header)) {
		inversion_Sections(&section);
	}

	// calcul du nombre de symbole
	nombre_symbol = section.TAILLE_SECTION / section.TAILLE_TABLE_ENTREE_FIXE;
	fseek(elfFile, section.DECALAGE_DEBUT_FICHIER, SEEK_SET);

	printf("La table de symboles << .symtab >> contient %d entrées :\n",
		nombre_symbol);
	printf("	Num:	 Valeur		 Tail	Type	Lien	Vis		 Ndx Nom\n");

	//on affiche les informations du symbole
	for(i=0; i< nombre_symbol; i++) {
		fread(&table_symbole, 1, sizeof(table_symbole), elfFile);
		if (isbigendian(header)){
			insersion_Table_Symbole(&table_symbole);
		}
		printf("	 %d:",i);
		printf("	%08x ", table_symbole.VALEUR_SYMBOLE);
		printf(" %u ", table_symbole.TAILLE_SYMBOLE);
		switch (ELF32_ST_TYPE(table_symbole.TYPE_SYMBOLE_ET_ATTRIBUT_LIAISON)) {
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
		switch(ELF32_ST_BIND(table_symbole.TYPE_SYMBOLE_ET_ATTRIBUT_LIAISON)) {
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
		switch(ELF32_ST_VISIBILITY(table_symbole.VISIBILITE_SYMBOLE)) {
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
		printf(" %x	", table_symbole.INDICE_TABLE_SECTION);
		printf(" %02x\n", table_symbole.NOM_SYMBOLE);
	}
}


void affichage_Table_Des_Symbole(FILE *elfFile, Elf32_Ehdr header) {
	uint32_t i;
	Elf32_Shdr section;
	//parcours la table des entêtes et
	//cherche la section contenant la table des symboles (SHT_SYMTAB)
	for(i=0; i < header.NOMBRE_ENTREE_TABLE_SECTIONS; i++) {
		fseek(elfFile, header.DECALAGE_TABLE_ENTETE_SECTIONS + i * sizeof(section), SEEK_SET);
		fread(&section, 1, sizeof(section), elfFile);
		if (isbigendian(header)) {
			inversion_Sections(&section);
		}
		if (section.CONTENU_SEMANTIQUE == SHT_SYMTAB) {
			print_symbol_table32(elfFile, header, section, i);
			break;
		}
	}
}
