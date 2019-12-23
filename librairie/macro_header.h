#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>

/* Ce tableau d'octets indique comment interpréter 
 * le fichier, indépendemment du processeur ou 
 * du reste du contenu du fichier
 */
#define TAB_OCTET_INTERPRETATION_FICHIER e_ident

/* Ce champ de la structure identifie le type 
 * de fichier objet
 */
#define TYPE_FICHIER_OBJET e_type

/* Ce champ indique l'architecture nécessaire 
 * à un fichier individuel
 */
#define ARCHITECTURE_FICHIER e_machine

/* Ce champ indique la version du fichier
 */
#define VERSION_FICHIER e_version

/* Ce champ indique l'adresse virtuelle à 
 * laquelle le système transfère initialement 
 * le contrôle, démarrant ainsi le processus. 
 * Si ce fichier ne possède pas de point d'entrée, 
 * ce champ contient zéro.
 */
#define ADRESSE_VIRTUELLE e_entry

/* Ce champ contient le décalage en octets 
 * de la table contenant l'entête de programme. 
 * Si ce fichier ne contient pas de table 
 * d'entête de programme, ce champ contient zéro. 
 */
#define DECALAGE_TABLE_ENTETE e_phoff

/* Ce champ contient le décalage en octets de
 * la table des entêtes de sections. Si ce 
 * fichier ne contient pas de table des entêtes
 * des sections, ce champ contient zéro.
 */
#define DECALAGE_TABLE_ENTETE_SECTIONS e_shoff

/* Ce champ contient des drapeaux spécifiques
 * au processeur. Le nom de ces drapeaux prend
 * la forme : « EF_machine_drapeau ». À
 * l'heure actuelle, aucun drapeau n'a été défini. 
 */
#define DRAPEAUX_PROCESSEUR e_flags

/* Ce champ contient la taille de l'entête
 * ELF en octets.
 */
#define TAILLE_ENTETE_ELF e_ehsize

/* Ce champ contient la taille en octets
 * d'une entrée de la table d'entête de
 * programme ; toutes les entrées ont la même taille. 
 */
#define TAILLE_ENTREE_TABLE_ENTETE e_phentsize

/* Ce champ contient le nombre d'entrées de
 * la table d'entête de programme. Ainsi,
 * la taille en octets de la table pourra
 * être obtenue en multipliant e_phentsize
 * par e_phnum. Si le fichier ne contient pas
 * d'entête de programme, e_phnum contiendra
 * la valeur zéro.
 */
#define NOMBRE_ENTREE_TABLE_ENTETE e_phnum

/* Ce champ contient la taille en octets d'un
 * entête de section. Un entête de section est
 * une entrée de la table des entêtes de sections ;
 * toutes les entrées ont la même taille.
 */
#define TAILLE_ENTETE_SECTION e_shentsize

/* Ce champ contient le nombre d'entrées de la
 * table des entêtes de sections. Ainsi, la taille
 * en octets de la table des entêtes de sections
 * pourra être obtenue en multipliant e_shentsize
 * par e_shnum S'il n'y a pas de table des entêtes
 * de sections, le champ e_shnum contiendra zéro.
 */
#define NOMBRE_ENTREE_TABLE_SECTIONS e_shnum

/* Ce champ contient l'indice dans la table des
 * entêtes de sections de l'entrée associée à la
 * table des chaînes de noms des sections. Si le
 * fichier ne contient pas de table des chaînes
 * de noms des sections, ce champ contiendra la
 * valeur SHN_UNDEF.
 */
#define INDICE_TABLE_ENTETE_SECTIONS e_shstrndx
