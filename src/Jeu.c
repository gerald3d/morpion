#include "Main.h"

/*********************************************/
int alea_entre_bornes(int a, int b) //choisit un nombre entre a (inclu) et b (inclu)
{
    int r;

    r=rand()%(b-a+1)+a;
    return r;
}

/**********************/

int maximumEntre(int a, int b)
{
    if (a<b)
        return b;
    else return a;
}

/**********************/
int minimumEntre(int a, int b)
{
    if (a<b)
        return a;
    else return b;
}

/*********************************************/
void maximumTableau(int tableau[], int taille, int *ValeurMax, int *NbMax) //Recherche la valeur maximale d'un tableau et le nombre de fois qu'eelle apparait.
{
    int i;

    *ValeurMax=-100000;

    for (i=0; i<taille; i++) //pour toutes les cases vides du plateau,
    {
        if(tableau[i]!=-1)
        {
            if((*ValeurMax)<tableau[i])
            {
                *ValeurMax = tableau[i];
                *NbMax=1;
            }
            else
            {
                if((*ValeurMax)==tableau[i])
                    *NbMax=(*NbMax)+1;
            }
        }
    }
}

/**********************/
int maxTab(int tableau[], int taille) //Recherche la valeur maximale d'un tableau
{
    int i;
    int ValeurMax=tableau[0];

    for (i=1; i<taille; i++) //pour toutes les cases vides du plateau,
    {
        if(ValeurMax<tableau[i])
            ValeurMax = tableau[i];
    }
    return ValeurMax;
}

/**********************/

int minTab(int tableau[], int taille) //Recherche la valeur maximale d'un tableau
{
    int i;
    int ValeurMin=tableau[0];

    for (i=1; i<taille; i++) //pour toutes les cases vides du plateau,
    {
        if(ValeurMin>tableau[i])
            ValeurMin = tableau[i];
    }
    return ValeurMin;
}

/**********************/
void TabCroissant(int tableau[], int taille)
{
    int a;//valeur pour permuter
    int i;
    int NbPermutation=1;//[=0 : aucune permutation n'a eu lieu, c'est fini][!=0 : une permutation a eu lieu, on continue]

    /*int t[9];
    for (i=0; i<9; i++)
    t[i]=tableau[i];*/

    while (NbPermutation!=0)//tant que l'on a permuté au moins deux cases, on continue
    {
        NbPermutation=0;
        for (i=0; i<taille-1; i++) //pour toutes les cases vides du plateau,
        {
            if(tableau[i]>tableau[i+1])
            {
                a=tableau[i];
                tableau[i]=tableau[i+1];
                tableau[i+1]=a;
                NbPermutation=NbPermutation+1;
            }
        }
    }
    /*for (i=0; i<9; i++)
    t[i]=tableau[i];*/
}

/**********************/
zone QuelleZoneCliquee(DimPlateau DimJeu,SDL_Rect clic, Joueur Joueur, CaracteristiqueJeu Jeu)
{
    zone zoneRenvoyee=MARGE;

    if((clic.x<=PlateauLarg/*clic.w*/)&&(clic.y<=PlateauHaut/*clic.h*/)&&(Joueur.ListeJ[Jeu.NumeroJEnCours].joueur==H)) //Si on a clique dans le plateau de jeu et que c'est H qui joue, alors on joue
        zoneRenvoyee = JEU;
    if((BoutonX<clic.x)&&(clic.x<BoutonX+BoutonLargeur)&&(BoutonYNouveau<clic.y)&&(clic.y<BoutonYNouveau+BoutonHauteur)) //Si on clique sur le bouton Nouveau, alors
        zoneRenvoyee = NOUVEAU;
    if((BoutonX<clic.x)&&(clic.x<BoutonX+BoutonLargeur)&&(BoutonYAnnuler<clic.y)&&(clic.y<BoutonYAnnuler+BoutonHauteur)) //Si on clique sur le bouton Annuler, alors
        zoneRenvoyee= ANNULER;
    if((BoutonX<clic.x)&&(clic.x<BoutonX+BoutonLargeur)&&(BoutonYCoupDePouce<clic.y)&&(clic.y<BoutonYCoupDePouce+BoutonHauteur)) //Si on clique sur le bouton Annuler, alors
        zoneRenvoyee= COUPDEPOUCE;
    if((BoutonX<clic.x)&&(clic.x<BoutonX+BoutonLargeur)&&(BoutonYConfigType<clic.y)&&(clic.y<BoutonYConfigType+BoutonHauteur)) //Si on clique sur le bouton ConfigType, alors
        zoneRenvoyee =CONFIGTYPE;
    if((BoutonX<clic.x)&&(clic.x<BoutonX+BoutonLargeur)&&(BoutonYConfigDimension<clic.y)&&(clic.y<BoutonYConfigDimension+BoutonHauteur)) //Si on clique sur le bouton ConfigDim, alors
        zoneRenvoyee= CONFIGDIMENSION;
    if((BoutonX<clic.x)&&(clic.x<BoutonX+BoutonLargeur)&&(BoutonYTournoi<clic.y)&&(clic.y<BoutonYTournoi+BoutonHauteur)) //Si on clique sur le bouton Tournoi, alors
        zoneRenvoyee =TOURNOI;
    if((BoutonX<clic.x)&&(clic.x<BoutonX+BoutonLargeur)&&(BoutonYValider<clic.y)&&(clic.y<BoutonYValider+BoutonHauteur)) //Si on clique sur le bouton Instruction, alors
        zoneRenvoyee= VALIDER;
    if((BoutonX<clic.x)&&(clic.x<BoutonX+BoutonLargeur)&&(BoutonYInstruction<clic.y)&&(clic.y<BoutonYInstruction+BoutonHauteur)) //Si on clique sur le bouton Instruction, alors
        zoneRenvoyee= INSTRUCTION;
    if((BoutonX<clic.x)&&(clic.x<BoutonX+BoutonLargeur)&&(BoutonYQuitter<clic.y)&&(clic.y<BoutonYQuitter+BoutonHauteur)) //Si on clique sur le bouton Quitter, alors
        zoneRenvoyee= QUITTER;

    return zoneRenvoyee;
}

/**********************/

void *FonctionAllocation1D(unsigned short dimension, size_t size, t_logs *logs)
{
    void *tableau=calloc(dimension, size); //Ce tableau doit etre dynamique et initialisé à 0
    if (tableau != NULL) return tableau;

    FILE *descripteur = logs_descripteur_fichier (logs, LOG_ERROR);
    fprintf(descripteur, "Erreur d'allocation du tableau 1D : %s\n", SDL_GetError());

    return NULL;
}

