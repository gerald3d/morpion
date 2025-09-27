#ifndef H_DEFINE
#define H_DEFINE

/**************************************/
#define EcranLarg 800 //dimensions de l'écran
#define EcranHaut 602

#define zonePlateauLarg 600 //dimensions de la zone carrée où l'on affiche le plateau. Le plateau de jeu peut etre carré ou rectangulaire, donc il faut modifier le pointeur en fleche lorsque l'on pointe sur la zone, hors du plateau
//#define PlateauLarg 600 /*DimPlateau.N*DimPlateau.DimCase;
//DimPlateau.M*DimPlateau.DimCase;*/ //dimensions du plateau a afficher
//#define PlateauHaut 600

#define PlateauLarg DimJeu.DimCase*DimJeu.N
#define PlateauHaut DimJeu.DimCase*DimJeu.M

#define BoutonX (zonePlateauLarg+25)//affiche les boutons les un sous les autres, a droite du plateau de jeu
#define BoutonLargeur 150
#define BoutonHauteur 50
#define EcartBouton 100

#define BoutonYNouveau 50
#define BoutonYAnnuler (BoutonYNouveau+EcartBouton)
#define BoutonYCoupDePouce (BoutonYAnnuler+BoutonHauteur)
#define BoutonYConfigType (BoutonYCoupDePouce+BoutonHauteur)
#define BoutonYConfigDimension (BoutonYConfigType+BoutonHauteur)
#define BoutonYTournoi (BoutonYConfigDimension+BoutonHauteur)
#define BoutonYValider (BoutonYTournoi+BoutonHauteur)
#define BoutonYInstruction (BoutonYValider+BoutonHauteur)
#define BoutonYQuitter (BoutonYInstruction+BoutonHauteur)

#define EpaisseurTraitCadre 1 //pour le cadre des boutons
#define EpaisseurTraitJeu 2 //

#define FAUX 0
#define VRAI 1

#define NON 0
#define OUI 1

#define ERREUR 1
#define SUCCES 0

/**************************************/
#define couleurImageBlanc 255, 255, 255 // pour rendre la couleur blanche transparente
#define couleurImageMarron 187, 122, 87 //couleur image
#define couleurImageNoir 0, 0, 0 //couleur image
#define couleurImageGris 195, 195, 195 //couleur image

#define coulJaune 255, 200, 14 //couleur image

#define couleurTexteNoir (SDL_Color){0, 0, 0, 0} //couleur d'ecriture
#define couleurTexteMarron (SDL_Color){187, 122, 87, 0} //couleur image
#define couleurTexteRouge (SDL_Color){255, 0, 0, 0} //couleur d'ecriture
#define couleurTexteGris (SDL_Color){195, 195, 195, 0} //couleur d'ecriture

#define NbMatchMax 10 //Nombre maximal de matches en tournoi impossible a depasser

#define NbFamillesPion 6
#define NbPionsParFamille 4
#define nbPions (NbFamillesPion*NbPionsParFamille)

#define NbMusiques 8 //nombre total de musique y compris la musique null
#define NbMusiquesMax 8 //nombre total de musique y compris la musique null


#define NbZonesCliquablesParLigneTournoi 7 //ZC_Manche, ZC_J1Icone, ZC_J1type, ZC_J2Icone= ZC_J2type,ZC_Musique,ZC_Dim
#define NbZonesAffichablesParLigneTournoi 8
#define NbLignesTournoi 10

/**************************************/
/* enum Case {Vide, Rond, Croix, xGagne, xPerd, oGagne, oPerd, CoupDePouce}; //affiche une case vide, avec un rond; une croix ou croix-rond gagnant-perdant */
/* typedef enum Case Case; */

enum jeu {HH=0, HO=1, OH=2, OO=3, VV=4}; //definit les modes de jeu Humain/Humain ou Humain/Ordi, ...
typedef enum jeu jeu;

/* enum joueur {H, O}; //joueur humain ou ordi ? */
/* typedef enum joueur joueur; */

