#include <stdio.h>
#include <stdlib.h>

typedef struct Noeud;

struct Noeud {
	struct Noeud *noeudGauche;
	struct Noeud *noeudDroit;
	Elf32_Sym symboleCourant;
};

/* Cherche un élément dans
 * l'arbre de recherche
 * retourne l'élément trouvé
 * ou NULL si l'élément n'existe pas
 * arguments :
 * 	- l'arbre contenant les variables globals (*noeud)
 *	- le symbole global recherché (symbole)
*/
struct Noeud* dansArbreDeRecherche(struct Noeud *noeud, Elf32_Sym symbole);

/* Insère un élément dans 
 * l'arbre de recherche
 * On suppose que le symbole ajouté n'est pas déjà présent dans l'arbre
 * arguments :
 * 	- l'arbre contenant toutes les variables globals (*Arbre)
 *	- le symbole global à ajouter dans l'arbre (symbole)
*/
void insereNoeudDansArbre(struct Noeud *Arbre, Elf32_Sym symbole);
