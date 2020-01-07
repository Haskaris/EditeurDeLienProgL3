#include <stdio.h>
#include <stdlib.h>
#include "arbreBinaire.h"


/* Cherche un élément dans
 * l'arbre de recherche
 * retourne l'élément trouvé
 * ou NULL si l'élément n'existe pas
*/
struct Noeud* dansArbreDeRecherche(struct Noeud *noeud, Elf32_Sym symbole){
	if (noeud == NULL){
		return NULL;
	} else if (noeud->symboleCourant.NOM_SYMBOLE == symbole.NOM_SYMBOLE){
		return noeud;
	} else if (noeud->symboleCourant.NOM_SYMBOLE < symbole.NOM_SYMBOLE){
		return dansArbreDeRecherche(noeud->noeudGauche, symbole);
	} else {
		//noeud->name > name
		return dansArbreDeRecherche(noeud->noeudDroit, symbole);
	}
}

/* Insère un élément dans 
 * l'arbre de recherche
*/
void insereNoeudDansArbre(struct Noeud *Arbre, Elf32_Sym symbole){
	if (Arbre->symboleCourant.NOM_SYMBOLE == symbole.NOM_SYMBOLE){
		printf("Problème noeud déjà dans arbre (arbreBinaire.c)\n");
		exit(0);
	} else if (Arbre->symboleCourant.NOM_SYMBOLE < symbole.NOM_SYMBOLE){
		if (Arbre->noeudGauche == NULL){
			struct Noeud *nouveauNoeud = malloc(sizeof(struct Noeud));
			nouveauNoeud->symboleCourant.NOM_SYMBOLE = symbole;
			Arbre->noeudGauche = nouveauNoeud;
		} else {
			return dansArbreDeRecherche(Arbre->noeudGauche, symbole);
		}
	} else {
		//noeud->name > name
		if (Arbre->noeudDroit == NULL){
			struct Noeud *nouveauNoeud = malloc(sizeof(struct Noeud));
			nouveauNoeud->symboleCourant.NOM_SYMBOLE = symbole;
			Arbre->noeudDroit = nouveauNoeud;
		} else {
			return dansArbreDeRecherche(Arbre->noeudDroit, symbole);
		}
	}
}
