#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>

/* Ce champ indique le nom de la section.
 * Sa valeur est un indice de la table des
 * chaînes des entêtes de sections, contenant
 * l'emplacement d'une chaîne terminée par
 * un octet nul. 
 */
#define NOM_SECTION sh_name

/* Ce champ définit le contenu et la
 * sémantique de la section. 
 */
#define CONTENU_SEMANTIQUE sh_type

/* Les sections contiennent des
 * drapeaux binaires décrivant divers
 * attributs. Si, dans sh_flags, le
 * bit correspondant à un drapeau
 * est positionné, l'attribut est
 * activé pour cette section. Sinon,
 * l'attribut est désactivé ou ne
 * s'applique pas. Les attributs non
 * définis sont mis à zéro. 
 */
#define DRAPEAUX_ATTRIBUTS sh_flags
 
/* Si cette section apparaît dans
 * l'image mémoire d'un processus,
 * ce champ contient l'adresse à
 * laquelle le premier octet de la
 * section doit se trouver. Sinon,
 * ce champ contient zéro. 
 */
#define ADRESSE_MEMOIRE sh_addr

/* La valeur de ce champ indique le
 * décalage du premier octet de la
 * section par rapport au début du fichier.
 * Une section de type SHT_NOBITS, n'occupe pas
 * de place dans le fichier et son champ sh_offset
 * indique son placement théorique dans le fichier. 
 */
#define DECALAGE_DEBUT_FICHIER sh_offset

/* Ce champ contient la taille de la section en
 * octets. À moins que cette section ne soit de
 * type SHT_NOBITS, elle occupe sh_size octets dans
 * le fichier. Une section de type SHT_NOBITS peut
 * avoir une taille non nulle, mais elle n'occupera
 * cependant aucune place dans le fichier. 
 */
#define TAILLE_SECTION sh_size

/* Ce champ contient un lien vers un indice de
 * la table des entêtes de sections, dont
 * l'interprétation dépend du type de section. 
 */
#define LIEN_INDICE_TABLE_SECTION sh_link

/* Ce champ contient des informations complémentaires,
 * dont l'interprétation dépend du type de section.
 */
#define INFORMATIONS_COMPLEMENTAIRES sh_info

/* Certaines sections ont des contraintes d'alignement.
 * Si une section contient un mot double, le système
 * doit s'assurer que la section tout entière est
 * alignée sur les mots doubles. Autrement dit,
 * la valeur de sh_addr doit être congrue à zéro,
 * modulo la valeur de sh_addralign. Seules des valeurs
 * de zéro ou des nombres positifs puissances entières
 * de deux sont autorisés. Une valeur de zéro ou de un
 * indique qu'aucune contrainte d'alignement ne s'applique
 * à la section.
 */
#define CONTRAINTE_D_ALIGNEMENT sh_addralign

/* Certaines sections contiennent une table contenant
 * un nombre d'entrées fixe, comme par exemple les
 * tables de symboles. Pour de telles sections, ce
 * champ donne la taille en octets de chaque entrée.
 * Ce champ contient zéro si cette section ne contient
 * pas une table de ce type. 
 */
#define TAILLE_TABLE_ENTREE_FIXE sh_entsize