/*********************************************/
void JouerCoup(Case *plateau, Joueur Joueur, CaracteristiqueJeu *Jeu, int NumeroCase) //Fonction pour jouer NumeroCasesur le plateau
{
    int i;


    plateau[NumeroCase] = (Joueur.ListeJ[(Jeu->NbCasesJouees)%2]).jeton;//place le jeton du joueur 0 ou 1 selon la liste
    Jeu->HistoriqueJeton[Jeu->NbCasesJouees]=NumeroCase;
    Jeu->NbCasesJouees=(Jeu->NbCasesJouees)+1;
    Jeu->NbMeilleuresCases=0;
    Jeu->NumeroJEnCours =(Jeu->NbCasesJouees)%2;

    for (i=0; i<Jeu->taille; i++)
    {
        Jeu->tabMeilleuresCases[i]=-1;
        if(plateau[i] == CoupDePouce) //annulation des cases CoupDePouce
            plateau[i] = Vide;
    }
}

/*********************************************/

void HumainJoue(Case *plateau, Joueur Joueur, CaracteristiqueJeu *Jeu, DimPlateau DimJeu, SDL_Rect clic) //Gestion du jeu de l'Humain lors d'un clic
{
    int i, j; //case correspondante au clic souris

    i = clic.x/DimJeu.DimCase; //Calcul de la case associée
    j = clic.y/DimJeu.DimCase;

    if((plateau[i+j*DimJeu.N]==Vide)||(plateau[i+j*DimJeu.N]==CoupDePouce)) //Si la case est vide,
        JouerCoup(plateau, Joueur, Jeu, i+j*DimJeu.N); //Joue le coup selectionné
}

/*********************************************/

void OrdiJoue(Case *plateau, Joueur Joueur, CaracteristiqueJeu *Jeu, DimPlateau DimJeu, t_logs *logs) //Gestion du jeu lorsque l'Ordi joue
{
    int NumeroCaseAJouer=-1;
    /**/    int tmc[20];//Jeu->tabMeilleuresCases[Jeu->NbMeilleuresCases]=
    /**/    int i,nmc;//=NbMeilleuresCases

    if(Jeu->NbCasesJouees==0) //si c'est le 1er coup de la partie, alors on choisit aleatoirement
    {
        NumeroCaseAJouer=alea_entre_bornes(0, DimJeu.N*DimJeu.M-1); //choisit aleatoirement une case sur tout le plateau
    }
    else
    {
      ia(plateau, Joueur, Jeu, DimJeu, logs); //l'ordi joue une case en analysant le jeu et cherchant la case la plus gagnante
        NumeroCaseAJouer=Jeu->tabMeilleuresCases[alea_entre_bornes(0, Jeu->NbMeilleuresCases-1)]; //on tire une case au hasard parmi les meilleures possibles

        for (i=0; i<20; i++)
        {
            /**/tmc[i]=Jeu->tabMeilleuresCases[i];
        }
        /**/     nmc=Jeu->NbMeilleuresCases;

    }
    JouerCoup(plateau, Joueur, Jeu, NumeroCaseAJouer); //Joue le coup selectionné
}

/**********************/

void AnnulerCoup(Case *plateau, CaracteristiqueJeu *Jeu) //Fonction pour annuler le dernier coup : HistoriqueJeton[*NbCasesJouees]
{
    int i;

    if(Jeu->NbCasesJouees!=0) //On ne peut annuler que si un pion est deja joué
    {
        Jeu->NbCasesJouees=(Jeu->NbCasesJouees)-1; //on a joué un jeton de moins
        plateau[Jeu->HistoriqueJeton[Jeu->NbCasesJouees]]=Vide; //On vide la case du plateau
        Jeu->HistoriqueJeton[Jeu->NbCasesJouees]=-1; //on enleve le dernier coup de l'historique
        Jeu->NumeroJEnCours = (Jeu->NbCasesJouees)%2;

        for (i=0; i<Jeu->taille; i++) //annulation des cases CoupDePouce
        {
            if(plateau[i] == CoupDePouce)
                plateau[i] = Vide;
        }
    }
}

/**********************/
void verifFini(Case *plateau, Joueur Joueur, DimPlateau DimJeu, CaracteristiqueJeu *Jeu, int *NbAlignements) //Fonction pour vérifier si la partie est terminée
{
    int i;
    int joueurEnCours=(Jeu->NbCasesJouees+1)%2;
    int NumeroCaseJouee =Jeu->HistoriqueJeton[(Jeu->NbCasesJouees)-1];
    Case icone=Joueur.ListeJ[joueurEnCours].jeton;
    int LigneATester=NumeroCaseJouee/DimJeu.N; //on determine la ligne correspondante à la case jouee pour verifier s'il y a un alignement realisé
    int ColonneATester=NumeroCaseJouee%DimJeu.N; //on determine la colonne correspondante à la case jouee pour verifier s'il y a un alignement realisé
    *NbAlignements=0; //On reinitialise le nombre d'alignements a chaque utilisation de la fonction verifFini
    int regleX; //pour indiquer comment on se deplace en ligne
    int regleY; //[colonne=0,1][ligne=1,0][DiagDescendante=1,1][DiagMontante=1,-1]
    int BorneDroite = minimumEntre(DimJeu.NbJetonsAAligner-1,ColonneATester);//nombre de reculs POSSIBLES vers la gauche (donc fin du for)
    int BorneGauche = maximumEntre(0,DimJeu.NbJetonsAAligner-DimJeu.N+ColonneATester);//nombre de reculs OBLIGATOIRES vers la gauche pour pouvoir tester un alignement ? (donc debut du for)
    int BorneBas  = maximumEntre(0,DimJeu.NbJetonsAAligner-DimJeu.M+LigneATester);
    int BorneHaut = minimumEntre(DimJeu.NbJetonsAAligner-1,LigneATester);

//Au lieu de faire l'ancienne vérification :
//A partir de la derniere  case jouée,on verifie les alignements verticuax-horizontaux-DiagMont-DiagDescendante, puis on recule d'une case pour retester
//Il ne faut pas sortir du tableau, donc il y a un test avant de verifier
//On teste aussi si on peut jouer tous les jetons sans sortir du tableau (pour eviter les problemes de cases trop a droite ou trop en bas
//Probleme : certains cas ne sont pas effectués (avec les if), si on est trop au bord pour tester l'alignement ou pour reculer
//Nouvelle version : a partir de la derniere case, on determine la Ligne et Colonne et on determine directement de combien on peut/doit reculer et avancer dans toutes les directions (cela change juste les bornes des for)
//et reduit le nombre de cas a traiter

    regleX=0;
    regleY=1;
    for (i=BorneBas; i<BorneHaut+1; i++) //On verifie les colonnes
        *NbAlignements=(*NbAlignements)+VerifAlignement(plateau, DimJeu, NumeroCaseJouee-i*DimJeu.N, regleX, regleY, icone); //alors on verifie l'alignement

    regleX=1;
    regleY=0;
    for (i=BorneGauche; i<BorneDroite+1; i++) //On verifie les lignes
        *NbAlignements=(*NbAlignements)+VerifAlignement(plateau, DimJeu, NumeroCaseJouee-i, regleX, regleY, icone);

    regleX=1;
    regleY=1;
    for (i=maximumEntre(BorneGauche,BorneBas); i<minimumEntre(BorneHaut,BorneDroite)+1; i++) //On verifie les diagonales descendantes
        *NbAlignements=(*NbAlignements)+VerifAlignement(plateau, DimJeu, NumeroCaseJouee-i*DimJeu.N-i, regleX, regleY, icone);

    regleX=1;
    regleY=-1;
    for (i=maximumEntre(BorneGauche,maximumEntre(0,DimJeu.NbJetonsAAligner-1-LigneATester)); i<minimumEntre(BorneDroite,minimumEntre(DimJeu.NbJetonsAAligner-1,DimJeu.M-1-LigneATester))+1; i++) //On verifie les diagonales montantes
        *NbAlignements=(*NbAlignements)+VerifAlignement(plateau, DimJeu, NumeroCaseJouee+i*DimJeu.N-i, regleX, regleY, icone);

    if ((Jeu->NbCasesJouees==DimJeu.N*DimJeu.M)||((*NbAlignements)!=0)) //Si le NbCasesJouees est tout le jeu (cad, pas de cases vides) (et pas d'alignement) ou qu'il y a un alignement (de Croix ou Rond)
    {
        Jeu->PartieFinie=OUI;//  *PartieFinie=OUI; //c'est fini s'il n'y a plus de case vide (casevide==NON) ou si il y a un alignement (*Partiefinie=OUI)

        for(i=0; i<DimJeu.N*DimJeu.M; i++)    //il faut indiquer perdu les pions non gagnants (de son équipe et de son adversaire)
        {
            //On parcourt le tableau pour trouver les pions non alignés,
            switch (plateau[i])
            {
            case Croix:
                plateau[i]=xPerd;
                break;
            case Rond:
                plateau[i]=oPerd;
                break;
            default: //les valeurs Vide, xGagne, xPerd, oGagne, oPerd ne sont pas changées
                break;
            }
        }
    }
}
/**********************/

