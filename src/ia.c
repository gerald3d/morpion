#include "Main.h"

/*
Pour les grandes dimensions, une zone de recherche est créée autour des jetons déjà joués.
*/

extern FILE* fichierErreur;//extern car la variable globale est deja initialisee dans main.c

/**********************/
//Dans cette version, on va créer un tableau de N*M cases correspondant à toutes les cases possibles.
//Il est initialise à -1
//On utilise ListeCasesVides pour remplacer les cases jouables à 0, n'ont pas ete validée par verif fini
//On ne va pas utiliser la fonction jouer mais on va faire jouer l'ordi/joueur jusqu'à plusieurs fois de suite, ignorant le coup adverse joué
//Quand une validation a lieu, on incremente la valeur testée (0->1->2...)
//Ainsi, une case intervenant dans 2 alignements gagnants aura un coef de 2
//Il faut jouer la case au plus grand coef

//la profondeur est 4 c'est-à-dire qu'au maximum, on simule 4 jetons : ordi - adv - ordi - adv
//Il y a 5 issues possibles :
//ordi gagne en 1 coup -> il faut jouer la case
//adv gagne en 1 coup -> il faut empecher cela
//ordi gagne en 2 coups -> il peut y avoir plusieurs solutions
//adv gagne en 2 coups -> il peut y avoir plusieurs solutions
//Personne ne gagne

//pour tester si on peut gagner avec 1 seul jeton (ordi ou adv), il faut avoir la liste des cases vide mais avec juste 1 rang autour
//pour tester si on peut gagner avec 2 jetons (ordi ou adv), il faut avoir la liste des cases vide mais avec juste 2 rangs autour

