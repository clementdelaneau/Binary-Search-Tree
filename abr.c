#include "abr.h"
#include <stdlib.h>
#include <stdio.h>

//----------------------------------------------------------
// Initialisation des structures
//----------------------------------------------------------

matrice_donnees* charger_donnnees(const char* nom_fichier)
{
	FILE* fichier = fopen(nom_fichier,"r");
	if( fichier != NULL )
	{
		unsigned int nb_lignes;
		unsigned int nb_colonnes;

		// Etape 1 - traitement première ligne
		fscanf(fichier, "%u %u", &nb_lignes, &nb_colonnes); // %u <=> unsigned int
		fgetc(fichier); // lecture du retour chariot ('\n'). Valeur ignorée => passage à la ligne suivante

		// Etape 2 - allocation des lignes de la matrice
		double** matrice = (double**) malloc( nb_lignes * sizeof(double*) );

		// Etape 3 - remplissage de la matrice
		for(int ligne = 0 ; ligne < nb_lignes ; ligne++)
		{
			// allocation des colonnes de la matrice (pour chaque ligne)
			matrice[ligne] = (double*) malloc( nb_colonnes * sizeof(double) );

			for(int colonne = 0 ; colonne < nb_colonnes ; colonne++)
			{
				if(colonne != nb_colonnes-1)
				{
					fscanf(fichier, "%lg ", &matrice[ligne][colonne]);
				}
				else // dernière colonne (où le réel n'est pas suivi d'un espace)
				{
					fscanf(fichier, "%lg" , &matrice[ligne][colonne]);
				}
			}

			fgetc(fichier); // lecture du retour chariot ('\n'). Valeur ignorée => passage à la ligne suivante
		}

		matrice_donnees * data = (matrice_donnees*) malloc(sizeof(matrice_donnees));
		data->nb_colonnes = nb_colonnes;
		data->nb_lignes = nb_lignes;
		data->matrice = matrice;

		fclose(fichier);
		return data;
	}

	printf("Fichier %s inconnu.\n", nom_fichier);
	return NULL;
}

// Usage : var = liberer_donnees(var);  => var devient NULL
matrice_donnees* liberer_donnees(matrice_donnees * data)
{
	if(data != NULL)
	{
		for(int ligne = 0 ; ligne < data->nb_lignes ; ligne++)
		{
			if(data->matrice[ligne] != NULL) // normalement toujours vrai
				free(data->matrice[ligne]);
		}
		free(data->matrice);
		free(data);
	}
	return NULL;
}


vecteur * create_vecteur(matrice_donnees * data)    //vecteur racine contenant la liste des 120 individus
{

    vecteur * newvect=(vecteur*)malloc(sizeof(vecteur));
    newvect->logic_size=120;
    newvect->donnees_individus=(double*)malloc(sizeof(double)*(newvect->logic_size));
    for(int i=0;i<newvect->logic_size;i++)
    {
        newvect->donnees_individus[i]=i+1;
    }


    return newvect;
}


noeud * creer_noeud(matrice_donnees * data, double variable_a_predire,vecteur * newvect,int col,double med,char * signe,noeud * par)
{
    noeud * newnoeud=(noeud*)malloc(sizeof(noeud));
    newnoeud->donnees=data;
    newnoeud->y=variable_a_predire; //= 1,2 ou 3
    newnoeud->index_colonne=col; // variable_xi= 1,2,3 ou 4
    newnoeud->mediane_corrigee=med;
    newnoeud->individus=newvect;
    newnoeud->precision=precision_noeud(newnoeud,variable_a_predire);
    newnoeud->signe_division=signe;
    newnoeud->parent=par;
    newnoeud->fils_gauche=NULL;
    newnoeud->fils_droite=NULL;


    return newnoeud;

}



//----------------------------------------------------------
// Création de l'arbre
//----------------------------------------------------------

