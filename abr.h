#ifndef DONNEES_H
#define DONNEES_H




//----------------------------------------------------------
// Structures
//----------------------------------------------------------

typedef struct
{
	unsigned int nb_colonnes; // la 1ere <=> classe à prédire (Y). Autres colonnes <=> variables d'observatio (Xi)
	unsigned int nb_lignes;   // <=> les individus
	double** matrice;         // tableau de tableaux de réels (i.e. tableaux 2D de réels)
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
// Création de l'arbre
//----------------------------------------------------------

void creer_arbre(noeud * parent,matrice_donnees * data,double variable_a_predire,int taille_max,int nb_min,double seuil_min,double seuil_max);



//----------------------------------------------------------
// Fonctions "classiques" utilisées pour le programme
//----------------------------------------------------------

int max(int a,int b);  //retourne le maximum entre deux entiers
double max_tab(double tableau[], int taille); //retourne l'élèment maximum dans un tableau de doubles
void selection(double *t, int taille); //tri selection d'un tableau de doubles
void afficher_tableau_iteratif(double * tableau, int taille);//affiche dans l'ordre tous les élèments d'un tableau de doubles
void afficher_offset(int offset); // affiche un certain nombre d'espaces, proportionnel à offset (fonction utilisée dans la fonction permettant l'affichage en arborescence de l'arbre


//----------------------------------------------------------
// Fonctions principales
//----------------------------------------------------------

bool est_divisible(noeud * arbre,int taille_max,int nb_min,double seuil_min,double seuil_max,double valeur_a_predire); //indique si un noeud est divisible
int hauteur_arbre(noeud * arbre); //retourne la hauteur de l'arbre passé en paramètre
int largeur_arbre(noeud *element);//retourne la largeur de l'arbre, i.e le nombre de feuilles
double precision_noeud(noeud * arbre,double valeur_a_predire); //affiche la précision de l'arbre passé en paramétre
double * recuperer_colonne(noeud * arbre,int variable_xi); //récupère la colonne de la matrice correspondant à la variable Xi entré en paramètre et retourne un tableau de cette colonne
double mediane(double * tab,int taille); //retourne la médiane d'un tableau (de doubles) trié
double mediane_corrigee(double * tab,int taille); //retourne la médiane corrigée d'un tableau (de doubles) trié
double precision_variable_gauche(noeud * arbre,matrice_donnees * data,int variable,double valeur_a_predire);//retourne la précision des élèments de la colonne "variable" inférieurs ou égaux à la médiane corrigée
double precision_variable_droite(noeud * arbre,matrice_donnees * data,int variable,double valeur_a_predire);//retourne la précision des élèments de la colonne "variable" supérieurs à la médiane corrigée
int variable_meilleure_division(noeud * arbre,matrice_donnees * data,double valeur_a_predire);// retourne la variable avec la meilleur précision pour le noeud passé en paramètre
bool est_feuille(noeud const * element);//indique si le noeud passé en paramètre est une feuille
char * critere_de_division(noeud * noeud);//retourne le critère de division utilisé pour arriver au noeud passé en paramètre
void chemin_feuille(noeud * noeud);//retourne le chemin (à l'envers !) en terme de critère de division permettant d'arriver au noeud passé en paramètre
vecteur * vec_fg(noeud * noeud,int variable,double mediane);//créé un nouveau vecteur pour le fils_gauche contenant les nouveaux élèments et la nouvelle taille
vecteur * vec_fd(noeud * noeud,int variable,double mediane);//crée un nouveau vecteur pour le fils droit contenant les nouveaux élèments et la nouvelle taille



//----------------------------------------------------------
// Fonctions d'affichage du menu
//----------------------------------------------------------
void afficher_hauteur(noeud * arbre);
void afficher_largeur(noeud * arbre);
void affichage_arborescence(noeud * arbre, int offset);
void affichage_feuille(noeud * noeud);





#endif // ABR_H_INCLUDED
