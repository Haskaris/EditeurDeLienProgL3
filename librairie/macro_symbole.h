#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>

/* Ce champ contient un indice de la table
 * des chaînes de symboles d'un fichier objet.
 * Cette table contient la représentation sous
 * la forme de chaînes de caractères des noms
 * des symboles. Si la valeur de ce champ est
 * non nulle, il représente l'indice de la table
 * des chaînes qui donne son nom au symbole.
 * Sinon, le symbole n'a pas de nom. 
 */
#define NOM_SYMBOLE st_name

/* Ce champ donne la valeur associée au symbole. 
 */
#define VALEUR_SYMBOLE st_value

/* Des tailles sont associées à nombre
 * de symboles. Ce champ contient zéro si
 * le symbole n'a pas de taille ou si sa
 * taille est inconnue. 
 */
#define TAILLE_SYMBOLE st_size

/* Ce champ indique le type de symbole et
 * ses attributs de liaison
 */
#define TYPE_SYMBOLE_ET_ATTRIBUT_LIAISON st_info

/* Ce champ définit la visibilité des symboles
 */
#define VISIBILITE_SYMBOLE st_other

/* Chaque entrée de la table des symboles est
 * « définie » en relation à une section. Ce
 * champ contient l'indice correspondant de
 * la table des entêtes de sections. 
 */
#define INDICE_TABLE_SECTION st_shndx