void creer_arbre(noeud * parent,matrice_donnees * data,double variable_a_predire,int taille_max,int nb_min,double seuil_min,double seuil_max)
{
    if(est_divisible(parent,taille_max,nb_min,seuil_min,seuil_max,variable_a_predire))
    {
        int variable=variable_meilleure_division(parent,data,variable_a_predire);  // definit la meilleur variable pour la division
        double * colonne_xi=recuperer_colonne(parent,variable); //on récupère la colonne de cette variable
        selection(colonne_xi,parent->individus->logic_size);
        double mediane_c=mediane_corrigee(colonne_xi,parent->individus->logic_size);// calcul la mediane corrigee des elements triées de cette variable
        char * signe=critere_de_division(parent);
        vecteur * vecfg=vec_fg(parent,variable,mediane_c);  //création d'un nouveau vecteur contenant la liste des indexs des individus du fils gauche
        vecteur * vecfd=vec_fd(parent,variable,mediane_c);
        parent->fils_gauche=creer_noeud(data,variable_a_predire,vecfg,variable,mediane_c,signe,parent);
        parent->fils_gauche->signe_division=critere_de_division(parent->fils_gauche);
        creer_arbre(parent->fils_gauche,data,variable_a_predire,taille_max,nb_min,seuil_min,seuil_max);
        parent->fils_droite=creer_noeud(data,variable_a_predire,vecfd,variable,mediane_c,signe,parent);
        parent->fils_droite->signe_division=critere_de_division(parent->fils_droite);
        creer_arbre(parent->fils_droite,data,variable_a_predire,taille_max,nb_min,seuil_min,seuil_max);
    }

}



//----------------------------------------------------------
// Fonctions classiques utilisées pour le programme
//----------------------------------------------------------

int max(int a,int b)
{
    int max=0;
    if(a>b)
    {
         max=a;
    }
    else
    {
        max=b;
    }
    return max;
}

double max_tab(double tableau[], int taille)
{
	int position_max = 0;

	for(int i = 1 ; i < taille ; i++)
	{
		if( tableau[i] > tableau[position_max] )
		{
			position_max = i;
		}
	}

	return tableau[position_max];
}


void selection(double *tab, int taille)
{
     int min=0;
     double temp=0;

     for (int i=0;i<taille-1;i++)
     {
         min=i;
         for (int j=i+1;j<taille;j++)
              if (tab[j]<tab[min])
              {
                  min=j;
              }
          temp=tab[i];
          tab[i]=tab[min];
          tab[min]=temp;
     }
}


void afficher_tableau_iteratif(double * tableau, int taille)
{
	int i = 0;
	while( i < taille )
	{
		printf("%lf\n",tableau[i]);
		i++;
	}
	printf("\n");
}


void afficher_offset(int offset)
{
	for(int i = 0 ; i < offset ; i++)
	{
		printf("  "); // 2 espaces
	}
}


//----------------------------------------------------------
// Fonctions principales
//----------------------------------------------------------

bool est_divisible(noeud * arbre,int taille_max,int nb_min,double seuil_min,double seuil_max,double valeur_a_predire)
{
    bool division=false;
    int hauteur=hauteur_arbre(arbre);
    int individus=arbre->individus->logic_size;
    double precision=precision_noeud(arbre,valeur_a_predire);
    if(hauteur<taille_max && individus>=nb_min && seuil_min<=precision && precision<=seuil_max)
    {
        division=true;
    }
    return division;
}


int hauteur_arbre(noeud * arbre)
{
    if(arbre==NULL)
    {
        return 0;
    }
    return 1+max(hauteur_arbre(arbre->fils_gauche),hauteur_arbre(arbre->fils_droite));
}


int largeur_arbre(noeud *element)
{
    if(element==NULL)
    {
        return 0;
    }
    else
    {
        if(est_feuille(element))
        {
            return 1;
        }
        else
            return largeur_arbre(element->fils_gauche)+largeur_arbre(element->fils_droite);
    }
}


double precision_noeud(noeud * arbre,double valeur_a_predire)
{
    double precision=0;
    double compt=0;
    for(int i=0;i<arbre->individus->logic_size;i++)
    {
      if(arbre->donnees->matrice[arbre->individus->donnees_individus[i]-1][0]==valeur_a_predire)
      {
          compt+=1;
      }
    }
    precision=compt/(arbre->individus->logic_size);

    return precision;
}


double * recuperer_colonne(noeud * arbre,int variable_xi)
{
    double * colonne=(double*)malloc(sizeof(double)*arbre->individus->logic_size);
    for(int i=0;i<arbre->individus->logic_size;i++)
    {
        colonne[i]=arbre->donnees->matrice[arbre->individus->donnees_individus[i]-1][variable_xi];
    }
    return colonne;

}