int VerifAlignement(Case *plateau, DimPlateau DimJeu, int CaseDebut, int regleX, int regleY, Case icone) //A partir de la case initiale, on applique une des 4 regles (colonne, ligne, diagMontante, DiagDescendante)
{
    Case iconeAssocie;
    int NbCasesAlignees=0; //nombre de cases correctement alignées
    int aligne=NON; //[1 : OUI : tous les jetons sont alignés][0 : NON : tous les jetons ne sont pas alignés]
    int i=0; //il faut l'initialiser a chaque tour

    if (icone==Rond) //on determine le dernier jeton joué : O ou X et son jeton associé
        iconeAssocie=oGagne;
    else iconeAssocie=xGagne;

    while ((i<DimJeu.NbJetonsAAligner)&&((plateau[CaseDebut+i*(regleX+DimJeu.N*regleY)]==icone)||(plateau[CaseDebut+i*(regleX+DimJeu.N*regleY)]==iconeAssocie))) //a la place d'un "for" qui ferait aussi les cas pour les mauvais jetons
    {
        NbCasesAlignees=NbCasesAlignees+1; //On ajoute une case correctement alignée
        i=i+1;
    }

    if(NbCasesAlignees==DimJeu.NbJetonsAAligner) //Si toutes les cases verifiees sont du meme jeton,
    {
        aligne=OUI; //alors on signale que l'alignement est realisé
        for(i=0; i<DimJeu.NbJetonsAAligner; i++)
        {
            plateau[CaseDebut+i*(regleX+DimJeu.N*regleY)]=iconeAssocie; //On modifie le jeton pour l'afficher gagnant
        }
    }

    return aligne;
}
/**********************/

void AnnulerVerifFini(Case *plateau, CaracteristiqueJeu *Jeu, DimPlateau DimJeu) //Fonction annule les changements de Rond en RondGagne et RondPerdu; ainsi que les changements de Croix en CroixGagne et CroixPerdu
{
    int i;

    Jeu->PartieFinie=NON;
    //*PartieFinie=NON;
    for(i=0; i<DimJeu.N*DimJeu.M; i++) //On parcourt le tableau pour trouver les pions non alignés,
    {
        switch (plateau[i])
        {
        case xGagne:
        case xPerd:
            plateau[i]=Croix;
            break;
        case oGagne:
        case oPerd:
            plateau[i]=Rond;
            break;
        default:  //les valeurs Vide, xGagne, xPerd, oGagne, oPerd ne sont pas changées
            break;
        }
    }
}

/**********************/

void DefinirZoneEtendue(CaracteristiqueJeu Jeu, DimPlateau DimJeu, Couronne *InfoZoneDeRecherche)
{
    int i=0;
    int LigneStricteH=0;//plus petit rectangle encadrant tous les jetons deja joués
    int LigneStricteB=DimJeu.M-1;
    int LigneStricteG=0;
    int LigneStricteD=DimJeu.N-1;
    int *TableauDeTravail;
    int marge;//[1 : couronne de 1 case][2 : couronne de 1 case]
    int LigneH;
    int LigneB;
    int LigneG;
    int LigneD;

    /**/  int cdp;
    /**/  int l;
    /**/  int h;

    if(Jeu.NbCasesJouees!=0)//pb si on clique dès le depart et que nombrecasejouee =0 donc tableau de taille nulle
    {
        TableauDeTravail=malloc((Jeu.NbCasesJouees)*sizeof(*TableauDeTravail)); //on définit un tableau qui sera supprimé à la fin de la fonction
//Ce tableau servira a stocker les valeurs de HistoriqueJetons pour chercher les min et max de chaque coordonnée
        if(TableauDeTravail==NULL) //si erreur d'allocation
        {
            free(TableauDeTravail); //liberation du tableau
            fprintf(stderr, "Erreur de creation du tableau de travail");
            exit(EXIT_FAILURE); //quitter le programme
        }

        for(i=0; i<Jeu.NbCasesJouees; i++)
            TableauDeTravail[i]=Jeu.HistoriqueJeton[i]%DimJeu.N; //Le TableauDeTravail contient toutes les abscisses des jetons déjà joués

        LigneStricteD=maxTab(TableauDeTravail, Jeu.NbCasesJouees); //Abscisse maximale
        LigneStricteG=minTab(TableauDeTravail, Jeu.NbCasesJouees); //Abscisse minimale

        for(i=0; i<Jeu.NbCasesJouees; i++)
            TableauDeTravail[i]=Jeu.HistoriqueJeton[i]/DimJeu.N; //Le TableauDeTravail contient toutes les ordonnées des jetons déjà joués

        LigneStricteB=maxTab(TableauDeTravail, Jeu.NbCasesJouees); //Ordonnée maximale
        LigneStricteH=minTab(TableauDeTravail, Jeu.NbCasesJouees); //Ordonnée minimale

        for(marge =1; marge<5/*DimJeu.NbJetonsAAligner*/; marge++)//on va determiner chaque couronne 1->4
        {
            LigneH=maximumEntre(0, LigneStricteH-marge); //reste a restreindre la zone de recherche
            LigneB=minimumEntre(DimJeu.M-1, LigneStricteB+marge);
            LigneG=maximumEntre(0, LigneStricteG-marge);
            LigneD=minimumEntre(DimJeu.N-1, LigneStricteD+marge);

            InfoZoneDeRecherche[marge].CaseDepart=LigneH*DimJeu.N+LigneG;
            InfoZoneDeRecherche[marge].Largeur=LigneD-LigneG+1;
            InfoZoneDeRecherche[marge].Hauteur=LigneB-LigneH+1;

            /**/    cdp=InfoZoneDeRecherche[marge].CaseDepart;
            /**/    l=InfoZoneDeRecherche[marge].Largeur;
            /**/    h=InfoZoneDeRecherche[marge].Hauteur;

        }

        free(TableauDeTravail);
        TableauDeTravail=NULL;
    }
}

