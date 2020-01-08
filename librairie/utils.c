#include "lib-bigEndian.h"
#include "utils.h"

#define DECALAGE(header, i) header.DECALAGE_TABLE_ENTETE_SECTIONS + i * sizeof(Elf32_Shdr)
#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))

void litEtInverse_Section(FILE* elfFile, Elf32_Ehdr header, Elf32_Shdr* section) {
	fread(section, 1, sizeof(Elf32_Shdr), elfFile);

	if (isbigendian(header)) {
		inversion_Sections(section);
	}
}

void litEtInverse_TalbeSymbole(FILE* elfFile, Elf32_Ehdr header, Elf32_Sym* table_symbole) {
	fread(table_symbole, 1, sizeof(Elf32_Sym), elfFile);

	if (isbigendian(header)) {
		insersion_Table_Symbole(table_symbole);
	}
}

void litEtInverse_Header(FILE* elfFile, Elf32_Ehdr* header) {
	fread(header, sizeof(Elf32_Ehdr), 1, elfFile);

	if (isbigendian(*header)) {
		inversion_Header(header);
	}
}

void litEtInverse_Rel(FILE* elfFile, Elf32_Ehdr header, Elf32_Rel* rel) {
	fread(rel, sizeof(Elf32_Rel), 1, elfFile);

	if (isbigendian(header)) {
		inversion_Relation_Sans_Additif(rel);
	}
}

void litEtInverse_Rela(FILE* elfFile, Elf32_Ehdr header, Elf32_Rela* rela) {
	fread(rela, sizeof(Elf32_Rela), 1, elfFile);

	if (isbigendian(header)) {
		inversion_Relation_Additif(rela);
	}
}

/*
* Il faut penser à FREE !
*/
char* get_section_name(FILE* elfFile, Elf32_Ehdr header, Elf32_Shdr section) {
	Elf32_Shdr table_chaine;
	char* name = malloc(255);
	fseek(elfFile, header.e_shoff + header.e_shstrndx * header.e_shentsize, SEEK_SET);
	litEtInverse_Section(elfFile, header, &table_chaine);
	/*fread(&table_chaine, 1, sizeof(Elf32_Shdr), elfFile);

	//Si le fichier ELF n'est pas en litle Endian on l'inverse
	if (isbigendian(header)) {
		inversion_Sections(&table_chaine);
	}*/

	fseek(elfFile, table_chaine.sh_offset + section.sh_name, SEEK_SET);
	char c = fgetc(elfFile);
	int i = 0;
	while(c!='\0') {
		name[i]=c;
		i++;
		c=fgetc(elfFile);
	}
	name[i]='\0';
	return name;
}

uint16_t reverseByte16(uint16_t n) {
	return ((n>>8)&0xff) | ((n<<8)&0xff00);
}

uint32_t reverseByte32(uint32_t n) {
	return ((n>>24)&0xff) | ((n<<8)&0xff0000)
				| ((n>>8)&0xff00) | ((n<<24)&0xff000000);
}

uint64_t reverseByte64(uint64_t n) {
	return ((n>>24)&0xff0000) | ((n<<8)&0xff00000000)
				| ((n>>8)&0xff000000) | ((n<<24)&0xff0000000000)
				| ((n>>40)&0xff00) | ((n<<40)&0xff000000000000)
				| ((n<<56)&0xff00000000000000) | ((n>>56)&0xff);
}
