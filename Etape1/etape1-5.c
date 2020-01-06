//Inclure les autres du .h ?
#include "etape1-5.h"

void afficher_relocation_type(int type) {
	switch(type) {
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

	for (int i = 0; i < header.e_shnum; i++) {
		fseek(elfFile, header.e_shoff + i * header.e_shentsize, SEEK_SET);
		fread(&section, 1, sizeof(section), elfFile);

		if (isbigendian(header)) {
			inversion_Sections(&section);
		}

		char* nom_section = NULL;

		if(section.sh_type == SHT_RELA){
			Elf32_Rela rela;
			//Penser à free
			nom_section = get_section_name(elfFile, header, section);

			int nb_entree = (int)section.TAILLE_SECTION / sizeof(Elf32_Rela);
			printf("Section de réadressage '%s' à l'adresse de décalage 0x%04x contient %d entrées\n",nom_section,section.sh_offset,nb_entree);
			fseek(elfFile, section.sh_offset, SEEK_SET);
			for (int i=0;i<nb_entree;i++){
				fread(&rela,1,sizeof(rela),elfFile);
				printf("décalage : %012x  ", rela.r_offset);
				printf("type : ");
				afficher_relocation_type(ELF32_R_TYPE(rela.r_info));
				printf("  ");
				printf("index : %u \n",ELF32_R_SYM(rela.r_info));
			}
			printf("\n");
		} //Verifier si ce n'est pas la même !!!
		else if(section.sh_type == SHT_REL) {
			Elf32_Rel rel;

			nom_section = get_section_name(elfFile, header, section);

			int nb_entree=(int)section.sh_size/sizeof(Elf32_Rel);
			printf("Section de réadressage '%s' à l'adresse de décalage 0x%04x contient %d entrées\n",nom_section,section.sh_offset,nb_entree);
			fseek(elfFile, section.sh_offset, SEEK_SET);
			for (int i=0;i<nb_entree;i++){
				fread(&rel,1,sizeof(rel),elfFile);
				printf("décalage : %012x  ",rel.r_offset);
				printf("type : ");
				afficher_relocation_type(ELF32_R_TYPE(rel.r_info));
				printf("  ");
				printf("index : %u \n",ELF32_R_SYM(rel.r_info));
			}
			printf("\n");
		}
		free(nom_section);
	}
}