void ia(Case *plateau, Joueur Joueur, CaracteristiqueJeu *Jeu, DimPlateau DimJeu, t_logs *logs)
{
  int *TabDeCoef= FonctionAllocation1D(DimJeu.N*DimJeu.M, sizeof(*TabDeCoef), logs);
    Couronne InfoZoneDeRecherche[5]= {{0,DimJeu.N,DimJeu.M},{0,0,0},{0,0,0},{0,0,0},{0,0,0}}; //les InfoZoneDeRecherche est un tableau de 6 infos (CaseDepart-Largeur-Hauteur) correspondant au tableau general [0] puis aux zones de recherche [couronne 1-2-3-4]
    int NbCasesVides=DimJeu.N*DimJeu.M-(Jeu->NbCasesJouees);
    int MeilleurScore = -10000;
    int NbMeilleurScore=0;//nombre de fois que le MeilleurScoreeeea été atteint
    int niveau=0;//[0: ordi teste 1 jeton][1: adv teste 1 jeton][2: ordi teste 2 jetons][3: adv teste 2 jetons][4: ordi teste 3 jetons][5: adv teste 3 jetons][6: ordi teste 4 jetons][7: adv teste 4 jetons]
    int AlignementRealise = NON;//[OUI : il y a eu un alignement, on ne teste pas un niveau plus profond][NON : il n'y a pas d'alignement, on testera un niveau plus profond]
    int i,j;
    int taille=DimJeu.N*DimJeu.M-Jeu->NbCasesJouees;//dimension maximale

    DefinirZoneEtendue(*Jeu, DimJeu, InfoZoneDeRecherche); //determine la zone a etudier (InfoZoneDeRecherche), Jeu nest aps modifie (d'ou *) mais InfoZoneDeRecherche est modifie (tableau passe en pointeur)
    DefinirListeCaseVide(Jeu, DimJeu, InfoZoneDeRecherche, logs);

    /**/ int nbcv;//NbCasesVides
    /**/    int lcv[80];//listecasesvides
    /**/    int tdc[16];//tabdecoef
    /**/    int tmc[16];//Jeu->tabMeilleuresCases[Jeu->NbMeilleuresCases]=

    /**/    int nmc;//=NbMeilleuresCases

    /**/int t=16;//taille du plateau de jeu
    /**/    for (i=0; i<5*t; i++)
    {
        /**/lcv[i]=Jeu->ListeCasesVides[i];
    }

    for (i=0; i<t; i++)
        /**/     tdc[i]=TabDeCoef[i];

    if(Jeu->NbCasesJouees==DimJeu.N*DimJeu.M-1) //S'il ne reste plus qu'une seule case à jouer, alors on la joue: c'est ListeCasesVides[0]
    {
//       for (i=0; i<5*t; i++)
//   lcv[i]=Jeu->ListeCasesVides[i];

        //Jeu->tabMeilleuresCases[0]=Jeu->ListeCasesVides_C0[0]; //Choisit la derniere case vide
        TabCroissant(Jeu->HistoriqueJeton, DimJeu.N*DimJeu.M-1);//on range toutes les cases jouées dans l'ordre croisant. Il n'y a qu'une suele case vide, toutes les cases sont Hist[i]=i (avant la case restante) ou Hist[i]= i-1 (apres la case restante)

//       for (i=0; i<5*t; i++)
//   lcv[i]=Jeu->ListeCasesVides[i];

        j=0;
        while(Jeu->HistoriqueJeton[j]==j)//tant que la case est au bon rang
            j=j+1;//on pâsse a la suivante
        Jeu->tabMeilleuresCases[0]=j;
        Jeu->NbMeilleuresCases=1;
        //     *NumeroCaseAJouer=j;
//  a=Jeu->tabMeilleuresCases[0];
    }
    else//si plusieurs choix, alors on lance on applique l'ia prevue
    {
        for (i=0; i<DimJeu.N*DimJeu.M; i++) //on rempli le tableau de -1
            TabDeCoef[i]=-1; //alors on ecrit -1 pour une "case occupée"

        for (i=0; i<NbCasesVides; i++) //on cherche les cases deja jouées
            TabDeCoef[Jeu->ListeCasesVides[i]]=0; //alors on ecrit 0 qui sera incrementé pour une "case à tester" et on garde //alors on ecrit -1 pour une "case occupée" a l'origine
//TabDeCoef contient TOUTES les cases du jeu avec un coef -1 si occupée et 0 si vide de TOUT le jeu
        for (i=0; i<t; i++)
            /**/     tdc[i]=TabDeCoef[i];

        /**/       for (i=0; i<5*t; i++)
        {
            /**/lcv[i]=Jeu->ListeCasesVides[i];
            //tdc[i]=TabDeCoef[i];*/
        }
        /**/nbcv=NbCasesVides;

        while((AlignementRealise==NON)&&(niveau<(DimJeu.NbJetonsAAligner-1)*2))
        {
	  SimuleCoup(niveau, *Jeu, plateau, Joueur, InfoZoneDeRecherche, DimJeu, TabDeCoef, &AlignementRealise, logs);//*Jeu car on ne modofie pas jeu
            niveau=niveau+1;
        }
        /**/       for (i=0; i<t; i++)
            /**/   tdc[i]=TabDeCoef[i];

        maximumTableau(TabDeCoef, DimJeu.N*DimJeu.M, &MeilleurScore, &NbMeilleurScore); //Recherche la valeur maximale du plateau de taille DimJeu.N*DimJeu.M  ainsi que le nombre de fois qu'eelle apparait.

        for (j=0; j<DimJeu.N*DimJeu.M; j++)//on va chercher toutes les cases de coef ayant le MeilleurScore
        {
            if(TabDeCoef[j]==MeilleurScore)
            {
                Jeu->tabMeilleuresCases[Jeu->NbMeilleuresCases]=j;
                Jeu->NbMeilleuresCases=Jeu->NbMeilleuresCases+1;//faut-il conserver ?
            }
        }
        for (i=0; i<t; i++)
        {
            /**/tdc[i]=TabDeCoef[i];
            /**/tmc[i]=Jeu->tabMeilleuresCases[i];
        }
        /**/     nmc=Jeu->NbMeilleuresCases;
    }
    /**/       for (i=0; i<t; i++)
        /**/tdc[i]=TabDeCoef[i];

    free(TabDeCoef);
    TabDeCoef=NULL;
}
/**********************/
void SimuleCoup(int niveau, CaracteristiqueJeu Jeu, Case *plateau,Joueur Joueur, Couronne InfoZoneDeRecherche[], DimPlateau DimJeu, int *TabDeCoef,int *AlignementRealise, t_logs *logs)
{
    //niveau = nombre de jetons a ajouter à NbCasesJouees
    int i,j,k,l;
    int NbCasesVides;//taille du tableau ListeCV, cases vides, que l'on va creer
    Case Ordi=(Joueur.ListeJ[(Jeu.NbCasesJouees)%2]).jeton;//Rond ou Croix, niveau 0 et 2 et 4 et 6
    Case Adversaire =(Joueur.ListeJ[(Jeu.NbCasesJouees+1)%2]).jeton;//Croix ou Rond, niveau 1 et 3 et 5 et 7
    Case JoueurTeste;
    int  NbAlignements=0;//initialisation
    int *ListeCV= NULL;//tableau servant a creer la ListeCaseVide utile pour le jeu (si grandes dimensions), tableau grande taille N*M

/**/int lcv[80];//4*5-4
/**/int lcv1[12];//4*5-4
/**/int tdc[16];
/**/int nbal=NbAlignements;
/**/int nbjaa=DimJeu.NbJetonsAAligner;//NbJetonsAAligner
/**/int cdp;//=InfoZoneDeRecherche[1].CaseDepart;
/**/int larg;//=InfoZoneDeRecherche[1].Largeur;
/**/int h;//=InfoZoneDeRecherche[1].hauteur;
/**/int nbcj = Jeu.NbCasesJouees;
/**/int t=16;
    //  int *ListeCV= FonctionAllocation1D(DimJeu.N*DimJeu.M, sizeof(*ListeCV));//tableau servant a creer la ListeCaseVide utile pour le jeu (si grandes dimensions), tableau grande taille N*M


    /**/       for (i=0; i<5*t; i++)
    {
        /**/lcv[i]=Jeu.ListeCasesVides[i];
    }

    /**/     for (i=0; i<t; i++)
    {
        /**/    tdc[i]=TabDeCoef[i];
    }



    switch (niveau) //determine les boutons a afficher
    {
    case 0 :
        JoueurTeste=Ordi;
        NbCasesVides=InfoZoneDeRecherche[1].Largeur*InfoZoneDeRecherche[1].Hauteur-Jeu.NbCasesJouees;
        free(ListeCV); //on libere l'ancien HistoriqueJeu
        ListeCV=NULL;
        ListeCV= FonctionAllocation1D(NbCasesVides, sizeof(*ListeCV), logs);//tableau servant a creer la ListeCaseVide, tableau grande taille N*M
        for (i = 0; i < NbCasesVides; i++) //on copie les bonnes cases vides dans la tableau ListeCV
            ListeCV[i]=Jeu.ListeCasesVides[i+1*DimJeu.N*DimJeu.M];

            /**/    cdp=InfoZoneDeRecherche[1].CaseDepart;
            /**/    larg=InfoZoneDeRecherche[1].Largeur;
            /**/    h=InfoZoneDeRecherche[1].Hauteur;

        break;
    case 1 :
        JoueurTeste=Adversaire;
        NbCasesVides=InfoZoneDeRecherche[1].Largeur*InfoZoneDeRecherche[1].Hauteur-Jeu.NbCasesJouees;
        free(ListeCV); //on libere l'ancien HistoriqueJeu
        ListeCV=NULL;
        ListeCV= FonctionAllocation1D(NbCasesVides, sizeof(*ListeCV), logs);//tableau servant a creer la ListeCaseVide, tableau grande taille N*M
        for (i = 0; i < NbCasesVides; i++) //on copie les bonnes cases vides dans la tableau ListeCV
            ListeCV[i]=Jeu.ListeCasesVides[i+1*DimJeu.N*DimJeu.M];
        break;
    case 2 :
        JoueurTeste=Ordi;
        NbCasesVides=InfoZoneDeRecherche[2].Largeur*InfoZoneDeRecherche[2].Hauteur-Jeu.NbCasesJouees;
        free(ListeCV); //on libere l'ancien HistoriqueJeu
        ListeCV=NULL;
        ListeCV= FonctionAllocation1D(NbCasesVides, sizeof(*ListeCV), logs);//tableau servant a creer la ListeCaseVide, tableau grande taille N*M
        for (i = 0; i < NbCasesVides; i++) //on copie les bonnes cases vides dans la tableau ListeCV
            ListeCV[i]=Jeu.ListeCasesVides[i+2*DimJeu.N*DimJeu.M];
        break;
    case 3 :
        JoueurTeste=Adversaire;
        NbCasesVides=InfoZoneDeRecherche[2].Largeur*InfoZoneDeRecherche[2].Hauteur-Jeu.NbCasesJouees;
        free(ListeCV); //on libere l'ancien HistoriqueJeu
        ListeCV=NULL;
        ListeCV= FonctionAllocation1D(NbCasesVides, sizeof(*ListeCV), logs);//tableau servant a creer la ListeCaseVide, tableau grande taille N*M
        for (i = 0; i < NbCasesVides; i++) //on copie les bonnes cases vides dans la tableau ListeCV
            ListeCV[i]=Jeu.ListeCasesVides[i+2*DimJeu.N*DimJeu.M];
        break;
    case 4 :
        JoueurTeste=Adversaire;
        NbCasesVides=InfoZoneDeRecherche[3].Largeur*InfoZoneDeRecherche[3].Hauteur-Jeu.NbCasesJouees;
        free(ListeCV); //on libere l'ancien HistoriqueJeu
        ListeCV=NULL;
        ListeCV= FonctionAllocation1D(NbCasesVides, sizeof(*ListeCV), logs);//tableau servant a creer la ListeCaseVide, tableau grande taille N*M
        for (i = 0; i < NbCasesVides; i++) //on copie les bonnes cases vides dans la tableau ListeCV
            ListeCV[i]=Jeu.ListeCasesVides[i+3*DimJeu.N*DimJeu.M];
        break;
    case 5 :
        JoueurTeste=Adversaire;
        NbCasesVides=InfoZoneDeRecherche[3].Largeur*InfoZoneDeRecherche[3].Hauteur-Jeu.NbCasesJouees;
        free(ListeCV); //on libere l'ancien HistoriqueJeu
        ListeCV=NULL;
        ListeCV= FonctionAllocation1D(NbCasesVides, sizeof(*ListeCV), logs);//tableau servant a creer la ListeCaseVide, tableau grande taille N*M
        for (i = 0; i < NbCasesVides; i++) //on copie les bonnes cases vides dans la tableau ListeCV
            ListeCV[i]=Jeu.ListeCasesVides[i+3*DimJeu.N*DimJeu.M];
        break;
    case 6 :
        JoueurTeste=Adversaire;
        NbCasesVides=InfoZoneDeRecherche[4].Largeur*InfoZoneDeRecherche[4].Hauteur-Jeu.NbCasesJouees;
        free(ListeCV); //on libere l'ancien HistoriqueJeu
        ListeCV=NULL;
        ListeCV= FonctionAllocation1D(NbCasesVides, sizeof(*ListeCV), logs);//tableau servant a creer la ListeCaseVide, tableau grande taille N*M
        for (i = 0; i < NbCasesVides; i++) //on copie les bonnes cases vides dans la tableau ListeCV
            ListeCV[i]=Jeu.ListeCasesVides[i+4*DimJeu.N*DimJeu.M];
        break;
    case 7 :
        JoueurTeste=Adversaire;
        NbCasesVides=InfoZoneDeRecherche[4].Largeur*InfoZoneDeRecherche[4].Hauteur-Jeu.NbCasesJouees;
        free(ListeCV); //on libere l'ancien HistoriqueJeu
        ListeCV=NULL;
        ListeCV= FonctionAllocation1D(NbCasesVides, sizeof(*ListeCV), logs);//tableau servant a creer la ListeCaseVide, tableau grande taille N*M
        for (i = 0; i < NbCasesVides; i++) //on copie les bonnes cases vides dans la tableau ListeCV
            ListeCV[i]=Jeu.ListeCasesVides[i+4*DimJeu.N*DimJeu.M];
        break;
    }

    Jeu.NbCasesJouees=Jeu.NbCasesJouees+(niveau+1);//on va jouer 1 coup pour ordi
/**/       for (i=0; i<16; i++)
    {
        /**/lcv1[i]=Jeu.ListeCasesVides[i];
    }
/**/ nbal=NbAlignements;

    switch(niveau)
    {
    case 0 ://test de 1 case
    case 1 :
        for (i = 0; i < NbCasesVides; i++) //pour chaque CaseVide, on teste son score
        {
            NbAlignements=0;//initialisation
            plateau[ListeCV[i]] = JoueurTeste;//On place le jeton de l'ordi
            Jeu.HistoriqueJeton[Jeu.NbCasesJouees-1]=ListeCV[i];//on actualise le dernier jeton joué
            verifFini(plateau, Joueur, DimJeu, &Jeu, &NbAlignements); //Fonction pour vérifier si la partie est terminée//on verfie s'il y a alignement

            if(NbAlignements!=0)//le jeton ne permet pas de gagner, il faut annuler et passer au suivant, inutile de modifier Jeu->NbCasesJouees (enlever puis le rajouter?)
            {
                TabDeCoef[ListeCV[i]]=TabDeCoef[ListeCV[i]]+1;//on incrémente le coef de la case testee
                *AlignementRealise=OUI;
            }
            AnnulerVerifFini(plateau, &Jeu, DimJeu);
            plateau[ListeCV[i]] = Vide;//on vide la case du plateau, inutile de liberer la derniere case de HistoriqueJeton
        }
        Jeu.HistoriqueJeton[Jeu.NbCasesJouees-1]=-1;//une fois toutes les possibilités testée, on vide le dernier jeton
       /**/ nbal=NbAlignements;

        break;
    case 2 ://test de 2 cases
    case 3 :
        for (i = 0; i < NbCasesVides; i++) //pour chaque CaseVide, on teste son score
        {
            for (j = 0; j < NbCasesVides; j++) //pour chaque CaseVide, on teste son score
            {
                if(j!=i)//si pas la meme case alors on simule sinon, on passe a la case j suivante
                {
                    NbAlignements=0;//initialisation
                    plateau[ListeCV[i]] = JoueurTeste;//On place le jeton de l'ordi
                    plateau[ListeCV[j]] = JoueurTeste;//On place le jeton de l'ordi
                    Jeu.HistoriqueJeton[Jeu.NbCasesJouees-2]=ListeCV[i];//on actualise l'avant dernier jeton joué
                    Jeu.HistoriqueJeton[Jeu.NbCasesJouees-1]=ListeCV[j];//on actualise le dernier jeton joué

                    verifFini(plateau, Joueur, DimJeu, &Jeu, &NbAlignements); //Fonction pour vérifier si la partie est terminée//on verfie s'il y a alignement

                    if(NbAlignements!=0)//le jeton ne permet pas de gagner, il faut annuler et passer au suivant, inutile de modifier Jeu->NbCasesJouees (enlever puis le rajouter?)
                    {
                        TabDeCoef[ListeCV[i]]=TabDeCoef[ListeCV[i]]+1;//on incrémente le coef de la case testee
                        TabDeCoef[ListeCV[j]]=TabDeCoef[ListeCV[j]]+1;
                        *AlignementRealise=OUI;
                    }
                    AnnulerVerifFini(plateau, &Jeu, DimJeu);
                    plateau[ListeCV[i]] = Vide;//on vide la case du plateau
                    plateau[ListeCV[j]] = Vide;//on vide la case du plateau
                }
            }
        }
        Jeu.HistoriqueJeton[Jeu.NbCasesJouees-2]=-1;//une fois toutes les possibilités testée, on vide l'avant dernier jeton
        Jeu.HistoriqueJeton[Jeu.NbCasesJouees-1]=-1;//une fois toutes les possibilités testée, on vide le dernier jeton
        break;
    case 4 ://test de 3 cases
    case 5 :
        for (i = 0; i < NbCasesVides; i++) //pour chaque CaseVide, on teste son score
        {
            for (j = 0; j < NbCasesVides; j++) //pour chaque CaseVide, on teste son score
            {
                if(j!=i)//si pas la meme case alors on simule sinon, on passe a la case j suivante
                {
                    for (k = 0; k < NbCasesVides; k++) //pour chaque CaseVide, on teste son score
                        if((k!=i)&&(k!=j))//si pas la meme case alors on simule sinon, on passe a la case j suivante
                        {
                            NbAlignements=0;//initialisation
                            plateau[ListeCV[i]] = JoueurTeste;//On place le jeton de l'ordi
                            plateau[ListeCV[j]] = JoueurTeste;//On place le jeton de l'ordi
                            plateau[ListeCV[k]] = JoueurTeste;//On place le jeton de l'ordi
                            Jeu.HistoriqueJeton[Jeu.NbCasesJouees-3]=ListeCV[i];//on actualise l'avant dernier jeton joué
                            Jeu.HistoriqueJeton[Jeu.NbCasesJouees-2]=ListeCV[j];//on actualise le dernier jeton joué
                            Jeu.HistoriqueJeton[Jeu.NbCasesJouees-1]=ListeCV[k];//on actualise le dernier jeton joué

                            verifFini(plateau, Joueur, DimJeu, &Jeu, &NbAlignements); //Fonction pour vérifier si la partie est terminée//on verfie s'il y a alignement

                            if(NbAlignements!=0)//le jeton ne permet pas de gagner, il faut annuler et passer au suivant, inutile de modifier Jeu->NbCasesJouees (enlever puis le rajouter?)
                            {
                                TabDeCoef[ListeCV[i]]=TabDeCoef[ListeCV[i]]+1;//on incrémente le coef de la case testee
                                TabDeCoef[ListeCV[j]]=TabDeCoef[ListeCV[j]]+1;
                                TabDeCoef[ListeCV[k]]=TabDeCoef[ListeCV[k]]+1;
                                *AlignementRealise=OUI;
                            }
                            AnnulerVerifFini(plateau, &Jeu, DimJeu);
                            plateau[ListeCV[i]] = Vide;//on vide la case du plateau
                            plateau[ListeCV[j]] = Vide;//on vide la case du plateau
                            plateau[ListeCV[k]] = Vide;//on vide la case du plateau
                        }
                }
            }
        }
        Jeu.HistoriqueJeton[Jeu.NbCasesJouees-3]=-1;//une fois toutes les possibilités testée, on vide l'avant-avant dernier jeton
        Jeu.HistoriqueJeton[Jeu.NbCasesJouees-2]=-1;//une fois toutes les possibilités testée, on vide l'avant dernier jeton
        Jeu.HistoriqueJeton[Jeu.NbCasesJouees-1]=-1;//une fois toutes les possibilités testée, on vide le dernier jeton
        break;
    case 6 ://test de 4 cases
    case 7 :
        for (i = 0; i < NbCasesVides; i++) //pour chaque CaseVide, on teste son score
        {
            for (j = 0; j < NbCasesVides; j++) //pour chaque CaseVide, on teste son score
            {
                if(j!=i)//si pas la meme case alors on simule sinon, on passe a la case j suivante
                {
                    for (k = 0; k < NbCasesVides; k++) //pour chaque CaseVide, on teste son score
                        if((k!=i)&&(k!=j))//si pas la meme case alors on simule sinon, on passe a la case j suivante
                        {
                            for (l = 0; l < NbCasesVides; l++) //pour chaque CaseVide, on teste son score
                                if((l!=i)&&(l!=j)&&(l!=k))//si pas la meme case alors on simule sinon, on passe a la case j suivante
                                {
                                    NbAlignements=0;//initialisation
                                    plateau[ListeCV[i]] = JoueurTeste;//On place le jeton de l'ordi
                                    plateau[ListeCV[j]] = JoueurTeste;//On place le jeton de l'ordi
                                    plateau[ListeCV[k]] = JoueurTeste;//On place le jeton de l'ordi
                                    plateau[ListeCV[l]] = JoueurTeste;//On place le jeton de l'ordi
                                    Jeu.HistoriqueJeton[Jeu.NbCasesJouees-4]=ListeCV[i];//on actualise l'avant dernier jeton joué
                                    Jeu.HistoriqueJeton[Jeu.NbCasesJouees-3]=ListeCV[i];//on actualise l'avant dernier jeton joué
                                    Jeu.HistoriqueJeton[Jeu.NbCasesJouees-2]=ListeCV[j];//on actualise le dernier jeton joué
                                    Jeu.HistoriqueJeton[Jeu.NbCasesJouees-1]=ListeCV[k];//on actualise le dernier jeton joué

                                    verifFini(plateau, Joueur, DimJeu, &Jeu, &NbAlignements); //Fonction pour vérifier si la partie est terminée//on verfie s'il y a alignement

                                    if(NbAlignements!=0)//le jeton ne permet pas de gagner, il faut annuler et passer au suivant, inutile de modifier Jeu->NbCasesJouees (enlever puis le rajouter?)
                                    {
                                        TabDeCoef[ListeCV[i]]=TabDeCoef[ListeCV[i]]+1;//on incrémente le coef de la case testee
                                        TabDeCoef[ListeCV[j]]=TabDeCoef[ListeCV[j]]+1;
                                        TabDeCoef[ListeCV[k]]=TabDeCoef[ListeCV[k]]+1;
                                        TabDeCoef[ListeCV[k]]=TabDeCoef[ListeCV[l]]+1;
                                        *AlignementRealise=OUI;
                                    }
                                    AnnulerVerifFini(plateau, &Jeu, DimJeu);
                                    plateau[ListeCV[i]] = Vide;//on vide la case du plateau
                                    plateau[ListeCV[j]] = Vide;//on vide la case du plateau
                                    plateau[ListeCV[k]] = Vide;//on vide la case du plateau
                                    plateau[ListeCV[l]] = Vide;//on vide la case du plateau
                                }
                        }
                }
            }
        }
        Jeu.HistoriqueJeton[Jeu.NbCasesJouees-4]=-1;//une fois toutes les possibilités testée, on vide l'avant-avant dernier jeton
        Jeu.HistoriqueJeton[Jeu.NbCasesJouees-3]=-1;//une fois toutes les possibilités testée, on vide l'avant-avant dernier jeton
        Jeu.HistoriqueJeton[Jeu.NbCasesJouees-2]=-1;//une fois toutes les possibilités testée, on vide l'avant dernier jeton
        Jeu.HistoriqueJeton[Jeu.NbCasesJouees-1]=-1;//une fois toutes les possibilités testée, on vide le dernier jeton
        break;
    }

    Jeu.NbCasesJouees=Jeu.NbCasesJouees-(niveau+1);//on annule le jeton joué pour l'ordi

    free(ListeCV);
    ListeCV=NULL;
}
/**********************/