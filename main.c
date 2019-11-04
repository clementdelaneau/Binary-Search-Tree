#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
# include "abr.h"
#include "abr.c"
#include "menu.c"


int main()
{
    bool initialisation=true;
    while(initialisation)
    {

        double variable_a_predire=0.0;
        double seuil_min=0.0;
        double seuil_max=0.0;
        int nb_min=0;
        int taille_max=0;

        printf("Veuillez choisir la valeur de la variable a predire Y:\n");
        scanf("%lf",&variable_a_predire);

        printf("Veuillez saisir le seuil minimal:\n");
        scanf("%lf",&seuil_min);

        printf("Veuillez saisir le seuil max:\n");
        scanf("%lf",&seuil_max);

        printf("Veuillez saisir le nombre minimal dindividu par echantillon:\n");
        scanf("%d",&nb_min);

        printf("Veuillez saisir la taille maximale de l'arbre:\n");
        scanf("%d",&taille_max);


        matrice_donnees* data=charger_donnnees("C:/Users/cleme/Documents/CodeBlocksProjects/C/projetstr/bin/Debug/iris.txt");
        vecteur * newvect=create_vecteur(data);


        noeud * parent=creer_noeud(data,variable_a_predire,newvect,-1,-1,'\0',NULL);


        creer_arbre(parent,data,variable_a_predire,taille_max,nb_min,seuil_min,seuil_max);

        printf("\n");



int menu=0;
bool continuer=true;
while(continuer)
{
    printf("\n");
    printf("\n");
    printf("1. Afficher la hauteur de l'arbre\n");
    printf("2. Afficher la largeur de l'arbre\n");
    printf("3. Afficher l'arbre sous forme arborescente\n");
    printf("4. Afficher les feuilles\n");
    printf("5. Tester le programme avec de nouvelles donnees\n");
    printf("Veuillez choisir une option svp : ");
    scanf("%d",&menu);
    printf("\n");
    switch(menu)
    {
    case 1:
        afficher_hauteur(parent);
        break;

    case 2:
        afficher_largeur(parent);
        break;

    case 3:
        affichage_arborescence(parent,0);
        break;

    case 4:
        affichage_feuille(parent);
        break;

    case 5:
        continuer=false;
        printf("\n\n\n");
        break;
    }

}

    }


    return 0;
}





