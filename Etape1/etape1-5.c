#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>
#include "etape1-5.h"

void get_section_name(FILE* elfFile,Elf32_Ehdr header,Elf32_Shdr section, char* name){
    Elf32_Shdr table_chaine;
    fseek(elfFile, header.e_shoff + header.e_shstrndx * header.e_shentsize, SEEK_SET);
    fread(&table_chaine, 1, sizeof(Elf32_Shdr), elfFile);
    if (isbigendian(header)){
	inversion_Sections(&table_chaine);
    }
    fseek(elfFile, table_chaine.sh_offset + section.sh_name, SEEK_SET);
    char c=fgetc(elfFile);
    int i=0;
    while(c!='\0'){
        name[i]=c;
        i++;
        c=fgetc(elfFile);
    }
    name[i]='\0';

}

void afficher_relocation_type(int type){
    switch(type){
        case 0:
            printf("R_ARM_NONE");
            break;
        case 1:
            printf("R_ARM_PC24");
            break;
        case 2:
            printf("R_ARM_ABS32");
            break;
        case 3:
            printf("R_ARM_REL32");
            break;
        case 4:
            printf("R_ARM_LDR_PC_G0");
            break;
        case 5:
            printf("R_ARM_ABS16");
            break;
        case 6:
            printf("R_ARM_ABS12");
            break;
        case 7:
            printf("R_ARM_THM_ABS5");
            break;
        case 8:
            printf("R_ARM_ABS8");
            break;
        case 9:
            printf("R_ARM_SBREL32");
            break;
        case 10:
            printf("R_ARM_THM_CALL");
            break;
        default:
            printf("TYPE INCONNU");
            break;
    }
}

void affichage_Table_Reimplantation(FILE *elfFile, Elf32_Ehdr header) {
  	Elf32_Shdr section;

  	char buff[255]; 


        //SectNames = malloc(sectHdr.sh_size);
        //fseek(ElfFile, sectHdr.sh_offset, SEEK_SET);
        //fread(SectNames, 1, sectHdr.sh_size, ElfFile);
        // read all section headers
        char nom_section[255];
        //printf("\n%d  %d\n",SHT_REL,SHT_RELA);
        for (int i = 0; i < header.e_shnum; i++){
          	fseek(elfFile, header.e_shoff + i * header.e_shentsize, SEEK_SET);
          	fread(&section, 1, sizeof(section), elfFile);
		if (isbigendian(header)){
			inversion_Sections(&section);
		}
          	//printf("TYPE : %d\n",section.sh_type);
          	//get_section_name(elfFile,header,section,nom_section);
          	//printf("NOM : %s",nom_section);
          	if(section.sh_type == SHT_RELA){
              		Elf32_Rela rela;
              		get_section_name(elfFile,header, section, nom_section);
              		int nb_entree=(int)section.sh_size / sizeof(Elf32_Rela);
              		printf("Section de réadressage '%s' à l'adresse de décalage 0x%04x contient %d entrées\n", nom_section, section.sh_offset, nb_entree);
              		fseek(elfFile, section.sh_offset, SEEK_SET);
              		for (int i=0;i<nb_entree;i++){
                  		fread(&rela,1,sizeof(rela),elfFile);
				if (isbigendian(header)){
					inversion_Relation_Additif(&rela);
				}
                  		printf("décalage : %012x  ", rela.r_offset);
                  		printf("type : ");
                  		afficher_relocation_type(ELF32_R_TYPE(rela.r_info));
                  		printf("  ");
                  		printf("index : %u \n",ELF32_R_SYM(rela.r_info));
              		}
              		printf("\n");
          	} //Verifier si ce n'est pas la même !!!!!!!!!!!!!!!!!
          	else if(section.sh_type == SHT_REL){
              		Elf32_Rel rel;
              		get_section_name(elfFile,header, section, nom_section);
              		int nb_entree=(int)section.sh_size / sizeof(Elf32_Rel);
              		printf("Section de réadressage '%s' à l'adresse de décalage 0x%04x contient %d entrées\n",nom_section, section.sh_offset, nb_entree);
              		fseek(elfFile, section.sh_offset, SEEK_SET);
              		for (int i=0;i<nb_entree;i++){
                  		fread(&rel,1,sizeof(rel),elfFile);
				if (isbigendian(header)){
					inversion_Relation_Sans_Additif(&rel);
				}
                  		printf("décalage : %012x  ", rel.r_offset);
                  		printf("type : ");
                  		afficher_relocation_type(ELF32_R_TYPE(rel.r_info));
                  		printf("  ");
                  		printf("index : %u \n",ELF32_R_SYM(rel.r_info));
              		}
              		printf("\n");
          	}

        }
}
