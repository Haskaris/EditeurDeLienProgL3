#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>

/* Ce champ donne l'emplacement où
 * appliquer l'action de repositionnement.
 */
#define EMPLACEMENT_REPOSITIONNEMENT r_offset

/* Ce champ donne à la fois l'indice de
 * la table des symboles par rapport auquel
 * on doit effectuer le repositionnement
 * et le type de repositionnement à appliquer.
 */
#define INDICE_SYMBOLE_ET_TYPE_REPOSITIONNEMENT r_info

/* Ce champ indique un additif constant
 * utilisé pour calculer la valeur à enregistrer
 * dans le champ repositionnable. 
 */
#define ADDITIF_CONSTANT r_addend
