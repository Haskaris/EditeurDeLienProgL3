#!/bin/bash
#Le script lance la compilation et le fichier principal

#il l'execute pour tout les examples

make clean

make

#Ensemble des fichiers tests .o
elfExampleFile=$(ls ../Examples_loader | grep .o$)

echo -e "\n\nQue voulez vous faire :\n\t- Etape 2-6 (1)\n\t- Etape 2-7 (2)\n\t- Etape 2-8 (3)\n\t- Etape 2-9 (4)\n"


echo "Quel est votre choix ? "
#Choix de ce qu'on veut afficher
read choix
echo "choix : $choix"



for ((i=6; i<=7; i++))
do
	j = ((i+1))
	echo -e "\n\n\nFichier du dossier Examples_loader : $i"
	./elfParsing "../Examples_loader/$i" "../Examples_loader/$j" "../Examples_loader/Fusion/fusion_$i" $choix

done