/**********************/
void DefinirListeCaseVide(CaracteristiqueJeu *Jeu, DimPlateau DimJeu,Couronne InfoZoneDeRecherche[], t_logs *logs) //Dresse la liste des cases vides dans la ZoneDeRecherche
{
    int i=0, j=0;
    int *TabDeTravail= FonctionAllocation1D(DimJeu.N*DimJeu.M, sizeof(*TabDeTravail), logs);//tableau servant a creer la ListeCaseVide, tableau grande taille N*M
//   int taille=DimJeu.N*DimJeu.M-Jeu->NbCasesJouees;//dimension maximale
    int NumCouronne = 0;//0->4
    int taille[5];//taille de chaque Listecasevides

    /*
    int taille_C0=InfoZoneDeRecherche[0].Largeur*InfoZoneDeRecherche[0].Hauteur-Jeu->NbCasesJouees;
    int taille_C1=InfoZoneDeRecherche[1].Largeur*InfoZoneDeRecherche[1].Hauteur-Jeu->NbCasesJouees;
    int taille_C2=InfoZoneDeRecherche[2].Largeur*InfoZoneDeRecherche[2].Hauteur-Jeu->NbCasesJouees;
    int taille_C3=InfoZoneDeRecherche[3].Largeur*InfoZoneDeRecherche[3].Hauteur-Jeu->NbCasesJouees;
    int taille_C4=InfoZoneDeRecherche[4].Largeur*InfoZoneDeRecherche[4].Hauteur-Jeu->NbCasesJouees;*/

    /*Pour creer la ListeCasesVides :
    - 1. on remplit tout le tableau de 1000
    - 2. on remplace les cases de la zone jouable par leur valeur
    - 3. on retire les jetons déjà joués, en ecrivant 1000
    TabDeTravail est constitué de 1000 (cases hors zone ou deja jouées) et autres cases jouables
    - 4. ranger dans l'ordre croissant
    - 5. copier les ... 1ers elements de TabdeTravail dans ListeCasesVides
    */
//On va creer un tableau de 5*NbCases juxtaposant ListeCasesVide+ListeCaseVideC1+ListeCaseVideC2+ListeCaseVideC3+ListeCaseVideC4

    for (i=0; i<5*DimJeu.N*DimJeu.M; i++) //initialisation de toutes les cases à -1
        Jeu->ListeCasesVides[i]=-1;

    for (i=0; i<5; i++)
        taille[i]=InfoZoneDeRecherche[i].Largeur*InfoZoneDeRecherche[i].Hauteur-Jeu->NbCasesJouees;


//creation de ListeCasesVide
    for (NumCouronne=0; NumCouronne<5; NumCouronne++)
    {
        for (i=0; i<DimJeu.N*DimJeu.M; i++) //etape 1
            TabDeTravail[i]=1000;

        for (i=0; i<InfoZoneDeRecherche[NumCouronne].Largeur; i++) //etape 2
        {
            for (j=0; j<InfoZoneDeRecherche[NumCouronne].Hauteur; j++)
                TabDeTravail[InfoZoneDeRecherche[NumCouronne].CaseDepart+i+j*DimJeu.N]=InfoZoneDeRecherche[NumCouronne].CaseDepart+i+j*DimJeu.N;
        }

        for (i=0; i<Jeu->NbCasesJouees; i++) //etape 3
            TabDeTravail[Jeu->HistoriqueJeton[i]]=1000;

        TabCroissant(TabDeTravail, DimJeu.N*DimJeu.M);//etape 4 //le TabDeTravail contient au debut toutes les cases vides jouables de la zone suivi de 1000 (case de la zone jouées ou case hors zone)

        for (i=0; i<taille[0]; i++) //etape 5, copie dans la bonne zone du tableau
            Jeu->ListeCasesVides[i+NumCouronne*DimJeu.N*DimJeu.M]=TabDeTravail[i];
    }

    free(TabDeTravail);
    TabDeTravail=NULL;
}


/**********************/

void ConfigType(t_morp_sdl *morp_sdl, jeu *TypeJeu, Joueur *Joueur, int x, int y) //Pour modifier le type de jeu et le pion
{
    SDL_Rect positionAffichage;
    /*SDL_Rect positionAffichage,
    .x=BoutonX;//pisition du boutoon configType...
    .y=BoutonYConfigType
    */
//les coordonnéees envoyées sont relatives au coin superieur gauche du bouton Config
    if((11<x)&&(x<50)&&(25<y)&&(y<54)) //Si on a cliqué pour modifier qui est le joueur 1 : humain ou ordi
        ConfigTypeJeu(TypeJeu,/*SDL_Rect positionAffichage,*/ 0, Joueur);

    if((99<x)&&(x<138)&&(25<y)&&(y<54)) //Si on a cliqué pour modifier qui est le joueur 2 : humain ou ordi
        ConfigTypeJeu(TypeJeu,/*SDL_Rect positionAffichage,*/ 1, Joueur);

    if((21<x)&&(x<50)&&(5<y)&&(y<24)) //Si on a cliqué pour modifier l'icone 1
        ConfigTypePion(morp_sdl,/*SDL_Rect positionAffichage,*/ 0, Joueur);

    if((99<x)&&(x<128)&&(5<y)&&(y<24)) //Si on a cliqué pour modifier l'icone 2
        ConfigTypePion(morp_sdl,/*SDL_Rect positionAffichage,*/ 1, Joueur);

    if((65<x)&&(x<85)&&(5<y)&&(y<24)) //Si on a cliqué pour modifier la musique et l'icone associée
    {
        positionAffichage.x=BoutonX;//position du boutoon configType...
        positionAffichage.y=BoutonYConfigType;
        ConfigTypeMusique(morp_sdl, positionAffichage);
    }
}
/**********************/

