#!/bin/bash
#Le script lance la compilation et le fichier principal
#il l'execute pour tout les examples 

make clean 

make

#Choix du numéro de section si on prend l'étape 3
numSection=0

#Ensemble des fichiers tests .o
elfExampleFile=$(ls ../Examples_loader | grep .o$)

echo -e "\n\nQue voulez vous faire :\n\t- Afficher l'entête d'un fichier ELF (1)\n\t- Afficher la table des sections et des détails relatifs à chaque section (2)\n\t- Afficher le contenu d'une section (3)\n\t- Afficher la table des symboles et des détails relatifs à chaque symbole (4)\n\t- Afficher la table de réimplantation et des détails relatifs à chaque entrée (5)\n"

echo "Quel est votre choix ? "
#Choix de ce qu'on veut afficher
read choix
echo "choix : $choix"

if [ $choix -eq 3 ]
then 
	#Si on veut afficher le contenu d'une section dans ce cas il faut le numéro de section
	echo ""
	echo "Il faut choisir le numéro d'une section : "
	read numSection
fi


for i in $elfExampleFile
do 
	echo -e "\n\n\nFichier du dossier Examples_loader : $i"
	./elfParsing "../Examples_loader/$i" $choix $numSection
done
