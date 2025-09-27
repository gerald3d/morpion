#ifndef DEF_Jeu
#define DEF_Jeu

#include "Define.h"
#include "morp_sdl.h"

/* typedef struct{ */
/*   int N;//dim horizontale */
/*   int M;//dim verticale */
/*   int DimCase; //Taille des cases carrées */
/*   int NbJetonsAAligner; */
/*   jeu TypeJeu; */
/* } DimPlateau; */

/* typedef struct { */
/*   int NumeroJEnCours;//0 ou 1 */
/*   int NbCasesJouees; */
/*   int *HistoriqueJeton; //liste les cases des jetons deja joués */
/*   int *ListeCasesVides; //liste les numeros de cases vides.Si 20 cases vides alors le tableau a 5*20=100 cases ([0->19 : toutes les cases vides][20->39 : cases vides de couronne 1][40->59 : cases vides de couronne 2][60->79 cases vides de couronne 3][80->99 cases vides de couronne 4] */
/*   //   int *ListeCasesVides_C0; //liste les numeros de cases vides */
/*   /\*   int *ListeCasesVides_C1; //liste les numeros de cases vides */
/*        int *ListeCasesVides_C2; //liste les numeros de cases vides */
/*        int *ListeCasesVides_C3; */
/*        int *ListeCasesVides_C4;*\/ */
/*   int taille; */
/*   int *tabMeilleuresCases; //liste les cases qui ont le meilleur score pour l'ia */
/*   int NbMeilleuresCases; */
/*   int PartieFinie; */
/* } CaracteristiqueJeu; */

int alea_entre_bornes(int a, int b);
int maximumEntre(int a, int b);
int minimumEntre(int a, int b);
void maximumTableau(int tableau[], int taille, int *ValeurMax, int *NbMax);
int maxTab(int tableau[], int taille);
int minTab(int tableau[], int taille);
void TabCroissant(int tableau[], int taille);

zone QuelleZoneCliquee(DimPlateau DimJeu,SDL_Rect Dimension, Joueur Joueur, CaracteristiqueJeu Jeu);

/* Alloue un tableau de taille size.
 * Si une erreur survient renvoie NULL */
void *FonctionAllocation1D(unsigned short dimension, size_t size, t_logs *logs);

void JouerCoup(Case *plateau, Joueur Joueur, CaracteristiqueJeu *Jeu, int NumeroCase); //Fonction pour jouer NumeroCasesur le plateau
void OrdiJoue(Case *plateau, Joueur Joueur, CaracteristiqueJeu *Jeu, DimPlateau DimJeu, t_logs *logs); //Gestion du jeu lors d'un clic
void HumainJoue(Case *plateau, Joueur Joueur, CaracteristiqueJeu *Jeu, DimPlateau DimJeu, SDL_Rect clic); //Gestion du jeu lors d'un clic
void AnnulerCoup(Case *plateau, CaracteristiqueJeu *Jeu); //Fonction pour annuler les derniers coups

void verifFini(Case *plateau, Joueur Joueur, DimPlateau DimJeu, CaracteristiqueJeu *Jeu, int *NbAlignements); //Fonction pour vérifier si la partie est terminée

int VerifAlignement(Case *plateau, DimPlateau DimJeu, int CaseDebut, int regleX, int regleY, Case icone); //Fonction pour vérifier si la partie est terminée
void AnnulerVerifFini(Case *plateau, CaracteristiqueJeu *Jeu, DimPlateau DimJeu); //Fonction annule les changements de Rond en RondGagne et RondPerdu; ainsi que les changements de Croix en CroixGagne et CroixPerdu

void DefinirZoneEtendue(CaracteristiqueJeu Jeu, DimPlateau DimJeu, Couronne *InfoZoneDeRecherche);
void DefinirListeCaseVide(CaracteristiqueJeu *Jeu, DimPlateau DimJeu,Couronne InfoZoneDeRecherche[], t_logs *logs); //Remplit dresse la liste des cases vides dans la ZoneDeRecherche

//void DefinirListeCaseVide(CaracteristiqueJeu *Jeu, DimPlateau DimJeu,Info InfoZoneDeRecherche); //Remplit dresse la liste des cases vides dans la ZoneDeRecherche
//void DefinirZoneEtendue(CaracteristiqueJeu Jeu, DimPlateau DimJeu, Info *InfoZoneDeRecherche);

void ConfigType(t_morp_sdl *sdl, jeu *TypeJeu, Joueur *Joueur, int x, int y); //Determine les action si clic sur bouton Type : modif Type ou pion
void ConfigTypeJeu(jeu *TypeJeu, int zone, Joueur *Joueur); //Determine le type de jeu et change l'affichage H et O
void ConfigTypePion(t_morp_sdl *sdl, int zone, Joueur *Joueur); //determine les jetons a utiliser
void ConfigTypeMusique(t_morp_sdl *sdl, SDL_Rect positionAffichage); //Determine la musique

void ConfigDimension(t_morp_sdl *sdl, Joueur Joueur, DimPlateau *DimJeu, CaracteristiqueJeu Jeu); //modifie les dimensions et le nombre de jetons a aligner
void ModificationConfigDimension(DimPlateau AncienDimJeu, DimPlateau NouveauDimJeu, SDL_Rect *DimensionPlateauJeu, Case **plateau, CaracteristiqueJeu *Jeu, t_logs *logs);

int QuelleZoneTournoiCliquee(int x, int y, SDL_Rect *PositionZonesCliquables);

void JouerMusique(Musique Musique);
#endif

