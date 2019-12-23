#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>
#include "etape1-2.h"

void headtext(){
  printf("Il y a 11 en-têtes de section, débutant à l'adresse de décalage 0x7b8:\n\nEn-têtes de section :\n[Nr] Nom               Type             Adresse           Décalage\n       Taille            TaillEntrée   Fanion  Lien  Info  Alignement\n");
}

void foottext(){
  printf("Clé des fanions :\n  W (écriture), A (allocation), X (exécution), M (fusion), S (chaînes), I (info),\n  L (ordre des liens), O (traitement supplémentaire par l'OS requis), G (groupe),\n  T (TLS), C (compressé), x (inconnu), o (spécifique à l'OS), E (exclu),\n  l (grand), p (processor specific)\n");

}

void get_section_name_2(FILE* elfFile,Elf32_Ehdr header,Elf32_Shdr section, char* name){
    Elf32_Shdr table_chaine;
    long adresse = header.DECALAGE_TABLE_ENTETE_SECTIONS + (header.INDICE_TABLE_ENTETE_SECTIONS * header.TAILLE_ENTETE_SECTION);
    fseek(elfFile, adresse, SEEK_SET);
    fread(&table_chaine, 1, sizeof(Elf32_Shdr), elfFile);
    //Si le fichier ELF n'est pas en litle Endian on l'inverse
    if (isbigendian(header)){
	inversion_Sections(&table_chaine);
    }
    fseek(elfFile, table_chaine.DECALAGE_DEBUT_FICHIER + section.NOM_SECTION, SEEK_SET);

    char c = fgetc(elfFile);
    int i=0;
    while(c!='\0'){
        name[i]=c;
        i++;

        c=fgetc(elfFile);
    }
    name[i]='\0';
}

void align(char* str, int indent) {
  int length = strlen(str);

  //printf("AAAAAAAAAAAAAAAAAAA %d : %s AAAAAAAAAAAAAAAAAA", length, str);
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

void affichage_Table_Sections(FILE *elfFile, Elf32_Ehdr header) {
  	Elf32_Shdr section;
  	char buff[255];
	
        //SectNames = malloc(sectHdr.sh_size);
        //fseek(ElfFile, sectHdr.sh_offset, SEEK_SET);
        //fread(SectNames, 1, sectHdr.sh_size, ElfFile);

        // read all section headers
        printf("Il  y a %d sections dans le fichier.", header.e_shnum);
        headtext();
        for (int i = 0; i < header.NOMBRE_ENTREE_TABLE_SECTIONS; i++){
          	const char* name = "";

          	fseek(elfFile, header.DECALAGE_TABLE_ENTETE_SECTIONS + i * sizeof(section), SEEK_SET);
          	fread(&section, 1, sizeof(section), elfFile);
		//Si le fichier ELF n'est pas en litle Endian on l'inverse
		if (isbigendian(header)){
			inversion_Sections(&section);
		}

          	// print section name
          	printf("[%2u] ", i);
          	//if (sectHdr.sh_name)
          	//  name = SectNames + sectHdr.sh_name;

          	char nom_section[255];



          	get_section_name_2(elfFile, header, section, nom_section);
          	printf("%s ",nom_section);
          	align(nom_section, 16);

          	typeToString(section.CONTENU_SEMANTIQUE);
          	printf("%016x ", section.ADRESSE_MEMOIRE);
          	printf("%08x\n     ", section.DECALAGE_DEBUT_FICHIER);
          	printf("%016x ", section.TAILLE_SECTION);
          	printf("%016x ", section.TAILLE_TABLE_ENTREE_FIXE);
          	flagsToString(section.DRAPEAUX_ATTRIBUTS);
          	printf("%u\t", section.LIEN_INDICE_TABLE_SECTION);
          	printf("%u\t", section.INFORMATIONS_COMPLEMENTAIRES);
          	printf("%u\n", section.CONTRAINTE_D_ALIGNEMENT);
        }
        foottext();
}