enum zone {JEU, NOUVEAU, ANNULER, COUPDEPOUCE, CONFIGTYPE, CONFIGDIMENSION, TOURNOI, VALIDER, INSTRUCTION, QUITTER, MARGE};
typedef enum zone zone;

enum EtatBouton {ACTIF=0, INACTIF=1, INVISIBLE=2}; //pour afficher les boutons en gris ou en noir
typedef enum EtatBouton EtatBouton;

enum EcranBouton {EcranJeu, EcranConfig, EcranInstruction, EcranTournoi}; //pour savoir quels boutons sont accessibles sur cette page
typedef enum EcranBouton EcranBouton;

enum ZC {ZC_Manche=0, ZC_J1Icone=1, ZC_J1type=2, ZC_J2Icone=3, ZC_J2type=4,ZC_Musique=5,ZC_Dim=6}; //pour savoir quelle ZoneCliquable est accessible
typedef enum ZC ZC; //zone cliquable

/**************************************/
#define NbImagesMax 11 //11 images qui ne sont pas des Pions
#define NbImagesPionMax 24 //24 Pions
#define NbImagesTotalMax (NbImagesMax+NbImagesPionMax) //nombre total image + pion
#define NbTexteMessageMax 15 //pour le texte de l'aide et le tournoi, (pas les boutons)en noir (pas de gris),non souligne
#define NbTexteBoutonMax (7*5+1) //=36pour le texte des boutons en souligne ou pas et noir ou gris (7 boutons et 5 comb + vide)

#define i_BoutonConfigDimension 0
#define i_BoutonConfigType 1
#define i_CaseGrise 2
#define i_CoupeGrande 3
#define i_CoupePetite 4
#define i_Curseur 5
#define i_CurseurAxe 6
#define i_CurseurCroixGris 7
#define i_CurseurCroixNoir 8
#define i_Musique 9
#define i_Titre 10

#define Texte_Aide0 0// Pour jouer une partie de morpion classique,
#define Texte_Aide1 1//il suffit d'aligner (verticalement, horizontalement,
#define Texte_Aide2 2//ou diagonalement) trois jetons.
#define Texte_Aide3 3//Ce bouton permet d'annuler plusieurs ----------->
#define Texte_Aide4 4//derniers coups joués. ------------------------------->
#define Texte_Aide5 5//Ce bouton permet de jouer contre l'ordinateur,
#define Texte_Aide6 6//de choisir son jeton, de modifier la musique. -->
#define Texte_Aide7 7//Ce bouton permet de changer le nombre ------->
#define Texte_Aide8 8//de cases et le nombre de jetons à aligner. ------->
#define Texte_Aide9 9//Ce bouton permet de définir un tournoi de ----->
#define Texte_Aide10 10//10 manches maximum. ---------------------------->
#define Texte_TournoiManche 11
#define Texte_TournoiJoueur1 12
#define Texte_TournoiJoueur2 13
#define Texte_TournoiDimensions 14

#define Texte_Annuler 0//noir
#define Texte_A 1//noir souligne
#define Texte_nnuler 2//noir normal
#define Texte_A_gris 3//gris souligne
#define Texte_nnuler_gris 4//gris normal
#define Texte_CoudPouS 5//noir
#define Texte_C 6//noir souligne
#define Texte_oudPouS 7//noir normal
#define Texte_C_gris 8//gris souligne
#define Texte_oudPouS_gris 9//gris normal
#define Texte_Instruction 10//noir
#define Texte_I 11//noir souligne
#define Texte_nstruction 12//noir normal
#define Texte_I_gris 13//gris souligne
#define Texte_nstruction_gris 14//gris normal
#define Texte_Nouveau 15//noir
#define Texte_N 16//noir souligne
#define Texte_ouveau 17//noir normal
#define Texte_N_gris 17//gris souligne
#define Texte_ouveau_gris 19//gris normal
#define Texte_Quitter 20//noir
#define Texte_Q 21//noir souligne
#define Texte_uitter 22//noir normal
#define Texte_Q_gris 23//gris souligne
#define Texte_uitter_gris 24//gris normal
#define Texte_Tournoi 25//noir
#define Texte_T 26//noir souligne
#define Texte_ournoi 27//noir normal
#define Texte_T_gris 28//gris souligne
#define Texte_ournoi_gris 29//gris normal
#define Texte_Valider 30//noir
#define Texte_V 31//noir souligne
#define Texte_alider 32//noir normal
#define Texte_V_gris 33//gris souligne
#define Texte_alider_gris 34//gris normal
#define Texte_Vide 35


