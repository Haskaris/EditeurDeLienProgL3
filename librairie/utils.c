#include "utils.h"

/*
* Il faut penser à FREE !
*/
char* get_section_name(FILE* elfFile, Elf32_Ehdr header, Elf32_Shdr section, int bigEndian) {
	Elf32_Shdr table_chaine;
	char* name = malloc(255);
	fseek(elfFile, byteshift32(header.e_shoff, bigEndian) + byteshift16(header.e_shstrndx, bigEndian) * byteshift16(header.e_shentsize, bigEndian), SEEK_SET);
	fread(&table_chaine, 1, sizeof(Elf32_Shdr), elfFile);
	fseek(elfFile, byteshift32(table_chaine.sh_offset, bigEndian) + byteshift32(section.sh_name, bigEndian), SEEK_SET);
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
