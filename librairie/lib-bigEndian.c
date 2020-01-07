#include <elf.h>
#include "utils.h"
#include "lib-bigEndian.h"

int isbigendian(Elf32_Ehdr h) {
	switch (h.e_ident[EI_DATA]) {
		case ELFDATA2LSB:
			return 0;
		case ELFDATA2MSB:
			return 1;
		default://ELFDATANONE
			printf("isbigEndian problème pas d'encodage\n");
			exit(1);
		}
}

void inversion_Header(Elf32_Ehdr *header) {
	header->e_phoff = reverseByte32(header->e_phoff);
	header->e_shoff = reverseByte32(header->e_shoff);
	header->e_flags = reverseByte32(header->e_flags);
	header->e_ehsize = reverseByte16(header->e_ehsize);
	header->e_phentsize = reverseByte16(header->e_phentsize);
	header->e_phnum = reverseByte16(header->e_phnum);
	header->e_shentsize = reverseByte16(header->e_shentsize);
	header->e_shnum = reverseByte16(header->e_shnum);
	header->e_shstrndx = reverseByte16(header->e_shstrndx);
	header->e_type = reverseByte16(header->e_type);
	header->e_machine = reverseByte16(header->e_machine);
	header->e_version = reverseByte32(header->e_version);
}

void inversion_Sections(Elf32_Shdr *section) {
	section->sh_name = reverseByte32(section->sh_name);
	section->sh_type = reverseByte32(section->sh_type);
	section->sh_addr = reverseByte32(section->sh_addr);
	section->sh_offset = reverseByte32(section->sh_offset);
	section->sh_size = reverseByte32(section->sh_size);
	section->sh_entsize = reverseByte32(section->sh_entsize);
	section->sh_flags = reverseByte32(section->sh_flags);
	section->sh_link = reverseByte32(section->sh_link);
	section->sh_info = reverseByte32(section->sh_info);
	section->sh_addralign = reverseByte32(section->sh_addralign);
}

void insersion_Table_Symbole(Elf32_Sym *sym_tbl) {
	sym_tbl->st_value = reverseByte32(sym_tbl->st_value);
	sym_tbl->st_size = reverseByte32(sym_tbl->st_size);
	sym_tbl->st_shndx = reverseByte16(sym_tbl->st_shndx);
	sym_tbl->st_name = reverseByte32(sym_tbl->st_name);
}

void inversion_Relation_Sans_Additif(Elf32_Rel *rel) {
	rel->r_offset = reverseByte32(rel->r_offset);
	rel->r_info = reverseByte32(rel->r_info);
}

void inversion_Relation_Additif(Elf32_Rela *rela) {
	rela->r_offset = reverseByte32(rela->r_offset);
	rela->r_info = reverseByte32(rela->r_info);
	rela->r_addend = reverseByte32(rela->r_addend);
}
