//Inclure les autres du .h ?
#include "etape1-2.h"

void headtext(){
	printf("En-têtes de section :\n[Nr] Nom\t\tType\t\tAdr\t Décala.Taille ES Fan Ln Inf Al\n");
}

void foottext(){
	printf("Clé des fanions :\n	W (écriture), A (allocation), X (exécution), M ("
		"fusion), S (chaînes), I (info),\n	L (ordre des liens), O (traitement supp"
		"lémentaire par l'OS requis), G (groupe),\n	T (TLS), C (compressé), x (inc"
		"onnu), o (spécifique à l'OS), E (exclu),\n	l (grand), p (processor specif"
		"ic)\n");
}

void align(char* str, int indent) {
	int length = strlen(str);
	if(length > indent) return;
	for(int i = length; i < indent; i++) {
		printf(" ");
	}
}

void flagsToString(uint32_t flag) {
	int i=0;
	if((flag&SHF_WRITE) != 0) {
		//Ajouter W
		printf("W");
	} else i++;
	if((flag&SHF_ALLOC) != 0) {
		//Ajouter A
		printf("A");
	} else i++;
	if((flag&SHF_EXECINSTR) != 0) {
		//Ajouter X
		printf("X");
	} else i++;
	if((flag&16) != 0) {
		//Ajouter M
		printf("M");
	} else i++;
	if((flag&32) != 0) {
		//Ajouter S
		printf("S");
	} else i++;
	if((flag&64) != 0) {
		//Ajouter I
		printf("I");
	} else i++;
	if((flag&128) != 0) {
		//Ajouter L
		printf("L");
	} else i++;
	if((flag&256) != 0) {
		//Ajouter O
		printf("O");
	} else i++;
	for (int count = 0; count < i; count++) printf(" ");
	printf(" ");
}

void typeToString(uint32_t type) {
	switch (type) {
		case SHT_NULL:
			printf("NULL\t\t");
			break;
		case SHT_PROGBITS:
			printf("PROGBITS\t");
			break;
		case SHT_SYMTAB:
			printf("SYMTAB\t\t");
			break;
		case SHT_STRTAB:
			printf("STRTAB\t\t");
			break;
		case SHT_RELA:
			printf("RELA\t\t");
			break;
		case SHT_HASH:
			printf("HASH\t\t");
			break;
		case SHT_DYNAMIC:
			printf("DYNAMIC\t\t");
			break;
		case SHT_NOTE:
			printf("NOTE\t\t");
			break;
		case SHT_NOBITS:
			printf("NOBITS\t\t");
			break;
		case SHT_REL:
			printf("REL\t\t");
			break;
		case SHT_SHLIB:
			printf("SHLIB\t\t");
			break;
		case SHT_DYNSYM:
			printf("DYNSYM\t\t");
			break;
		case SHT_LOPROC:
			printf("LOPROC\t\t");
			break;
		case SHT_HIPROC:
			printf("HIPROC\t\t");
			break;
		case SHT_LOUSER:
			printf("LOUSER\t\t");
			break;
		case SHT_HIUSER:
			printf("HIUSER\t\t");
			break;
		case 1879048195:
			printf("ARM_ATTRIBUTES\t");
			break;
		default :
			printf("Err : %d\t", type);
	}
}

void affichage_Table_Sections(FILE *elfFile, Elf32_Ehdr header) {
	Elf32_Shdr section;

	// read all section headers
	printf("Il y a %d sections dans le fichier\n.", header.e_shnum);
	headtext();
	for (int i = 0; i < header.e_shnum; i++) {
		const char* name = "";

		fseek(elfFile, DECALAGE(header, i), SEEK_SET);
		litEtInverse_Section(elfFile, header, &section);

		// print section name
		printf("[%2u] ", i);

		char* nom_section = NULL;
		nom_section = get_section_name(elfFile, header, section);
		printf("%s ",nom_section);
		align(nom_section, 18);

		typeToString(section.sh_type);
		printf("%08x ", section.sh_addr);
		printf("%06x ", section.sh_offset);
		printf("%06x ", section.sh_size);
		printf("%02x ", section.sh_entsize);
		flagsToString(section.sh_flags);
		printf("%u ", section.sh_link);
		printf("%u ", section.sh_info);
		printf("%u\n", section.sh_addralign);
	}
	foottext();
}
