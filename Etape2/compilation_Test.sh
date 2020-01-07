#!/bin/bash
#Le script lance la compilation et le fichier principal
<<<<<<< HEAD
#il l'execute pour tout les examples 

make clean 

make


#
#
# A CHANGER 
#
#



#Choix du numéro de section si on prend l'étape 3
numSection=0

#Ensemble des fichiers tests .o
elfExampleFile=$(ls ../Examples_loader | grep .o$)

echo -e "\n\nQue voulez vous faire :\n\t- Afficher l'entête d'un fichier ELF (1)\n\t- Afficher la table des sections et des détails relatifs à chaque section (2)\n\t- Afficher le contenu d'une section (3)\n\t- Afficher la table des symboles et des détails relatifs à chaque symbole (4)\n\t- Afficher la table de réimplantation et des détails relatifs à chaque entrée (5)\n"
=======
#il l'execute pour tout les examples

make clean

make

#Ensemble des fichiers tests .o
elfExampleFile=$(ls ../Examples_loader | grep .o$)

echo -e "\n\nQue voulez vous faire :\n\t- Etape 2-6 (1)\n\t- Etape 2-7 (2)\n\t- Etape 2-8 (3)\n\t- Etape 2-9 (4)\n"
>>>>>>> 4b12010e003952b0c10878daccdf53b552c46c20

echo "Quel est votre choix ? "
#Choix de ce qu'on veut afficher
read choix
echo "choix : $choix"

<<<<<<< HEAD
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
=======

for ((i=6; i<=7; i++))
do
	j = ((i+1))
	echo -e "\n\n\nFichier du dossier Examples_loader : $i"
	./elfParsing "../Examples_loader/$i" "../Examples_loader/$j" "../Examples_loader/Fusion/fusion_$i" $choix
>>>>>>> 4b12010e003952b0c10878daccdf53b552c46c20
done