void ConfigTypeJeu(jeu *TypeJeu, int zone, Joueur *Joueur) //Pour modifier le type de jeu
{
    if (zone==0) //si on clique dans la zone gauche
    {
        switch (*TypeJeu) //on modifie le type de jeu
        {
        case HH:
            *TypeJeu=OH;
            break;
        case HO:
            *TypeJeu=OO;
            break;
        case OH:
            *TypeJeu=HH;
            break;
        case OO:
            *TypeJeu=HO;
            break;
        default:
            break;
        }
    }
    else //si on clique dans la zone droite
    {
        switch (*TypeJeu) //on modifie le type de jeu
        {
        case HH:
            *TypeJeu=HO;
            break;
        case HO:
            *TypeJeu=HH;
            break;
        case OH:
            *TypeJeu=OO;
            break;
        case OO:
            *TypeJeu=OH;
            break;
        default:
            break;
        }
    }
    Joueur->ListeJ[zone].joueur= (Joueur->ListeJ[zone].joueur+1)%2; //on modifie le statut du joueur gauche ou droite en passant au statut suivant
}
/**********************/

void ConfigTypePion(t_morp_sdl *morp_sdl, int zone, Joueur *Joueur) //Pour modifier le pion
{
    SDL_Event event;
    int ChoixFini=NON;
    SDL_Rect clic= {0, 0, EcranLarg, EcranHaut}; //position du clic (x, y) et dimension de l'ecran (w, h)
    int i=0;
    SDL_Rect positionFondMarron= {BoutonX+108*zone-19, BoutonYConfigType+5, 20*NbPionsParFamille, 20*NbFamillesPion}; //dessine un rectangle marron comme fond a la bonne taille
    SDL_Rect position[nbPions]; //position de collage pointeur2
    SDL_Rect ZoneImageSelectionnee = {874, 0, 20, 20};
    int NumeroPionChoisi=24; //il y a 24 pions (n°0->23)+ le reste de la zone (n°24)


    AfficheRectPlein(morp_sdl, positionFondMarron, couleurImageMarron);//petit fond marron pour mieux voir les icones proposées
    for (i=0; i<nbPions; i++) //affiche tous les pions possibles
    {
        position[i].x = positionFondMarron.x+20*(i%NbPionsParFamille); //position de collage du pion aligné en ligne de NbPionsParFamille (=4) symboles
        position[i].y = positionFondMarron.y+20*(i/NbPionsParFamille);
        position[i].w =20;
        position[i].h =20;

        SDL_RenderCopy(morp_sdl->renderer, morp_sdl->jeton.Pion[i],&ZoneImageSelectionnee,&position[i]); //On colle le pointeur2
    }
    SDL_RenderPresent(morp_sdl->renderer);//actualisation de l'ecran

    while(ChoixFini==NON) //gestion du clic
    {
        while(SDL_PollEvent(&event)) //Traiter les évènements
        {
            switch(event.type)
            {
            case SDL_QUIT: //Si on a cliqué sur la croix
                ChoixFini=OUI;
                break;
            case SDL_KEYUP: //Si touche du clavier enfoncée
                switch (event.key.keysym.sym) //selon la touche enfoncée
                {
                case SDLK_ESCAPE: // Appui sur la touche Echap, on va arrêter le jeu
                case SDLK_a: //appui sur la touche a pour quitter
                case SDLK_v: //appui sur la touche v pour valider et quitter
                    ChoixFini=OUI;
                    break;
                default : //sinon rien pour eviter message d'alerte lors de la compilation
                    break;
                }
                break;
            case SDL_MOUSEBUTTONUP: //Si clic souris //pions
                clic.x=event.button.x-positionFondMarron.x; //position du clic par rapport au coin superieur gauche du menu qui vient d'apparaitre
                clic.y=event.button.y-positionFondMarron.y;
                if((clic.x>=0)&&(clic.x<=NbPionsParFamille*20)&&(clic.y>=0)&&(clic.y<=NbFamillesPion*20)) //si on clique dans la zone d'affichage des pions alors on determine le bon pion
                {
                    NumeroPionChoisi=clic.x/20+NbPionsParFamille*(clic.y/20); //numero de l'image choisie
                    if(NumeroPionChoisi!=Joueur->ListeJ[(zone+1)%2].NumeroPion) //si le joueur choisi est différent de celui de l'autre joueur alors le choix est bon
                        Joueur->ListeJ[zone].NumeroPion= NumeroPionChoisi; //on modifie le numero du pion
                } //else NumeroPionChoisi = 24 // sinon on garge la zone 24
                ChoixFini=OUI;
                break;
            default: //Sinon on ne fait rien
                break;
            }
        }
    }
}
/**********************/

void ConfigTypeMusique(t_morp_sdl *morp_sdl, SDL_Rect positionAffichage) //Pour modifier le pion
{
    SDL_Event event;
    int ChoixFini=NON;
    SDL_Rect clic= {0, 0, EcranLarg, EcranHaut}; //position du clic (x, y) et dimension de l'ecran (w, h)
    SDL_Rect positionEcart= {(BoutonLargeur-20)/2,5, 0, 0};//decalage par rapport au coin superieur gauche de positionAffichage
    SDL_Rect positionFondMarron= {positionAffichage.x+positionEcart.x, positionAffichage.y+positionEcart.y, 20, 20*NbMusiquesMax}; //dessine un rectangle marron comme fond a la bonne taille
    if(positionFondMarron.y+morp_sdl->jeton.DimensionImage[i_Musique].h>EcranHaut)//si on depasse en bas, alors il faut remonter la zone d'affichage
        positionFondMarron.y=positionFondMarron.y- morp_sdl->jeton.DimensionImage[i_Musique].h;

    SDL_Rect ZoneImageSelectionnee = {0, 0, 20, 20*NbMusiquesMax};

    AfficheRectPlein(morp_sdl, positionFondMarron, couleurImageMarron);//recouvre la marge avec la couleur de fond

    SDL_RenderCopy(morp_sdl->renderer, morp_sdl->jeton.Image[i_Musique],&ZoneImageSelectionnee,&positionFondMarron);

    SDL_RenderPresent(morp_sdl->renderer);//Actualisation

    while(ChoixFini==NON) //gestion du clic
    {
        while(SDL_PollEvent(&event)) //Traiter les évènements
        {
            switch(event.type)
            {
            case SDL_QUIT: //Si on a cliqué sur la croix
                ChoixFini=OUI;
                break;
            case SDL_KEYUP: //Si touche du clavier enfoncée
                switch (event.key.keysym.sym) //selon la touche enfoncée
                {
                case SDLK_ESCAPE: // Appui sur la touche Echap, on va arrêter le jeu
                case SDLK_a: //appui sur la touche a pour quitter
                case SDLK_v: //appui sur la touche v pour valider et quitter ChoixFini=OUI;
                    ChoixFini=OUI;
                    break;
                default : //sinon rien pour eviter message d'alerte lors de la compilation
                    break;
                }
                break;
            case SDL_MOUSEBUTTONUP: //Si clic souris //pions
                morp_sdl->musique.Ancien = morp_sdl->musique.Actuel;//sauvegarde e l'anciene musique
                clic.x=event.button.x-positionFondMarron.x; //position du clic par rapport au coin superieur gauche du menu qui vient d'apparaitre
                clic.y=event.button.y-positionFondMarron.y;
                if((clic.x>=0)&&(clic.x<=20)&&(clic.y>=0)&&(clic.y<=NbMusiquesMax*20)) //si on clique dans la zone d'affichage des icones alors on determine la bonne musique
                    morp_sdl->musique.Actuel=(clic.y)/20;

                ChoixFini=OUI;
                break;
            default: //Sinon on ne fait rien
                break;
            }
        }
    }
}
/**********************/

