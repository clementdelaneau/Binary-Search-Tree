#ifndef DONNEES_H
#define DONNEES_H




//----------------------------------------------------------
// Structures
//----------------------------------------------------------

typedef struct
{
	unsigned int nb_colonnes; // la 1ere <=> classe � pr�dire (Y). Autres colonnes <=> variables d'observatio (Xi)
	unsigned int nb_lignes;   // <=> les individus
	double** matrice;         // tableau de tableaux de r�els (i.e. tableaux 2D de r�els)
} matrice_donnees;


typedef struct
{
    int logic_size;
    int * donnees_individus;
}vecteur;



typedef struct _noeud
{
    matrice_donnees * donnees;
    double y;
    int index_colonne;
    double mediane_corrigee;
    vecteur * individus;
    double precision;
    char * signe_division;
    struct _noeud * parent;
    struct _noeud * fils_gauche;
    struct _noeud * fils_droite;
}noeud;



//----------------------------------------------------------
// Initialisation des structures
//----------------------------------------------------------

matrice_donnees* charger_donnnees(const char* nom_fichier);
matrice_donnees* liberer_donnees(matrice_donnees * data);
vecteur * create_vecteur(matrice_donnees * data);
noeud * creer_noeud(matrice_donnees * data, double variable_a_predire,vecteur * newvect,int col,double med,char * signe,noeud * par);



//----------------------------------------------------------
// Cr�ation de l'arbre
//----------------------------------------------------------

void creer_arbre(noeud * parent,matrice_donnees * data,double variable_a_predire,int taille_max,int nb_min,double seuil_min,double seuil_max);



//----------------------------------------------------------
// Fonctions "classiques" utilis�es pour le programme
//----------------------------------------------------------

int max(int a,int b);  //retourne le maximum entre deux entiers
double max_tab(double tableau[], int taille); //retourne l'�l�ment maximum dans un tableau de doubles
void selection(double *t, int taille); //tri selection d'un tableau de doubles
void afficher_tableau_iteratif(double * tableau, int taille);//affiche dans l'ordre tous les �l�ments d'un tableau de doubles
void afficher_offset(int offset); // affiche un certain nombre d'espaces, proportionnel � offset (fonction utilis�e dans la fonction permettant l'affichage en arborescence de l'arbre


//----------------------------------------------------------
// Fonctions principales
//----------------------------------------------------------

bool est_divisible(noeud * arbre,int taille_max,int nb_min,double seuil_min,double seuil_max,double valeur_a_predire); //indique si un noeud est divisible
int hauteur_arbre(noeud * arbre); //retourne la hauteur de l'arbre pass� en param�tre
int largeur_arbre(noeud *element);//retourne la largeur de l'arbre, i.e le nombre de feuilles
double precision_noeud(noeud * arbre,double valeur_a_predire); //affiche la pr�cision de l'arbre pass� en param�tre
double * recuperer_colonne(noeud * arbre,int variable_xi); //r�cup�re la colonne de la matrice correspondant � la variable Xi entr� en param�tre et retourne un tableau de cette colonne
double mediane(double * tab,int taille); //retourne la m�diane d'un tableau (de doubles) tri�
double mediane_corrigee(double * tab,int taille); //retourne la m�diane corrig�e d'un tableau (de doubles) tri�
double precision_variable_gauche(noeud * arbre,matrice_donnees * data,int variable,double valeur_a_predire);//retourne la pr�cision des �l�ments de la colonne "variable" inf�rieurs ou �gaux � la m�diane corrig�e
double precision_variable_droite(noeud * arbre,matrice_donnees * data,int variable,double valeur_a_predire);//retourne la pr�cision des �l�ments de la colonne "variable" sup�rieurs � la m�diane corrig�e
int variable_meilleure_division(noeud * arbre,matrice_donnees * data,double valeur_a_predire);// retourne la variable avec la meilleur pr�cision pour le noeud pass� en param�tre
bool est_feuille(noeud const * element);//indique si le noeud pass� en param�tre est une feuille
char * critere_de_division(noeud * noeud);//retourne le crit�re de division utilis� pour arriver au noeud pass� en param�tre
void chemin_feuille(noeud * noeud);//retourne le chemin (� l'envers !) en terme de crit�re de division permettant d'arriver au noeud pass� en param�tre
vecteur * vec_fg(noeud * noeud,int variable,double mediane);//cr�� un nouveau vecteur pour le fils_gauche contenant les nouveaux �l�ments et la nouvelle taille
vecteur * vec_fd(noeud * noeud,int variable,double mediane);//cr�e un nouveau vecteur pour le fils droit contenant les nouveaux �l�ments et la nouvelle taille



//----------------------------------------------------------
// Fonctions d'affichage du menu
//----------------------------------------------------------
void afficher_hauteur(noeud * arbre);
void afficher_largeur(noeud * arbre);
void affichage_arborescence(noeud * arbre, int offset);
void affichage_feuille(noeud * noeud);





#endif // ABR_H_INCLUDED
