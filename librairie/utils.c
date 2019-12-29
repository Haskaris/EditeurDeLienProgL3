#include "lib-bigEndian.h"
#include "utils.h"

#define DECALAGE(header, i) header.DECALAGE_TABLE_ENTETE_SECTIONS + i * sizeof(Elf32_Shdr)

/*
* Il faut penser à FREE !
*/
char* get_section_name(FILE* elfFile, Elf32_Ehdr header, Elf32_Shdr section) {
	Elf32_Shdr table_chaine;
	char* name = malloc(255);
	fseek(elfFile, header.e_shoff + header.e_shstrndx * header.e_shentsize, SEEK_SET);
	fread(&table_chaine, 1, sizeof(Elf32_Shdr), elfFile);

	//Si le fichier ELF n'est pas en litle Endian on l'inverse
	if (isbigendian(header)) {//
		inversion_Sections(&table_chaine);//
	}//

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