void ConfigDimension(t_morp_sdl *morp_sdl, Joueur Joueur, DimPlateau *DimJeu, CaracteristiqueJeu Jeu)
{
    DimPlateau AncienDimJeu;
    DimPlateau NouveauDimJeu;

    /*   AncienDimJeu.N=(*DimJeu).N;
       AncienDimJeu.M=(*DimJeu).M;
       AncienDimJeu.DimCase=(*DimJeu).DimCase;
       AncienDimJeu.NbJetonsAAligner=(*DimJeu).NbJetonsAAligner;
       NouveauDimJeu.N=(*DimJeu).N;
       NouveauDimJeu.M=(*DimJeu).M;
       NouveauDimJeu.DimCase=(*DimJeu).DimCase;
       NouveauDimJeu.NbJetonsAAligner=(*DimJeu).NbJetonsAAligner;*/
    memcpy(&AncienDimJeu,DimJeu,sizeof(DimPlateau));//copie dans AncienDimJeu des données de DimJeu (on ecrit "DimJeu" au lieu de "&DimJeu" car c'est deja un pointeur)
    memcpy(&NouveauDimJeu,DimJeu,sizeof(DimPlateau));//copie dans NouveauDimJeu des données de DimJeu (on ecrit "DimJeu" au lieu de "&DimJeu" car c'est deja un pointeur)

    SDL_Rect positionCurseurAxe = {200, 200, morp_sdl->jeton.DimensionImage[i_CurseurAxe].w,  morp_sdl->jeton.DimensionImage[i_CurseurAxe].h}; //position des deux axes et sert d'origine relative pour les curseurs rouges et les croix Noires ou grises
    SDL_Rect positionCurseur1 = {positionCurseurAxe.x+(((*DimJeu).N)-0.5)*morp_sdl->jeton.DimensionImage[i_Curseur].w, positionCurseurAxe.y, morp_sdl->jeton.DimensionImage[i_Curseur].w,  morp_sdl->jeton.DimensionImage[i_Curseur].h}; //affiche le point rouge a l'horizontale
    SDL_Rect positionCurseur2 = {positionCurseurAxe.x, positionCurseurAxe.y+(((*DimJeu).M)-0.5)*morp_sdl->jeton.DimensionImage[i_Curseur].h, morp_sdl->jeton.DimensionImage[i_Curseur].w,  morp_sdl->jeton.DimensionImage[i_Curseur].h}; //affiche le point rouge a la verticale
    SDL_Rect positionCroix = {positionCurseurAxe.x+2*morp_sdl->jeton.DimensionImage[i_CurseurCroixGris].w, positionCurseurAxe.y+3*morp_sdl->jeton.DimensionImage[i_CurseurCroixGris].w, morp_sdl->jeton.DimensionImage[i_CurseurCroixNoir].w,  morp_sdl->jeton.DimensionImage[i_CurseurCroixNoir].h}; //affiche le point rouge a la verticale
    SDL_Rect positionFond= {positionCurseurAxe.x, positionCurseurAxe.y, morp_sdl->jeton.DimensionImage[i_CurseurAxe].w, morp_sdl->jeton.DimensionImage[i_CurseurAxe].h}; //Fond marron derriere le curseur

    int i;
    int T[12]= {3, 4, 5, 6, 8, 10, 12, 15, 20, 24, 25, 30}; //liste des dimensions possibles
    int ConfigChoisie=NON;
    SDL_Event event;
    SDL_Rect clic= {0, 0, zonePlateauLarg, zonePlateauLarg}; //position du clic (x, y) et dimension de l'ecran (w, h)
    int positionCurseurX=3;
    int positionCurseurY=3;
    int NbJetonsAAlignerPossible=(*DimJeu).NbJetonsAAligner;
    zone ZoneCliquee = MARGE;

    AfficheBoutons(morp_sdl, Joueur, *DimJeu, EcranConfig);

    SDL_ShowCursor(SDL_ENABLE);
    for (i=0; i<12; i++)
    {
        if(T[i]==(*DimJeu).N) positionCurseurX=3+i;
        if(T[i]==(*DimJeu).M) positionCurseurY=3+i;
    }
    positionCurseur1.x = positionCurseurAxe.x+(positionCurseurX-0.5)*morp_sdl->jeton.DimensionImage[i_Curseur].w;
    positionCurseur2.y = positionCurseurAxe.y+(positionCurseurY-0.5)*morp_sdl->jeton.DimensionImage[i_Curseur].h;

    AffichePlateau(morp_sdl, *DimJeu); //on affiche le plateau de configuration de dimensions N*M
    AfficheRectPlein(morp_sdl, positionFond, couleurImageMarron);//recouvre la marge avec la couleur de fond

    SDL_RenderCopy(morp_sdl->renderer, morp_sdl->jeton.Image[i_CurseurAxe],NULL,&positionCurseurAxe); //on affiche le moyen de modifier les dimensions
    SDL_RenderCopy(morp_sdl->renderer, morp_sdl->jeton.Image[i_Curseur],NULL,&positionCurseur1); //affiche le point rouge a l'horizontale
    SDL_RenderCopy(morp_sdl->renderer, morp_sdl->jeton.Image[i_Curseur],NULL,&positionCurseur2); //affiche le point rouge a la verticale

    for(i=0; i<5; i++) //Affiche le nombre de jetons à aligner, au max 5
    {
        if(i<(*DimJeu).NbJetonsAAligner) //Affiche le NbJetonsAAligner
        {
            SDL_RenderCopy(morp_sdl->renderer, morp_sdl->jeton.Image[i_CurseurCroixNoir],NULL,&positionCroix); //
            positionCroix.x=positionCroix.x+ morp_sdl->jeton.DimensionImage[i_CurseurCroixGris].w;
        }
        else
        {
            SDL_RenderCopy(morp_sdl->renderer,morp_sdl->jeton.Image[i_CurseurCroixGris],NULL,&positionCroix);
            positionCroix.x=positionCroix.x+(morp_sdl->jeton.DimensionImage[i_CurseurCroixGris]).w;
        }
    }

    while(ConfigChoisie==NON)
    {
        while(SDL_PollEvent(&event)) //Traiter les évènements
        {
            switch(event.type)
            {
            case SDL_QUIT: //Si on a cliqué sur la croix
                ConfigChoisie=OUI;
                ZoneCliquee = QUITTER;
                break;
            case SDL_KEYUP: //Si touche du clavier enfoncée
                switch (event.key.keysym.sym) //selon la touche enfoncée
                {
                case SDLK_ESCAPE: // Appui sur la touche Echap, on annule les modifications en cours,
                case SDLK_a: //si appui sur la touche q pour quitter sans modifier
                    ConfigChoisie=OUI;
                    ZoneCliquee = QUITTER;
                    break;
                case SDLK_RETURN: //si appui sur la touche RETURN pour quitter en modifiant
                case SDLK_v: //si appui sur la touche V pour quitter en modifiant
                    ConfigChoisie=OUI;
                    ZoneCliquee = VALIDER;
                    break;
                default : //sinon rien pour eviter message d'alerte lors de la compilation
                    break;
                }
                break;
            case SDL_MOUSEBUTTONUP: //Si clic souris
                //si on clique sur une graduation, la calculer et actualiser N, M et la position du curseur
                //Si on clique sur le curseur et qu'on le déplace, regarder quand on relache la souris et actualiser...
                clic.x=event.button.x; //on recupere les coordonnées du clic
                clic.y=event.button.y;

                ZoneCliquee=QuelleZoneCliquee(*DimJeu,clic, Joueur,Jeu); //si on a clique sur les boutons valider, quitter ou jeu
                switch(ZoneCliquee)
                {
                case QUITTER: //on quitte sans valider les modifications
                    ConfigChoisie=OUI;
                    ZoneCliquee = QUITTER; //inutile mais pratique pour garder la meme structure qu'avant
                    break;
                case VALIDER: //on quitte en validant les modifications
                    ConfigChoisie=OUI;
                    ZoneCliquee = VALIDER; //inutile mais pratique pour garder la meme structure qu'avant
                    break;
                default: //sinon, on regarde les modifications effectuees
                    clic.x=event.button.x; //on recupere les coordonnées du clic
                    clic.y=event.button.y;

                    if((positionCurseurAxe.x<clic.x)&&(clic.x<positionCurseurAxe.x+morp_sdl->jeton.DimensionImage[i_CurseurAxe].w)&&(positionCurseurAxe.y<clic.y)&&(clic.y<positionCurseurAxe.y+morp_sdl->jeton.DimensionImage[i_CurseurAxe].h)) //Si on clique dans la zone de modification CurseurAxe
                    {
                        /*            int a=positionCurseurAxe.x+2.5*Jeton->DimensionImage[i_Curseur].w;//<x
                                    int b=positionCurseurAxe.x+Jeton->DimensionImage[i_CurseurAxe].w;//x<
                                    int c=positionCurseurAxe.y;//<y
                                    int d=positionCurseurAxe.y+30;//y<
                        */
                        //modification du nombre de cases dans la largeur
                        if((positionCurseurAxe.x+2.5*morp_sdl->jeton.DimensionImage[i_Curseur].w<clic.x)&&(clic.x<positionCurseurAxe.x+morp_sdl->jeton.DimensionImage[i_CurseurAxe].w)&&(positionCurseurAxe.y<clic.y)&&(clic.y<positionCurseurAxe.y+30)) //Si on clique dans la zone de modification horizontale
                        {
                            NouveauDimJeu.N=T[(clic.x-(positionCurseurAxe.x+35))/(morp_sdl->jeton.DimensionImage[i_Curseur].w)]; //On determine la case associee du tableau qui renvoie la valeur de N associee
                            positionCurseur1.x = positionCurseurAxe.x+(((clic.x-(positionCurseurAxe.x+35))/(morp_sdl->jeton.DimensionImage[i_Curseur].w))+2.5)*morp_sdl->jeton.DimensionImage[i_Curseur].w; //Mise a jour des coordonnées
                        } // il faut determiner la graduation a partir de la position du clic avant ou apres le milieu du segment

                        //modification du nombre de cases dans la hauteur
                        if((positionCurseurAxe.y+2.5*morp_sdl->jeton.DimensionImage[i_Curseur].h<clic.y)&&(clic.y<positionCurseurAxe.y+morp_sdl->jeton.DimensionImage[i_CurseurAxe].h)&&(positionCurseurAxe.x<clic.x)&&(clic.x<positionCurseurAxe.x+30)) //Si on clique dans la zone de modification verticale
                        {
                            NouveauDimJeu.M=T[(clic.y-(positionCurseurAxe.y+35))/(morp_sdl->jeton.DimensionImage[i_Curseur].h)]; //On determine la case associee du tableau qui renvoie la valeur de N associee
                            positionCurseur2.y = positionCurseurAxe.y+(((clic.y-(positionCurseurAxe.y+35))/(morp_sdl->jeton.DimensionImage[i_Curseur].h))+2.5)*morp_sdl->jeton.DimensionImage[i_Curseur].h; //Mise a jour des coordonnées
                        } // il faut determiner la graduation a partir de la position du clic avant ou apres le milieu du segment

                        //modification du NbJetonsAAligner
                        if((positionCurseurAxe.x+4*morp_sdl->jeton.DimensionImage[i_CurseurCroixNoir].w<clic.x)&&(clic.x<positionCurseurAxe.x+7*(morp_sdl->jeton.DimensionImage[i_CurseurCroixNoir].w))&&(positionCurseurAxe.y+3*(morp_sdl->jeton.DimensionImage[i_CurseurCroixNoir].w) <clic.y)&&(clic.y<positionCurseurAxe.y+4*(morp_sdl->jeton.DimensionImage[i_CurseurCroixNoir].w ))) //Si on clique dans la zone de modification du nombre de jeton a aligner
                        {
                            NbJetonsAAlignerPossible=(clic.x-(positionCurseurAxe.x+morp_sdl->jeton.DimensionImage[i_CurseurCroixNoir].w))/morp_sdl->jeton.DimensionImage[i_CurseurCroixNoir].w; //On cherche le nombre de jetons à aligner
                        }

                        NouveauDimJeu.NbJetonsAAligner=minimumEntre(NbJetonsAAlignerPossible, maximumEntre(NouveauDimJeu.N, NouveauDimJeu.M)); //on choisit le minimum entre le NbJetonsAAligner voulu et la dimension la plus grande : comment aligner 5 pions dans un tableau 4*3? Non ce sera 4 jetons au max.
                        NouveauDimJeu.DimCase=zonePlateauLarg/maximumEntre(NouveauDimJeu.N, NouveauDimJeu.M); //Determine la nouvelle dimension des cases

                        AffichePlateau(morp_sdl, NouveauDimJeu);
                        AfficheRectPlein(morp_sdl, positionFond, couleurImageMarron);//on colle un fond marron pour mieux visualiser les curseurs
                        SDL_RenderCopy(morp_sdl->renderer,morp_sdl->jeton.Image[i_CurseurAxe],NULL,&positionCurseurAxe); //on affiche le moyen de modifier les dimensions
                        SDL_RenderCopy(morp_sdl->renderer,morp_sdl->jeton.Image[i_Curseur],NULL,&positionCurseur1);
                        SDL_RenderCopy(morp_sdl->renderer,morp_sdl->jeton.Image[i_Curseur],NULL,&positionCurseur2);

                        positionCroix.x=positionCurseurAxe.x+2*(morp_sdl->jeton.DimensionImage[i_CurseurCroixGris].w); //affiche le point rouge a la verticale

                        for(i=0; i<NouveauDimJeu.NbJetonsAAligner; i++) //Affiche les croix en noir
                        {
                            SDL_RenderCopy(morp_sdl->renderer,morp_sdl->jeton.Image[i_CurseurCroixNoir],NULL,&positionCroix);
                            positionCroix.x=positionCroix.x+ morp_sdl->jeton.DimensionImage[i_CurseurCroixNoir].w;
                        }
                        for(i=NouveauDimJeu.NbJetonsAAligner; i<5; i++) //Affiche les croix en gris
                        {
                            SDL_RenderCopy(morp_sdl->renderer,morp_sdl->jeton.Image[i_CurseurCroixGris],NULL,&positionCroix);
                            positionCroix.x=positionCroix.x+ morp_sdl->jeton.DimensionImage[i_CurseurCroixGris].w;
                            //          SDL_BlitSurface(Jeton->CurseurCroixGris, NULL, Jeton->Ecran, &positionCroix);                            positionCroix.x=positionCroix.x+(Jeton->CurseurCroixGris)->w;
                        }
                        ConfigChoisie=NON;
                        ZoneCliquee=MARGE;
                    }
                    break;
                }
            default:
                break;
            }
        }
        SDL_RenderPresent(morp_sdl->renderer);//Actualisation
    }

    switch(ZoneCliquee)
    {
    case QUITTER:
        /* (*DimJeu).N=AncienDimJeu.N; //on annule les modifications en cours, on garde un tableau avec les memes dimensions qu'avant
         (*DimJeu).M=AncienDimJeu.M;
         (*DimJeu).DimCase=AncienDimJeu.DimCase;
         (*DimJeu).NbJetonsAAligner=AncienDimJeu.NbJetonsAAligner;*/
        memcpy(DimJeu,&AncienDimJeu,sizeof(DimPlateau));//copie dans NouveauDimJeu des données de DimJeu (on ecrit "DimJeu" au lieu de "&DimJeu" car c'est deja un pointeur)

        ConfigChoisie=OUI;
        break;
    case VALIDER:
        /* (*DimJeu).N=NouveauDimJeu.N;
         (*DimJeu).M=NouveauDimJeu.M;
         (*DimJeu).DimCase=NouveauDimJeu.DimCase;
         (*DimJeu).NbJetonsAAligner=NouveauDimJeu.NbJetonsAAligner;*/
        memcpy(DimJeu,&NouveauDimJeu,sizeof(DimPlateau));//copie dans NouveauDimJeu des données de DimJeu (on ecrit "DimJeu" au lieu de "&DimJeu" car c'est deja un pointeur)

        ConfigChoisie=OUI;
        break;
    default :
        break;
    }
}