/**************************************/
#define ColleTexture SDL_RenderCopy

/**************************************/
/* typedef struct IdentiteJoueur IdentiteJoueur; */
/* struct IdentiteJoueur */
/* { */
/*     joueur joueur; //H ou O */
/*     Case jeton; //CROIX ou ROND */
/*     int NumeroPion;// de 0 a 23 */
/* }; */

/* typedef struct Joueur Joueur; */
/* struct Joueur */
/* { */
/*     IdentiteJoueur ListeJ[2]; //Pour J1 et J2 */
/* }; */

typedef struct Couronne Couronne ;
struct Couronne
{
    int CaseDepart;//case en haut a gauche de la couronne
    int Largeur;//largeur de la couronne
    int Hauteur;//hauteur de la couronne
};

typedef struct Info Info;
struct Info
{
 /*   int CaseDepart_C0;
    int Largeur_C0;
    int Hauteur_C0;*/
    int CaseDepart_C1;//couronne de 1 case
    int Largeur_C1;//couronne de 1 case
    int Hauteur_C1;//couronne de 1 case
    int CaseDepart_C2;//couronne de 2 cases
    int Largeur_C2;//couronne de 2 cases
    int Hauteur_C2;//couronne de 2 cases
};

/* typedef struct DimPlateau DimPlateau; */
/* struct DimPlateau */
/* { */
/*     int N;//dim horizontale */
/*     int M;//dim verticale */
/*     int DimCase; //Taille des cases carrées */
/*     int NbJetonsAAligner; */
/*     jeu TypeJeu; */
/* }; */

/* typedef struct CaracteristiqueJeu CaracteristiqueJeu; */
/* struct CaracteristiqueJeu */
/* { */
/*     int NumeroJEnCours;//0 ou 1 */
/*     int NbCasesJouees; */
/*     int *HistoriqueJeton; //liste les cases des jetons deja joués */
/*     int *ListeCasesVides; //liste les numeros de cases vides.Si 20 cases vides alors le tableau a 5*20=100 cases ([0->19 : toutes les cases vides][20->39 : cases vides de couronne 1][40->59 : cases vides de couronne 2][60->79 cases vides de couronne 3][80->99 cases vides de couronne 4] */
/*  //   int *ListeCasesVides_C0; //liste les numeros de cases vides */
/*  /\*   int *ListeCasesVides_C1; //liste les numeros de cases vides */
/*     int *ListeCasesVides_C2; //liste les numeros de cases vides */
/*     int *ListeCasesVides_C3; */
/*     int *ListeCasesVides_C4;*\/ */
/*     int taille; */
/*     int *tabMeilleuresCases; //liste les cases qui ont le meilleur score pour l'ia */
/*     int NbMeilleuresCases; */
/*     int PartieFinie; */
/* }; */

typedef struct Musique Musique;
struct Musique
{
    Mix_Music *Liste[NbMusiquesMax];
    int Actuel;
    int Ancien;
};

typedef struct MancheTournoi MancheTournoi;
struct MancheTournoi
{
    int numeroDeManche; //entre 1 et 10
    /* IdentiteJoueur Joueur1; //H ou O et ROND ou CROIX */
    /* IdentiteJoueur Joueur2; //H ou O et ROND ou CROIX */
    jeu TypeJeu; // determine le type HH, HO, OH, OO, VIDE
//    int NumeroMusique;
    Musique Musique;
    int dimN; //largeur
    int dimM; //hauteur
    int NbJetonsAAligner; //Nombre de jetons a aligner pour gagner
};

/**************************************/

#endif


