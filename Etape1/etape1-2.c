//Inclure les autres du .h ?
#include "etape1-2.h"

void headtext(){
	printf("Il y a 11 en-têtes de section, débutant à l'adresse de décalage 0x7b8"
		":\n\nEn-têtes de section :\n[Nr] Nom               Type             Adress"
		"e           Décalage\n       Taille            TaillEntrée   Fanion  Lien "
		" Info  Alignement\n");
}

void foottext(){
	printf("Clé des fanions :\n  W (écriture), A (allocation), X (exécution), M ("
		"fusion), S (chaînes), I (info),\n  L (ordre des liens), O (traitement supp"
		"lémentaire par l'OS requis), G (groupe),\n  T (TLS), C (compressé), x (inc"
		"onnu), o (spécifique à l'OS), E (exclu),\n  l (grand), p (processor specif"
		"ic)\n");
}

void align(char* str, int indent) {
	int length = strlen(str);
	if(length >= indent) return;
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
		//Ajouter I
		printf("L");
	} else i++;
	if((flag&256) != 0) {
		//Ajouter I
		printf("O");
	} else i++;
	for (int count = 0; count < i; count++) printf(" ");
	printf("\t");
}

void typeToString(uint32_t type) {
	switch (type) {
		case SHT_NULL:
			printf("NULL             ");
			break;
		case SHT_PROGBITS:
			printf("PROGBITS         ");
			break;
		case SHT_SYMTAB:
			printf("SYMTAB           ");
			break;
		case SHT_STRTAB:
			printf("STRTAB           ");
			break;
		case SHT_RELA:
			printf("RELA             ");
			break;
		case SHT_HASH:
			printf("HASH             ");
			break;
		case SHT_DYNAMIC:
			printf("DYNAMIC          ");
			break;
		case SHT_NOTE:
			printf("NOTE             ");
			break;
		case SHT_NOBITS:
			printf("NOBITS           ");
			break;
		case SHT_REL:
			printf("REL              ");
			break;
		case SHT_SHLIB:
			printf("SHLIB            ");
			break;
		case SHT_DYNSYM:
			printf("DYNSYM           ");
			break;
		case SHT_LOPROC:
			printf("LOPROC           ");
			break;
		case SHT_HIPROC:
			printf("HIPROC           ");
			break;
		case SHT_LOUSER:
			printf("LOUSER           ");
			break;
		case SHT_HIUSER:
			printf("HIUSER           ");
			break;
		case 1879048195:
			printf("ARM_ATTRIBUTES   ");
			break;
		default :
			printf("Err : %d     ", type);
	}
}

void affichage_Table_Sections(FILE *elfFile, Elf32_Ehdr header, int bigEndian) {
	Elf32_Shdr section;
	
	// read all section headers
	printf("Il y a %d sections dans le fichier.", byteshift16(header.e_shnum, bigEndian));
	headtext();
	for (int i = 0; i < byteshift16(header.e_shnum, bigEndian); i++){
		const char* name = "";

		fseek(elfFile, byteshift32(header.e_shoff, bigEndian) + i * sizeof(section), SEEK_SET);
		fread(&section, 1, sizeof(section), elfFile);

		printf("[%2u] ", i);

		//Penser à FREE nom_section
		char* nom_section = get_section_name(elfFile, header, section, bigEndian);
		printf("%s ",nom_section);
		align(nom_section, 16);
		free(nom_section);

		typeToString(byteshift32(section.sh_type, bigEndian));
		printf("%016x ", byteshift32(section.sh_addr, bigEndian));
		printf("%08x\n     ", byteshift32(section.sh_offset, bigEndian));
		printf("%016x ", byteshift32(section.sh_size, bigEndian));
		printf("%016x ", byteshift32(section.sh_entsize, bigEndian));
		flagsToString(byteshift32(section.sh_flags, bigEndian));
		printf("%u\t", byteshift32(section.sh_link, bigEndian));
		printf("%u\t", byteshift32(section.sh_info, bigEndian));
		printf("%u\n", byteshift32(section.sh_addralign, bigEndian));
	}
	foottext();
}