/**********************/
void ModificationConfigDimension(DimPlateau AncienDimJeu, DimPlateau NouveauDimJeu, SDL_Rect *DimensionPlateauJeu, Case **plateau, CaracteristiqueJeu *Jeu, t_logs *logs)
{
    Jeu->taille=NouveauDimJeu.N*NouveauDimJeu.M; //modification de la nouvelle taille des tableaux

//si seul le NbJetonAAligner a changé, alors on vide le plateau
//si N ou M a changé, il faut tout vider et faire les realloc puis vider le jeu
    videJeu(*plateau, Jeu, AncienDimJeu); //Vide le plateau de jeu AVANT d'avoir modifier les dimensions, pour tout remettre a zero
    free(*plateau); //libere l'ancien plateau
    *plateau=NULL;
    *plateau=FonctionAllocation1D(NouveauDimJeu.N*NouveauDimJeu.M, sizeof(*plateau), logs); //definit le nouveau plateau

    free(Jeu->HistoriqueJeton); //on libere l'ancien HistoriqueJeu
    Jeu->HistoriqueJeton=NULL;
    Jeu->HistoriqueJeton= FonctionAllocation1D(NouveauDimJeu.N*NouveauDimJeu.M, sizeof(Jeu->HistoriqueJeton), logs); //definit le nouveau HistoriqueJeu

    free(Jeu->ListeCasesVides); //on libere l'ancien ListeCasesVides
    Jeu->ListeCasesVides=NULL;
    Jeu->ListeCasesVides= FonctionAllocation1D(NouveauDimJeu.N*NouveauDimJeu.M, sizeof(Jeu->ListeCasesVides), logs); //definit le nouveau ListeCasesVides

    free(Jeu->tabMeilleuresCases); //on libere l'ancien tabMeilleuresCases
    Jeu->tabMeilleuresCases=NULL;
    Jeu->tabMeilleuresCases= FonctionAllocation1D(NouveauDimJeu.N*NouveauDimJeu.M, sizeof(Jeu->tabMeilleuresCases), logs); //definit le nouveau ListeCasesVides

    videJeu(*plateau, Jeu, NouveauDimJeu); //Vide le plateau de jeu APRES d'avoir modifier les dimensions, sinon, on ne vide pas tout le tableau
    (*DimensionPlateauJeu).w=zonePlateauLarg/maximumEntre(NouveauDimJeu.N, NouveauDimJeu.M)*NouveauDimJeu.N; //Dimension reelle de la zone de jeu quand on personnalise les dimensions
    (*DimensionPlateauJeu).h=zonePlateauLarg/maximumEntre(NouveauDimJeu.N, NouveauDimJeu.M)*NouveauDimJeu.M;
}

