#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>
/*
    //si ça ne marche pas essayer ça :
    //type
    switch (ELF32_ST_TYPE(mysym.st_info)) {
        case 0:
            printf("  NOTY");
            break;
        case 1:
            printf("  OBJT");
            break;
        case 2:
            printf("  FUNC");
            break;
        case 3:
            printf("  SECT");
            break;
        case 4:
            printf("  FILE");
            break;

        default:
            break;
    }

    //bind
    switch(ELF32_ST_BIND(mysym.st_info))
    {
        case 0: printf(" LOCL");
            break;
        case 1: printf(" GLOB");
            break;
        case 2: printf(" WEAK");
            break;
        case 3: printf("  NUM");
            break;

        default:
            break;
    }

*/

char * read_section64(int32_t fd, Elf64_Shdr sh)
{
	//alloue la mémoire requise pour stocker les informations du fichier elf
	//sh_size contient la taille de la section en octet
	char* buff = malloc(sh.sh_size);
	if(!buff) {
		printf("%s:Failed to allocate %ldbytes\n",
				__func__, sh.sh_size);
	}

	assert(buff != NULL);
	//lseek permet de placer la tête de lecture/ecriture à la position sh.sh_offset du descripteur fd
	//sh.sh_offset indique le décalage du premier octet de la section par rapport au début du fichier
	assert(lseek(fd, (off_t)sh.sh_offset, SEEK_SET) == (off_t)sh.sh_offset);
	//lit le fichier et le stock dans le buffer buff
	assert(read(fd, (void *)buff, sh.sh_size) == sh.sh_size);

	return buff;
}

void print_symbol_table64(int32_t fd, Elf64_Ehdr eh, Elf64_Shdr sh_table[], uint32_t symbol_table){	
	char *str_tbl;
	Elf64_Sym* sym_tbl;
	uint32_t i, symbol_count;
	//on lit et stock la table des symboles dans la variable sym_tbl
	sym_tbl = (Elf64_Sym*)read_section64(fd, sh_table[symbol_table]);
	
	//sh_link contient un lien vers l'indice de la table des entêtes de sections
	uint32_t str_tbl_ndx = sh_table[symbol_table].sh_link;
	//debug("str_table_ndx = 0x%x\n", str_tbl_ndx);
	//on lit et stock les informations du lien du ficheir ELF
	str_tbl = read_section64(fd, sh_table[str_tbl_ndx]);
	
	//nombre de symbole de la section
	symbol_count = (sh_table[symbol_table].sh_size/sizeof(Elf64_Sym));
	printf("%d symbols\n", symbol_count);
	
	//on affiche les informations du symbole
	for(i=0; i< symbol_count; i++) {
		printf("0x%08lx ", sym_tbl[i].st_value);
		printf("0x%02x ", ELF32_ST_BIND(sym_tbl[i].st_info));
		printf("0x%02x ", ELF32_ST_TYPE(sym_tbl[i].st_info));
		printf("%s\n", (str_tbl + sym_tbl[i].st_name));
	}
}

void print_table_symbol(FILE* elfFile, Elf64_Ehdr eh, Elf64_Shdr sh_table[]){
	uint32_t i;
	//créé le descripteur de fichier
	int32_t fd = (int32_t)fileno(efFile);
	
	//eh.e_shnum contient le nombre d'entrees de la table des entêtes de sections
	for(i=0; i<eh.e_shnum; i++) {
		//sh_table[i].sh_type définit le contenu et la sémantique de la section
		// SHT_SYMTAB cette section contient une table de symbole
		// SHT_DYNSYM est l'autre section pour la table des symboles de liaison dynamique
		if ((sh_table[i].sh_type==SHT_SYMTAB)
				|| (sh_table[i].sh_type==SHT_DYNSYM)) {
			printf("\n[Section %03d]", i);
			print_symbol_table64(fd, eh, sh_table, i);
		}
	}
}

int main(int argc, char* argv[]){
	FILE * elfFile;

	char buff[255];
	if (argc == 1) {
		printf("Il faut donner un fichier en argument.\n");
		exit(1);
	} else {
		elfFile = fopen(argv[1], "r");
		if (elfFile == NULL) {
			printf("Erreur d'ouverture du fichier.\n");
		}
	}
	// read the symbol table
	//fread(&symbole, 1, sizeof(symbole), elfFile);
	
	// check so its really an elf file
	//if (memcmp(header.e_ident, ELFMAG, SELFMAG) == 0) {
	
	return 0;
}