double mediane(double * tab,int taille)
{
    double mediane=-1;
    if(taille>0)
    {
        if(taille%2==0)
        {
            mediane=(tab[taille/2]+tab[(taille/2)+1])/2;
        }
        else
        {
            mediane=tab[(taille+1)/2];
        }
    }
    return mediane;
}


double mediane_corrigee(double * tab,int taille)
{
    double m=mediane(tab,taille);
    double mediane_corrige=m;
    if(taille>=2)
    {
        if(max_tab(tab,taille)==m)
        {
            double * tab_copie=(double*)malloc(sizeof(double)*taille);
            for(int i=0;i<taille;i++)
            {
                tab_copie[i]=tab[i];
                if(tab[i]==m)
                {
                    tab_copie[i]=0;
                }
            }
            mediane_corrige=max_tab(tab_copie,taille);
        }
        else
        {
            mediane_corrige=m;
        }
    }
    return mediane_corrige;
}


double precision_variable_gauche(noeud * arbre,matrice_donnees * data,int variable,double valeur_a_predire)    //"variable" vaut 1,2,3,ou 4
{

double * liste=recuperer_colonne(arbre,variable);
    selection(liste,arbre->individus->logic_size);
    double compt=0;
    double mediane=mediane_corrigee(liste,arbre->individus->logic_size);
    int j=0;
    while(liste[j]<=mediane)
    {
        j++;
    }
    for(int i=0;i<arbre->individus->logic_size;i++)
    {
        if(arbre->donnees->matrice[i][variable]<=mediane)
        {
            if(arbre->donnees->matrice[i][0]==valeur_a_predire)
            {
                compt+=1;
            }
        }
    }
    return compt/j;
}


double precision_variable_droite(noeud * arbre,matrice_donnees * data,int variable,double valeur_a_predire)    //"variable" vaut 1,2,3,ou 4
{

double * liste=recuperer_colonne(arbre,variable);
    selection(liste,arbre->individus->logic_size);
    double compt=0;
    double mediane=mediane_corrigee(liste,arbre->individus->logic_size);
    int j=0;
    while(liste[j]<=mediane)
    {
        j++;
    }
    for(int i=0;i<arbre->individus->logic_size;i++)
    {
        if(arbre->donnees->matrice[i][variable]>mediane)
        {
            if(arbre->donnees->matrice[i][0]==valeur_a_predire)
            {
                compt+=1;
            }
        }
    }
    return compt/(arbre->individus->logic_size-j);
}


int variable_meilleure_division(noeud * arbre,matrice_donnees * data,double valeur_a_predire)
{
    int res=0;
    double g1=precision_variable_gauche(arbre,data,1,valeur_a_predire);
    double d1=precision_variable_droite(arbre,data,1,valeur_a_predire);
    double g2=precision_variable_gauche(arbre,data,2,valeur_a_predire);
    double d2=precision_variable_droite(arbre,data,2,valeur_a_predire);
    double g3=precision_variable_gauche(arbre,data,3,valeur_a_predire);
    double d3=precision_variable_droite(arbre,data,3,valeur_a_predire);
    double g4=precision_variable_gauche(arbre,data,4,valeur_a_predire);
    double d4=precision_variable_droite(arbre,data,4,valeur_a_predire);
    double tab[]={g1,d1,g2,d2,g3,d3,g4,d4};
    double max=max_tab(tab,8);
    if(max==g1 || max==d1)
    {
        res=1;
    }
    if(max==g2 || max==d2)
    {
        res=2;
    }
    if(max==g3 || max==d3)
    {
        res=3;
    }
    if(max==g4 || max==d4)
    {
        res=4;
    }
    return res;
}


bool est_feuille(noeud const * element)
{
    bool est_feuille=false;
    if(element!=NULL && element->fils_gauche==NULL && element->fils_droite==NULL)
    {
        est_feuille=true;
    }

   return est_feuille;
}


char * critere_de_division(noeud * noeud)
{
    char * division=(char*)malloc(sizeof(char)*3);
    for(int i=0;i<3;i++)
    {
        division[i]='\0';
    }
    if(noeud->parent!=NULL)
    {

    if(noeud==noeud->parent->fils_gauche)
    {
        division[0]='<';
        division[1]='=';
    }
    else
    {
        division[0]='>';
    }
    }
    return division;
}


