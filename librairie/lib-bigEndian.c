#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>
#include "lib-bigEndian.h"

int isbigendian(Elf32_Ehdr h){
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

uint16_t byteshift16(uint16_t n){
    return ((n>>8)&0xff) | ((n<<8)&0xff00);
}

uint32_t byteshift32(uint32_t n) {
    return ((n>>24)&0xff) | ((n<<8)&0xff0000) | ((n>>8)&0xff00) | ((n<<24)&0xff000000);
}

uint64_t byteshift64(uint64_t n) {
    return ((n>>24)&0xff0000) | ((n<<8)&0xff00000000) | ((n>>8)&0xff000000) | ((n<<24)&0xff0000000000) | ((n>>40)&0xff00) | ((n<<40)&0xff000000000000) | ((n<<56)&0xff00000000000000) | ((n>>56)&0xff);
}

void inversion_Header(Elf32_Ehdr *header){
        header->e_phoff = byteshift32(header->e_phoff);
        header->e_shoff = byteshift32(header->e_shoff);
        header->e_flags = byteshift32(header->e_flags);
        header->e_ehsize = byteshift16(header->e_ehsize);
        header->e_phentsize = byteshift16(header->e_phentsize);
        header->e_phnum = byteshift16(header->e_phnum);
        header->e_shentsize = byteshift16(header->e_shentsize);
        header->e_shnum = byteshift16(header->e_shnum);
        header->e_shstrndx = byteshift16(header->e_shstrndx);
	header->e_type = byteshift16(header->e_type);
	header->e_machine = byteshift16(header->e_machine);
	header->e_version = byteshift32(header->e_version);
}

void inversion_Sections(Elf32_Shdr *section){
	section->sh_name = byteshift32(section->sh_name);
	section->sh_type = byteshift32(section->sh_type);
        section->sh_addr = byteshift32(section->sh_addr);
        section->sh_offset = byteshift32(section->sh_offset);
        section->sh_size = byteshift32(section->sh_size);
        section->sh_entsize = byteshift32(section->sh_entsize);
        section->sh_flags = byteshift32(section->sh_flags);
        section->sh_link = byteshift32(section->sh_link);
        section->sh_info = byteshift32(section->sh_info);
        section->sh_addralign = byteshift32(section->sh_addralign);
}

void insersion_Table_Symbole(Elf32_Sym *sym_tbl){
	sym_tbl->st_value = byteshift32(sym_tbl->st_value);
	sym_tbl->st_size = byteshift32(sym_tbl->st_size);
    	sym_tbl->st_shndx = byteshift16(sym_tbl->st_shndx);
    	sym_tbl->st_name = byteshift32(sym_tbl->st_name);	
}

void inversion_Relation_Sans_Additif(Elf32_Rel *rel){
	rel->r_offset = byteshift32(rel->r_offset);
        rel->r_info = byteshift32(rel->r_info);
}

void inversion_Relation_Additif(Elf32_Rela *rela){
	rela->r_offset = byteshift32(rela->r_offset);
        rela->r_info = byteshift32(rela->r_info);
	rela->r_addend = byteshift32(rela->r_addend);
}