/**********************/

int QuelleZoneTournoiCliquee(int ClicX, int ClicY, SDL_Rect *PositionZonesCliquables)
{
    int zone=72; //[0: ligne 1, colonne 1][1: ligne 1, colonne 2]...[n: ligne n/7, colonne n%7]...[70:bouton Valider][71:bouton Quitter][72:Hors zone cliquable]
    int i=0;

    while(((PositionZonesCliquables[i].x<ClicX)&&(ClicX<PositionZonesCliquables[i].x+PositionZonesCliquables[i].w)&&(PositionZonesCliquables[i].y<ClicY)&&(ClicY<PositionZonesCliquables[i].y+PositionZonesCliquables[i].h))==NON&&(i<72)) //Si on clique sur le bouton Annuler, alors
    {
        i++;
    }
    zone=i; //si une zone valide a ete atteinte, on connait son numero

    return zone;
}
/**********************/

void JouerMusique(Musique Musique)
{
    if(Musique.Actuel==0) //si le numero de musique est alors ne rien jouer
        Mix_HaltMusic();

    if(Musique.Actuel!=Musique.Ancien) //si on a changé de musique, alors on joue la nouvelle sinon, on conserve l'ancienne
        Mix_PlayMusic(Musique.Liste[Musique.Actuel], -1); //-1 pour jouer infiniment la musique/a mettre dans une nouvelle fonction JoueMusique
}
