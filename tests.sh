#!/bin/bash


while [ 1 ]
do
	echo -e "Quelle phase voulez-vous tester? (0 pour quitter)\n\n"

	read selection
	echo "Vous avez séléctionné : $selection"

	if [ $selection -eq 0 ]
	then
		break
	fi

	if [ $selection -eq 1 ]
	then
		cd Etape1

	########################################################## PHASE 1 ##########################################################
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
		cd ..
		echo -e "\n\n"
	fi
	########################################################## PHASE 2 ##########################################################

	if [ $selection -eq 2 ]
		then
		cd Etape2

		make clean

		make

		#Ensemble des fichiers tests .o
		elfExampleFile=$(ls ../Examples_loader | grep .o$)

		echo -e "\n\nQue voulez vous faire :\n\t- Etape 2-6 (1)\n\t- Etape 2-7 (2)\n\t- Etape 2-8 (3)\n\t- Etape 2-9 (4)\n"

		echo "Quel est votre choix ? "
		#Choix de ce qu'on veut afficher
		read choix
		echo "choix : $choix"

		for ((i=1; i<3; i++))
		do
			echo $i
			((j=i+1))
			echo $j
			echo -e "\nFichier du dossier Examples_loader : $i"
			./elfParsing "../Examples_loader/example$i.o" "../Examples_loader/example$j.o" "fusion_$i.o" $choix
		done
		cd ..
		echo -e "\n\n"
	fi
done