void chemin_feuille(noeud * noeud)
{
        while(noeud->parent!=NULL)
        {
            printf("X%d %s %lf\n",noeud->index_colonne,noeud->signe_division,noeud->mediane_corrigee);
            noeud=noeud->parent;
        }
}


vecteur * vec_fg(noeud * noeud,int variable,double mediane)
{
    int compt=0;
    vecteur * vec_fg=(vecteur*)malloc(sizeof(vecteur));
    vec_fg->donnees_individus=(double*)malloc(sizeof(double)*(noeud->individus->logic_size));



        for(int j=0;j<noeud->individus->logic_size;j++)
        {
            if(noeud->donnees->matrice[j][variable]<=mediane)
        {


            vec_fg->donnees_individus[compt]=noeud->donnees->matrice[j][0];
            compt+=1;
        }
        }

    vec_fg->logic_size=compt;
    return vec_fg;
}


vecteur * vec_fd(noeud * noeud,int variable,double mediane)
{
    int compt=0;
    vecteur * vec_fd=(vecteur*)malloc(sizeof(vecteur));
    vec_fd->donnees_individus=(double*)malloc(sizeof(double)*(noeud->individus->logic_size));


        for(int j=0;j<noeud->individus->logic_size;j++)
        {
            if(noeud->donnees->matrice[j][variable]>mediane)
        {


            vec_fd->donnees_individus[compt]=noeud->donnees->matrice[j][0];
            compt+=1;
        }
        }

    vec_fd->logic_size=compt;
    return vec_fd;
}



//----------------------------------------------------------
// Fonctions d'affichage du menu
//----------------------------------------------------------

void afficher_hauteur(noeud * arbre)
{
    int h=hauteur_arbre(arbre);
    printf("La hauteur de l'arbre est: %d\n",h);

}


void afficher_largeur(noeud * arbre)
{
    int l=largeur_arbre(arbre);
    printf("La largeur de l'arbre est: %d\n",l);
}


// Affichage récursif <=> parcours préfixé
// Précondition : l'arbre peut-être NULL (dans ce cas rien ne s'affiche)
// Initialisation : offset = 0
void affichage_arborescence(noeud * arbre, int offset)
{
	if(arbre != NULL)
	{
		// Etape 1 - afficher la valeur
		printf("\n");
		afficher_offset(offset);
		if( offset != 0 ) // tous les éléments sauf la racine
		{
			printf("|-");
		}
		printf("precision:%lf\n",arbre->precision);
		printf("\tnombre d'individu:%d\n",arbre->individus->logic_size);
		printf("\tX%d %s %lf\n",arbre->index_colonne,arbre->signe_division,arbre->mediane_corrigee);


		// Etape 2 - appel récursif avec sous-arbre gauche
			// Si à gauche (et uniquement à gauche) c'est NULL on affiche "|-x"
			if( !est_feuille(arbre) && (arbre->fils_gauche == NULL) )
			{
				printf("\n");
				afficher_offset(offset+1);
				printf("|-x");
			}
		affichage_arborescence(arbre->fils_gauche, offset+1);


		// Etape 2 - appel récursif avec sous-arbre de droite

			// Si à droite (et uniquement à droite) c'est NULL on affiche "|-x"
			if( !est_feuille(arbre) && (arbre->fils_droite == NULL) )
			{
				printf("\n");
				afficher_offset(offset+1);
				printf("|-x");
			}
		affichage_arborescence(arbre->fils_droite, offset+1);
	}
	//else <=> arrêt de la récursivité
}


void affichage_feuille(noeud * noeud)
{
    if(est_feuille(noeud))
    {
        printf("precision: %lf\n",noeud->precision);
        printf("nombre d'individus: %d\n",noeud->individus->logic_size);
        chemin_feuille(noeud);
        printf("\n");
    }
    if(noeud->fils_gauche!=NULL)
    {
    affichage_feuille(noeud->fils_gauche);
    }
    if(noeud->fils_droite!=NULL)
   {
       affichage_feuille(noeud->fils_droite);
   }
}








//fonction pour la question du rapport
void afficher_individus(noeud * noeud)
{
    for(int i=0;i<noeud->individus->logic_size;i++)
    {
        printf("%d\n",noeud->individus->donnees_individus[i]);
    }
}








