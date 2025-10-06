#include "Main.h"
#include "Affichage.h"

/* extern FILE* fichierErreur;//extern car la variable globale est deja initialisee dans main.c */

/*********************************************/
void SDL_RenderCopyTransparent(t_morp_sdl *morp_sdl, SDL_Texture * texture,SDL_Rect *selection,SDL_Rect *position)
{
    SDL_SetTextureAlphaMod(texture, 100);//affichage de la texture2 avec une transparence de 8 /255
    SDL_RenderCopy(morp_sdl->renderer, texture, selection, position);
    SDL_SetTextureAlphaMod(texture, 255);//affichage de la texture2 avec une transparence de 8 /255
}

/*********************************************/
void AfficheTitre(t_morp_sdl *morp_sdl)
{
    SDL_Rect positionTitre = {(EcranHaut-(morp_sdl->jeton.DimensionImage[i_Titre]).w)/2, (EcranHaut-(morp_sdl->jeton.DimensionImage[i_Titre]).w)/2, (morp_sdl->jeton.DimensionImage[i_Titre]).w,(morp_sdl->jeton.DimensionImage[i_Titre]).h};//position de collage en haut a gauche

    /*   SDL_Surface *Lettre[11]= {NULL}; //On definit un tableau de 9 images
       int dimCase = PlateauLarg/3; //laisser la 3 et ne pas le remplacer par N car c'est pour l'affichage du titre
       int i,j;
       SDL_Rect positionTitre = {0, 0, (Jeton->DimensionImage[Titre]).w,(Jeton->DimensionImage[Titre]).h};//position de collage en haut a gauche
    SDL_Rect position = {0, 0, EcranLarg, EcranHaut}; //position de collage
        SDL_Rect SelectionZoneTitre= {(0,0,((Jeton->DimensionImage[Menu]).w)/3,(Jeton->DimensionImage[Menu]).h}; //pour choisir le bon menu parmi les 3 possibles (donc /3)
    */


    SDL_SetRenderDrawColor(morp_sdl->renderer, couleurImageMarron);//selectionne la couleurImageBlanc pour le fond (renderer), AVEC Alpha=255
    SDL_RenderClear(morp_sdl->renderer);//nettoie le fond
    SDL_RenderCopy(morp_sdl->renderer, morp_sdl->jeton.Image[i_Titre],NULL,&positionTitre); // Affiche le Titre
}

/*********************************************/
void AfficheRectVide(t_morp_sdl *morp_sdl, SDL_Rect position, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    SDL_SetRenderDrawColor(morp_sdl->renderer, r,g,b,a);//choix de la couleur
    SDL_RenderDrawRect(morp_sdl->renderer, &position);
}
/*********************************************/
void AfficheRectPlein(t_morp_sdl *morp_sdl, SDL_Rect position, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    SDL_SetRenderDrawColor(morp_sdl->renderer, r,g,b,a);//choix de la couleur
    SDL_RenderFillRect(morp_sdl->renderer, &position);
}
/*********************************************/
/*void AfficheRectPleinCadre(SDL_Renderer *renderer, SDL_Rect position, int epaisseurCadre, Uint8 rc, Uint8 gc, Uint8 bc, Uint8 ac, Uint8 rf, Uint8 gf, Uint8 bf, Uint8 af)//[Couleur cadre: rc, gc, bc, ac][Couleur fond : rf, gf, bf, af][SDL_Rect position = dimension exterieure du cadre][partie intérieure = position - 2 * epaisseurCadre]
{
     SDL_SetRenderDrawColor(renderer, rc,gc,bc,ac);//choix de la couleur pour le cadre
    SDL_RenderFillRect(renderer, &position);
    position.x=position.x+1;
    position.y=position.y+1;
    position.w=position.w-2*epaisseurCadre;
    position.h=position.h-2*epaisseurCadre;
SDL_SetRenderDrawColor(renderer, rf,gf,bf,af);//choix de la couleur pour le fond
    SDL_RenderFillRect(renderer, &position);
}*/
/*********************************************/
void AfficheBoutons(t_morp_sdl *morp_sdl, Joueur Joueur, DimPlateau DimJeu, EcranBouton EcranBouton)
{
    SDL_Rect position = {zonePlateauLarg+3, 0, EcranLarg-zonePlateauLarg, EcranHaut}; //On va coller un gros rectangle noir à la position recue

    AfficheRectPlein(morp_sdl, position, couleurImageMarron);//recouvre la marge avec la couleur de fond

//affiche les etats du bouton : actif inactif
    switch (EcranBouton) //determine les boutons a afficher
    {
    case EcranJeu :
        /* AfficheBoutonTexte(morp_sdl, Texte_Nouveau, ACTIF); //ecriture du bouton Nouveau */
        /* AfficheBoutonTexte(morp_sdl, Texte_Annuler, ACTIF); //ecriture du bouton Annuler */
        /* AfficheBoutonTexte(morp_sdl, Texte_CoudPouS, ACTIF); //ecriture du bouton CoupDePouce */
        AfficheBoutonConfigType(morp_sdl, Joueur, DimJeu.TypeJeu, ACTIF); //Affichage de l'image du bouton Config selon le type
        AfficheBoutonConfigDim(morp_sdl, DimJeu, ACTIF); //Affichage de l'image du bouton Config selon le type
        /* AfficheBoutonTexte(morp_sdl, Texte_Tournoi,  ACTIF); */
        AfficheBoutonTexte(morp_sdl, Texte_Valider, INVISIBLE);
        /* AfficheBoutonTexte(morp_sdl, Texte_Instruction, ACTIF); */
        /* AfficheBoutonTexte(morp_sdl, Texte_Quitter, ACTIF); */
        break;
    case EcranConfig:
    case EcranInstruction:
    case EcranTournoi:
        DimJeu.TypeJeu=VV;
        AfficheBoutonTexte(morp_sdl, Texte_Nouveau, INACTIF); //ecriture du bouton Nouveau
        AfficheBoutonTexte(morp_sdl, Texte_Annuler, INACTIF); //ecriture du bouton Annuler
        AfficheBoutonTexte(morp_sdl, Texte_CoudPouS, INACTIF); //ecriture du bouton CoupDePouce
        AfficheBoutonConfigType(morp_sdl, Joueur, DimJeu.TypeJeu, INACTIF); //Affichage de l'image du bouton Config selon le type
        AfficheBoutonConfigDim(morp_sdl, DimJeu, INACTIF); //Affichage de l'image du bouton Config selon le type
        AfficheBoutonTexte(morp_sdl, Texte_Tournoi, INACTIF);
        AfficheBoutonTexte(morp_sdl, Texte_Valider, ACTIF);
        AfficheBoutonTexte(morp_sdl, Texte_Instruction, INACTIF);
        AfficheBoutonTexte(morp_sdl, Texte_Quitter, ACTIF);
        break;
    default:
        break;
    }
    // SDL_RenderPresent(morp_sdl->renderer);//Actualisation
}

/*********************************************/
void AfficheBoutonTexte(t_morp_sdl *morp_sdl, int NumTexte, BUTTON_SDL_TYPE_OF_BUTTON_STATE Etat)
{
    SDL_Rect positionCadre = {BoutonX, 0, BoutonLargeur, BoutonHauteur}; //info collage : x, y, longueur , hauteur, le y est a adapter selon le bouton

    if(NumTexte==Texte_Annuler)//"Annuler"=0
        positionCadre.y=BoutonYAnnuler;
    if(NumTexte==Texte_CoudPouS)//"CoudPouS"=5
        positionCadre.y=BoutonYCoupDePouce;
    if(NumTexte==Texte_Instruction)//"Instruction"=10
        positionCadre.y=BoutonYInstruction;
    if(NumTexte==Texte_Nouveau) //"Nouveau"=15,determine l'ordonnée du bouton en fonction du mot à ecrire
        positionCadre.y=BoutonYNouveau;
    if(NumTexte==Texte_Quitter)//"Quitter"=20
        positionCadre.y=BoutonYQuitter;
    if(NumTexte==Texte_Tournoi)//"Quitter"=25
        positionCadre.y=BoutonYTournoi;
    if(NumTexte==Texte_Valider)//"Valider" =30
        positionCadre.y=BoutonYValider;



  //  positionFond.y = positionCadre.y+EpaisseurTraitCadre;// actualisation de la position du fond en fonction du bouton à placer

    SDL_Rect positionTexteDebut= {positionCadre.x, positionCadre.y+(BoutonHauteur-morp_sdl->jeton.DimensionTexteBouton[NumTexte].h)/2, morp_sdl->jeton.DimensionTexteBouton[Texte_Vide].w, morp_sdl->jeton.DimensionTexteBouton[NumTexte].h};//zone de la taille adaptée au mot à écrire
    SDL_Rect positionTexteSouligne= {positionTexteDebut.x+positionTexteDebut.w, positionTexteDebut.y, morp_sdl->jeton.DimensionTexteBouton[NumTexte+1].w, morp_sdl->jeton.DimensionTexteBouton[NumTexte].h};
    SDL_Rect positionTexteFin= {positionTexteSouligne.x+morp_sdl->jeton.DimensionTexteBouton[NumTexte+1].w, positionTexteDebut.y, morp_sdl->jeton.DimensionTexteBouton[NumTexte+2].w, morp_sdl->jeton.DimensionTexteBouton[NumTexte].h}; //centrage du mot dans la zone de bouton

    switch (Etat) //determine la couleur du texte, du cadre et du fond
    {
    case ACTIF://cadre: noir+ fond marron et ecriture noir

        //    AfficheRectPleinCadre(morp_sdl->renderer, positionCadre, EpaisseurTraitCadre, couleurImageNoir,255, couleurImageMarron,255);//
        AfficheRectVide(morp_sdl, positionCadre, couleurImageNoir,255);//le fond etant deja marron, il suffit de tracer une rectangle vide en noir
        //   AfficheRectVide(morp_sdl->renderer, positionCadre, couleurImageNoir,255);

        //   AfficheRectPlein(morp_sdl->renderer, positionCadre, couleurImageNoir,255);//Tracage du cadre en noir
        //   AfficheRectPlein(morp_sdl->renderer, positionFond, couleurImageMarron,255);//Coloriage du fond en marron
        SDL_RenderCopy(morp_sdl->renderer,morp_sdl->jeton.TexteBouton[Texte_Vide],NULL,&positionTexteDebut); // Affiche l'espace initial
        SDL_RenderCopy(morp_sdl->renderer,morp_sdl->jeton.TexteBouton[NumTexte+1],NULL,&positionTexteSouligne); // Affiche la lettre soulignee
        SDL_RenderCopy(morp_sdl->renderer,morp_sdl->jeton.TexteBouton[NumTexte+2],NULL,&positionTexteFin); // Affiche le reste du mot, non souligné
        break;
    case INACTIF://cadre: gris + fond marron et ecriture gris
        AfficheRectVide(morp_sdl, positionCadre, couleurImageGris,255);//le fond etant deja marron, il suffit de tracer une rectangle vide en noir

//     AfficheRectPlein(morp_sdl->renderer, positionCadre, couleurImageGris,255);//Trace du cadre en noir
        //      AfficheRectPlein(morp_sdl->renderer, positionFond, couleurImageMarron,255);//Coloriage du fond en marron
        SDL_RenderCopy(morp_sdl->renderer,morp_sdl->jeton.TexteBouton[Texte_Vide],NULL,&positionTexteDebut); // Affiche l'espace initial
        SDL_RenderCopy(morp_sdl->renderer,morp_sdl->jeton.TexteBouton[NumTexte+3],NULL,&positionTexteSouligne); // Affiche la lettre soulignee en greis
        SDL_RenderCopy(morp_sdl->renderer,morp_sdl->jeton.TexteBouton[NumTexte+4],NULL,&positionTexteFin); // Affiche le reste du mot, non souligné, en gris
        break;
    case INVISIBLE://cadre: marron+ fond marron et ecriture marron
        AfficheRectVide(morp_sdl, positionCadre, couleurImageMarron);//le fond etant deja marron, il suffit de tracer une rectangle vide en noir

        //    AfficheRectPlein(morp_sdl->renderer, positionCadre, couleurImageMarron,255);//Trace du cadre en noir
        //      AfficheRectPlein(morp_sdl->renderer, positionFond, couleurImageMarron,255);//Coloriage du fond en marron
        break;
    default:
        break;
    }
}
/*********************************************/

void AfficheBoutonConfigType(t_morp_sdl *morp_sdl, Joueur Joueur, jeu TypeJeu, BUTTON_SDL_TYPE_OF_BUTTON_STATE Etat) //Affichage de l'image du bouton Config selon le type
{
    SDL_Rect positionCadre = {BoutonX, BoutonYConfigType, BoutonLargeur, BoutonHauteur}; //position de collage
    SDL_Rect positionJ0 = {positionCadre.x+21, positionCadre.y+5, 20, 20}; //position de collage pointeur1 et triangles de selection par rapport au coin superieur du bouton typejeu
    SDL_Rect positionJ1 = {positionCadre.x+109, positionCadre.y+5, 20, 20}; //position de collage pointeur 2 par rapport au coin superieur du bouton typejeu
    SDL_Rect ZoneImageSelectionnee = {874, 0, 20, 20}; //selection de la derniere icone, la plus petite sur la 1ere ligne
    SDL_Rect positionMusique = {positionCadre.x+(BoutonLargeur-20)/2, positionCadre.y+5, 20, 20};
    SDL_Rect ZoneImageSelectionneeMusique = {0, 20 * morp_sdl->musique.Actuel, 20, 20}; //selection de la derniere icone, la plus petite sur la 1ere ligne

    switch (Etat)
    {
    case ACTIF: //on affiche en noir
        SDL_RenderCopy(morp_sdl->renderer, morp_sdl->jeton.Pion[(Joueur.ListeJ[0]).NumeroPion],&ZoneImageSelectionnee,&positionJ0); //On colle le pointeur1
        SDL_RenderCopy(morp_sdl->renderer, morp_sdl->jeton.Pion[(Joueur.ListeJ[1]).NumeroPion],&ZoneImageSelectionnee,&positionJ1); //On colle le pointeur1
        SDL_RenderCopy(morp_sdl->renderer, morp_sdl->jeton.Image[i_Musique],&ZoneImageSelectionneeMusique,&positionMusique); //On colle le pointeur1

        ZoneImageSelectionnee.x=TypeJeu*BoutonLargeur;
        ZoneImageSelectionnee.y=0;
        ZoneImageSelectionnee.w=BoutonLargeur;
        ZoneImageSelectionnee.h=BoutonHauteur;
        SDL_RenderCopy(morp_sdl->renderer, morp_sdl->jeton.Image[i_BoutonConfigType],&ZoneImageSelectionnee,&positionCadre); //On colle le pointeur1
        break;
    case INACTIF: //on affiche en gris
    case INVISIBLE: //on affiche en gris
        ZoneImageSelectionnee.x=TypeJeu*BoutonLargeur;
        ZoneImageSelectionnee.y=0;
        ZoneImageSelectionnee.w=BoutonLargeur;
        ZoneImageSelectionnee.h=BoutonHauteur;
        SDL_RenderCopy(morp_sdl->renderer, morp_sdl->jeton.Image[i_BoutonConfigType],&ZoneImageSelectionnee,&positionCadre); //On colle le pointeur1

        ZoneImageSelectionneeMusique.x=20; //pour afficher l'icone musique en gris
        SDL_RenderCopy(morp_sdl->renderer, morp_sdl->jeton.Image[i_Musique],&ZoneImageSelectionneeMusique,&positionMusique); //On colle le pointeur1

        ZoneImageSelectionnee.x = 874; //selection de la derniere icone, la plus petite ...
        ZoneImageSelectionnee.y=20; //... sur la 2eme ligne
        ZoneImageSelectionnee.w =20;
        ZoneImageSelectionnee.h =20;
        SDL_RenderCopy(morp_sdl->renderer, morp_sdl->jeton.Pion[(Joueur.ListeJ[0]).NumeroPion],&ZoneImageSelectionnee,&positionJ0); //On colle le pointeur0
        SDL_RenderCopy(morp_sdl->renderer, morp_sdl->jeton.Pion[(Joueur.ListeJ[1]).NumeroPion],&ZoneImageSelectionnee,&positionJ1); //On colle le pointeur1
        break;
    default:
        break;
    }
}
/*********************************************/

void AfficheBoutonConfigDim(t_morp_sdl *morp_sdl, DimPlateau DimJeu, BUTTON_SDL_TYPE_OF_BUTTON_STATE Etat) //Affichage de l'image du bouton Config selon le type
{
    SDL_Rect position = {BoutonX, BoutonYConfigDimension, BoutonLargeur, BoutonHauteur}; //position de collage
    SDL_Rect zoneSelectionnee = {(DimJeu.NbJetonsAAligner-3)*BoutonLargeur, 0, BoutonLargeur, BoutonHauteur}; //position de la zonne selectionnee dasn l'image BoutonConfigDimension

    switch(Etat)
    {
    case ACTIF:
        SDL_RenderCopy(morp_sdl->renderer, morp_sdl->jeton.Image[i_BoutonConfigDimension],&zoneSelectionnee,&position); //On colle le pointeur1
        break;
    case INACTIF:
    case INVISIBLE:
        zoneSelectionnee.y=BoutonHauteur;
        SDL_RenderCopy(morp_sdl->renderer, morp_sdl->jeton.Image[i_BoutonConfigDimension],&zoneSelectionnee,&position); //On colle le pointeur1
        break;
    default:
        break;
    }
}
/*********************************************/
void AffichePlateau(t_morp_sdl *morp_sdl, DimPlateau DimJeu)
{
    int DimensionCase=zonePlateauLarg/maximumEntre(DimJeu.N, DimJeu.M); //Dimension de la case carrée depend du maximum entre N et M
    int i,j;
    SDL_Rect position = {0, 0, zonePlateauLarg+25, EcranHaut}; //position de collage

    AfficheRectPlein(morp_sdl, position, couleurImageMarron);//Coloriage du fond en marron
    position.w=DimJeu.N*DimensionCase+2;
    position.h=DimJeu.M*DimensionCase+2;
    AfficheRectVide(morp_sdl, position, couleurImageNoir,255);//Collage du cadre rectangulaire extérieur

    position.h=DimensionCase;//case carrée
    position.w=position.h;
    for(i=0; i<DimJeu.N; i++) //On colle les cases sur les colonnes
    {
        for(j=0; j<DimJeu.M; j++) //On colle les cases sur les lignes
        {
            position.x=1+i*DimensionCase;//pour laisser le grand cadre et faire des ctraits depaisseur = 2
            position.y = 1+j*DimensionCase;
            AfficheRectVide(morp_sdl, position, couleurImageNoir,255);//Coloriage du fond en marron
        }
    }
}

/**********************/

void AffichePointeur(t_morp_sdl *morp_sdl, Case *plateau, Joueur Joueur, DimPlateau DimJeu, SDL_Rect Souris, CaracteristiqueJeu Jeu) //Affiche le bon pointeur de souris
{
    SDL_Texture *pointeurSouris; //Selectionne le pointeur Rond ou Croix
    SDL_Rect positionCaseGrise = {EpaisseurTraitJeu+(Souris.x/DimJeu.DimCase)*DimJeu.DimCase, EpaisseurTraitJeu+(Souris.y/DimJeu.DimCase)*DimJeu.DimCase, DimJeu.DimCase-EpaisseurTraitJeu, DimJeu.DimCase-EpaisseurTraitJeu}; //position de collage de la CaseGrise. Le +EpaisseurTraitJeu est pour ne pas depasser sur le trait noir en haut et a gauche
    SDL_Rect zoneDeCollage = {0, 0, DimJeu.DimCase-EpaisseurTraitJeu, DimJeu.DimCase-EpaisseurTraitJeu}; //de l'image grise, je colle la partie allant de (0, 0) et d'une zone de (DimCase-EpaisseurTraitJeu)*(DimCase-EpaisseurTraitJeu). Le -EpaisseurTraitJeu est pour ne pas depasser sur le trait noir en bas et à droite
    SDL_Rect ZoneImageSelectionnee = {874, 0, 20, 20}; //determine la zone de l'image a coller

    AfficheJeu(morp_sdl, plateau, Joueur, DimJeu, Jeu.PartieFinie); // On reaffiche le plateau+jeton pour effacer l'ancienne position du pointeur

    if((Souris.x<PlateauLarg)&&(Souris.y<PlateauHaut)) //si on pointe dans le plateau
    {
        SDL_ShowCursor(SDL_DISABLE); //[SDL_DISABLE : cache le pointeur][SDL_ENABLE : affiche le pointeur], alors on cache le pointeur-fleche

        if(Jeu.PartieFinie==NON) //[NON : on affiche le fond de la case en gris][OUI : on laisse le fond de case marron pour voir l'alignement]
            SDL_RenderCopyTransparent(morp_sdl, morp_sdl->jeton.Image[i_CaseGrise],&zoneDeCollage,&positionCaseGrise); //On colle la casegrise découpée à la bonne taille

        pointeurSouris=morp_sdl->jeton.Pion[Joueur.ListeJ[(Jeu.NbCasesJouees)%2].NumeroPion];//choix du bon pointeur selon le NbCasesJouees

        Souris.x = Souris.x - 10; //On recalcule les coordonnées de positions pour centrer le pointeur au centre l'image (le -10 est la demilargeur du pointeur)
        Souris.y = Souris.y - 10;
        Souris.w = 20;
        Souris.h = 20;
        SDL_RenderCopy(morp_sdl->renderer, pointeurSouris, &ZoneImageSelectionnee, &Souris); //Collage de la croix gagnante
    }
    else //sinon, si on est a l'exterieur du plateau, soit en dessous, soit a droite, alors
        SDL_ShowCursor(SDL_ENABLE); //alors on affiche le pointeur-fleche normal
}
/*********************************************/

void AfficheJeu(t_morp_sdl *morp_sdl, Case *plateau, Joueur Joueur, DimPlateau DimJeu, int PartieFinie)
{
    int i;
    SDL_Rect PositionCollage = {0, 0, DimJeu.DimCase, DimJeu.DimCase}; //position de collage des ronds et croix selon les valeurs du tableau. Le 1 est pour centrer dans la case
    SDL_Rect ZoneImageSelectionnee = {0, 0, DimJeu.DimCase, DimJeu.DimCase}; //determine la zone de l'image a coller
    int TableauDesAbscissesDesImages[24]= {200, 0, 150, 200, 120, 350, 100, 470, 75, 570, 60, 645, 50, 705, 40, 755, 30, 795, 25, 825, 24, 850, 20, 874}; //le tableau est une liste de couples (DimensionCase, ZoneImageSelectionnee.x)
    SDL_Rect zoneDeCollage = {0, 0, DimJeu.DimCase-(EpaisseurTraitJeu-1), DimJeu.DimCase-(EpaisseurTraitJeu-1)}; //de l'image grise, je colle la partie allant de (0, 0) et d'une zone de (DimCase-(EpaisseurTraitJeu-1))*(DimCase-(EpaisseurTraitJeu-1)). Le -(EpaisseurTraitJeu+1) est pour ne pas depasser sur le trait noir en bas et à droite

    AffichePlateau(morp_sdl, DimJeu); //Affichage du plateau vide

//on va selectionner la position du jeton dans le fichier image
    i=0;
    while (TableauDesAbscissesDesImages[2*i]!=DimJeu.DimCase) ///on parcourt le tableau tant que l'on n'a pas trouvé la bonne dimension
    {
        i++;
    }
    ZoneImageSelectionnee.x=TableauDesAbscissesDesImages[2*i+1]; //On choisis la
//On affiche des jetons

    if(PartieFinie==OUI) //On doit afficher [normal :oGagne, xGagne][fond gris : vide, oPerdu, xPerdu, CoupDePouce][impossible : Rond, Croix]
    {
        for(i=0; i<DimJeu.N*DimJeu.M; i++)
        {
            PositionCollage.x = 1+(i%DimJeu.N)*DimJeu.DimCase; //le 1 est pour centrer dans la case
            PositionCollage.y = 1+(i/DimJeu.N)*DimJeu.DimCase; //le 1 est pour centrer dans la case

            switch (plateau[i])
            {
            case xGagne:
                SDL_RenderCopy(morp_sdl->renderer, morp_sdl->jeton.Pion[(Joueur.ListeJ[1]).NumeroPion],&ZoneImageSelectionnee,&PositionCollage); //Collage du rond
                break;
            case oGagne:
                SDL_RenderCopy(morp_sdl->renderer, morp_sdl->jeton.Pion[(Joueur.ListeJ[0]).NumeroPion],&ZoneImageSelectionnee,&PositionCollage); //Collage du rond
                break;
            case Vide:
                SDL_RenderCopyTransparent(morp_sdl,morp_sdl->jeton.Image[i_CaseGrise],&zoneDeCollage,&PositionCollage); //Collage du rond
                break;
            case xPerd:
                SDL_RenderCopyTransparent(morp_sdl,morp_sdl->jeton.Image[i_CaseGrise],&zoneDeCollage,&PositionCollage); //On colle la casegrise découpée à la bonne taille
                SDL_RenderCopyTransparent(morp_sdl,morp_sdl->jeton.Pion[(Joueur.ListeJ[1]).NumeroPion],&ZoneImageSelectionnee,&PositionCollage); //Collage du rond
                // SDL_BlitSurface(Jeton->PionTransparent[(Joueur.ListeJ[1]).NumeroPion], &ZoneImageSelectionnee, Jeton->Ecran, &PositionCollage); //Collage du rond
                break;
            case oPerd:
                SDL_RenderCopyTransparent(morp_sdl,morp_sdl->jeton.Image[i_CaseGrise],&zoneDeCollage,&PositionCollage); //On colle la casegrise découpée à la bonne taille
                SDL_RenderCopyTransparent(morp_sdl,morp_sdl->jeton.Pion[(Joueur.ListeJ[0]).NumeroPion],&ZoneImageSelectionnee,&PositionCollage); //Collage du rond
                // SDL_BlitSurface(Jeton->PionTransparent[(Joueur.ListeJ[0]).NumeroPion], &ZoneImageSelectionnee, Jeton->Ecran, &PositionCollage); //Collage du rond
                break;
            default:
                break;
            }
        }
    }
    else //On doit afficher [normal :Rond, Croix, vide ]|fond gris : aucune]|impossible : oGagne, xGagne, oPerdu, xPerdu ]
    {
        for(i=0; i<DimJeu.N*DimJeu.M; i++)
        {
            PositionCollage.x = 1+(i%DimJeu.N)*DimJeu.DimCase; //le 1 est pour centrer dans la case
            PositionCollage.y = 1+(i/DimJeu.N)*DimJeu.DimCase; //le 1 est pour centrer dans la case

            switch (plateau[i])
            {
            case Vide: //si la case est vide, alors on ne colle rien
                break;
            case Rond:
                SDL_RenderCopy(morp_sdl->renderer,morp_sdl->jeton.Pion[(Joueur.ListeJ[0]).NumeroPion],&ZoneImageSelectionnee,&PositionCollage); //Collage du rond
                break;
            case Croix:
                SDL_RenderCopy(morp_sdl->renderer,morp_sdl->jeton.Pion[(Joueur.ListeJ[1]).NumeroPion],&ZoneImageSelectionnee,&PositionCollage); //Collage du rond
                break;
            case CoupDePouce:
                SDL_RenderCopyTransparent(morp_sdl,morp_sdl->jeton.Image[i_CaseGrise],&zoneDeCollage,&PositionCollage); //On colle la casegrise découpée à la bonne taille
                break;
            default:
                break;
            }
        }
    }
}
/*********************************************/

/* Callback appelé lorsque le bouton Nouveau est cliqué */
void
clic_sur_le_bouton_nouveau (t_button_sdl *button, void *userdata) {
  printf ("Enter in %s ();\n", __func__);

}

/* Callback appelé lorsque le bouton Annuler est cliqué */
void
clic_sur_le_bouton_annuler (t_button_sdl *button, void *userdata) {
  printf ("Enter in %s ();\n", __func__);

}

/* Callback appelé lorsque le bouton CousPous est cliqué */
void
clic_sur_le_bouton_coudpous (t_button_sdl *button, void *userdata) {
  printf ("Enter in %s ();\n", __func__);

}

/* Callback appelé lors du clic sur le bouton de fermeture de la fenêtre de dialogue
 * du configurateur du plateau de jeu */
void clic_sur_le_bouton_de_fermeture_du_dialogue (t_button_sdl, void *userdata) {
	t_dialog_sdl *dialog = (t_dialog_sdl*)userdata;

	/* Rend invisible la fernêtre de dialogue */
	widget_sdl_set_visible(dialog_sdl_get_widget(dialog), false);
}

/* Callback appelé lors du clic sur le bouton de validation de la fenêtre de dialogue
 * du configurateur du plateau de jeu */
void clic_sur_le_bouton_de_validation_du_dialogue (t_button_sdl, void *userdata) {
	t_dialog_sdl *dialog = (t_dialog_sdl*)userdata;

	/* Rend invisible la fernêtre de dialogue */
	widget_sdl_set_visible(dialog_sdl_get_widget(dialog), false);

	/* Change les caractéristiques du plateau jeu */

	/* Initialise le jeu */

}

/* Callback appelé lorsque le bouton configurateur de plateau de jeu est cliqué */
void
clic_sur_le_bouton_size_config(t_button_sdl *button, void *userdata) {
	t_ihm_sdl *ihm_sdl = (t_ihm_sdl*)userdata;

	/* Création d'une fenêtre de dialogue */
	static t_dialog_sdl *dialog = NULL;

	/* Si c'est la première fois qu'on ouvre le dialogue alors on le crée */
	if (dialog == NULL)
		dialog = dialog_sdl_new((SDL_Rect){200, 100, 400, 400}, ihm_sdl_get_logs(ihm_sdl));
	else { // On rend la fenêtre de dialogue visible
		widget_sdl_set_visible(dialog_sdl_get_widget(dialog), true);
		return;
	}

	/* Ajout d'un titre */
	dialog_sdl_set_title (dialog, "Changement de la taille du plateau de jeu");

	/* Changement de la couleur de fond */
  widget_sdl_set_color (dialog_sdl_get_widget(dialog), (SDL_Color){231, 226, 226, 255}, FOND);

	/* Récupération des boutons "Fermeture", "Annuler" et "Valider" pour configuration */
	t_button_sdl *close_button = dialog_sdl_get_close_button  (dialog);

	/* Changement de la couleur de fond */
  widget_sdl_set_color (button_sdl_get_widget(close_button), (SDL_Color){231, 226, 226, 255}, FOND);

	/* Ajout d'une bulle d'aide */
	widget_sdl_set_tooltip (button_sdl_get_widget(close_button), "Fermeture sans modification de la configuration");

	/* Ajout d'un callback pour gérer le clic de la souris.
	* Ce callback va rendre la fenêtre de dialogue invisible */
	widget_sdl_set_mouse_clic_callback(button_sdl_get_widget(close_button), clic_sur_le_bouton_de_fermeture_du_dialogue, dialog);

	t_button_sdl *annul_button = dialog_sdl_get_annul_button  (dialog);

	/* Changement de la couleur de fond */
  widget_sdl_set_color (button_sdl_get_widget(annul_button), (SDL_Color){231, 226, 226, 255}, FOND);

	/* Ajout d'une bulle d'aide */
	widget_sdl_set_tooltip (button_sdl_get_widget(annul_button), "Fermeture sans modification de la configuration");

	/* Ajout d'un callback pour gérer le clic de la souris.
	* Ce callback va rendre la fenêtre de dialogue invisible */
	widget_sdl_set_mouse_clic_callback(button_sdl_get_widget(annul_button), clic_sur_le_bouton_de_fermeture_du_dialogue, dialog);

	t_button_sdl *valid_button = dialog_sdl_get_valid_button  (dialog);

	/* Changement de la couleur de fond */
  widget_sdl_set_color (button_sdl_get_widget(valid_button), (SDL_Color){231, 226, 226, 255}, FOND);

	/* Ajout d'une bulle d'aide */
	widget_sdl_set_tooltip (button_sdl_get_widget(valid_button), "La partie en cours sera initialisée");

	/* Ajout d'un callback pour gérer le clic de la souris.
	* Ce callback va rendre la fenêtre de dialogue invisible, modifier les caractéristiques du jeu et initialiser une nouvelle partie */
	widget_sdl_set_mouse_clic_callback(button_sdl_get_widget(annul_button), clic_sur_le_bouton_de_validation_du_dialogue, dialog);


	/* Création et insertion du configurateur dans la fenêtre de dialogue */
	/* Récupération de la position et de la taille de la fenêtre de dialogue */
	SDL_Rect dial_size;
	widget_sdl_get_size (dialog_sdl_get_widget(dialog), &dial_size);

	int config_width = 210;
  int config_height = 210;
  /* Création du configateur centré sur le dialogue */
  t_game_config_sdl *game_config = game_config_sdl_new ((SDL_Rect){dial_size.x + dial_size.w / 2 - config_width /2,
																												dial_size.y + dial_size.h / 2 - config_height / 2,
																												config_width, config_height}, ihm_sdl_get_logs(ihm_sdl));

  /* Insertion du configurateur dans la fenêtre de dialogue */
  widget_sdl_add_child_widget(dialog_sdl_get_widget(dialog), game_config_sdl_get_widget(game_config));

  /* Insertion de la fenêtre de dialogue dans l'ihm */
  ihm_sdl_widget_append (ihm_sdl, dialog_sdl_get_widget(dialog));
}

/* Callback appelé lorsque le bouton Tournoi est cliqué */
void
clic_sur_le_bouton_tournoi (t_button_sdl *button, void *userdata) {
  printf ("Enter in %s ();\n", __func__);

}

/* Callback appelé lorsque le bouton Quitter est cliqué */
void
clic_sur_le_bouton_quitter (t_button_sdl *button, void *userdata) {
  printf ("Enter in %s ();\n", __func__);

}

/* Callback appelé lorsqu'une case est cliquée */
void
clic_sur_une_case (t_button_sdl *button, void *userdata) {
  printf ("Enter in %s ();\n", __func__);

}

/* Callback appelé lorsque le bouton à bascule est cliqué */
void clic_sur_un_bouton_a_bascule (t_toggle_button_sdl *toggle_button, void *userdata) {
  printf ("Enter in %s\n", __func__);
  /* Affichage de l'état du bouton */
  if (toggle_button_sdl_button1_is_active (toggle_button) == true)
    printf ("Bouton 1 est actif\n");
  else
    printf ("Bouton 2 est actif\n");
}

static void
creation_plateau_de_jeu (t_ihm_sdl *ihm_sdl, unsigned int taille) {
	t_logs *logs = ihm_sdl->logs;
  int nbre_casex = taille;
  int nbre_casey = taille;

  t_case_sdl *case_sdl [nbre_casex*nbre_casey];

  /* Récupération de la taille de la fenêtre */
  SDL_Window *window = SDL_RenderGetWindow(ihm_sdl->renderer);
  int winx, winy;
  SDL_GetWindowSize(window, &winx, &winy);

  /* Création des cases et insertion dans le plateau */
  int dim_casex, dim_casey;
  dim_casey = (int)(winy/(double)nbre_casey);
  dim_casex = dim_casey;

  for (int i = 0; i < nbre_casey; i++) {
    for (int j = 0; j < nbre_casex; j++) {
      case_sdl[j + i*nbre_casex] = case_sdl_new ((SDL_Rect){j*dim_casex, i*dim_casey, dim_casex, dim_casey}, logs);
      case_sdl[j + i*nbre_casex]->type_de_case = VIDE;

       /* Changement de la couleur de fond par défaut */
      widget_sdl_set_color (case_sdl_get_widget (case_sdl[j + i*nbre_casex]), couleurMarron, FOND);

      /* Changement de la couleur de fond lorsque la souris est dessus */
      widget_sdl_set_color (case_sdl_get_widget (case_sdl[j + i*nbre_casex]), couleurMarron_active, ACTIVE);

      /* Changement de la couleur de fond lorsque la case est ensensible */
      widget_sdl_set_color (case_sdl_get_widget (case_sdl[j + i*nbre_casex]), couleurMarron, INSENSIBLE);

      /* Change le cursor lorsque la souris est dessus */
      SDL_Surface *case_noire = IMG_Load ("Ressources/croix_noire.png");
      if (case_noire==NULL)
        fprintf (stderr, "%s\n", SDL_GetError());

      widget_sdl_set_cursor_from_surface (case_sdl_get_widget (case_sdl[j + i*nbre_casex]), case_noire);

      /* Affectation d'un callback lors du clic de la souris sur la case.
       * Pour l'exemple affiche les caractèritisques de la case en console */
      widget_sdl_set_mouse_clic_callback (case_sdl_get_widget (case_sdl[j + i*nbre_casex]),
					  clic_sur_une_case, NULL);

      /* Insertion de la case dans l'ihm */
      ihm_sdl_widget_append (ihm_sdl, case_sdl_get_widget (case_sdl[j + i*nbre_casex]));
    }
  }
}

t_ihm_sdl*
creation_interface (t_morp_sdl *morp_sdl) {
  t_ihm_sdl *ihm_sdl = ihm_sdl_new (morp_sdl->renderer);
  if (ihm_sdl == NULL)
    return NULL;

 /* Création d'un bouton Nouveau */
  t_button_sdl *bouton_nouveau = button_sdl_new (TEXTE, morp_sdl->police, (SDL_Rect){630, 50, 150, 40}, ihm_sdl_get_logs(ihm_sdl));
  button_sdl_set_text (bouton_nouveau, "Nouveau", TTF_STYLE_NORMAL);

  /* Change le curseur lorsque la souris est sur le bouton */
  widget_sdl_set_cursor_from_file (button_sdl_get_widget (bouton_nouveau), cursor_sdl);

  /* Affectation d'un callback lors du clic de la souris sur le bouton.
   * Pour l'exemple on attache un texte à la donnée personnelle */
  widget_sdl_set_mouse_clic_callback (button_sdl_get_widget (bouton_nouveau), clic_sur_le_bouton_nouveau, NULL);

  /* Pour changer la couleur de fond du bouton */
  widget_sdl_set_color (button_sdl_get_widget (bouton_nouveau), couleurTexteMarron, FOND);

  /* Ajout d'une bulle d'aide */
  widget_sdl_set_tooltip (button_sdl_get_widget (bouton_nouveau),
			  "Réinitialise le jeu. Pour gagner il suffit d'aligner verticalement, horizontalement ou en diagonal trois pions identiques.");

  /* Insertion du bouton Nouveau dans l'ihm */
  ihm_sdl_widget_append (ihm_sdl, button_sdl_get_widget (bouton_nouveau));

  /*******************/

  /* Création du bouton Annuler */
  t_button_sdl *bouton_annuler = button_sdl_new (TEXTE, morp_sdl->police, (SDL_Rect){630, 150, 150, 40}, ihm_sdl_get_logs(ihm_sdl));
  button_sdl_set_text (bouton_annuler, "Annuler", TTF_STYLE_NORMAL);

  /* Change le curseur lorsque la souris est sur le bouton */
  widget_sdl_set_cursor_from_file (button_sdl_get_widget (bouton_annuler), cursor_sdl);

  /* Affectation d'un callback lors du clic de la souris sur le bouton. */
  widget_sdl_set_mouse_clic_callback (button_sdl_get_widget (bouton_annuler), clic_sur_le_bouton_annuler, NULL);

  /* Pour changer la couleur de fond du bouton */
  widget_sdl_set_color (button_sdl_get_widget (bouton_annuler), couleurTexteMarron, FOND);

  /* Ajout d'une bulle d'aide */
  widget_sdl_set_tooltip (button_sdl_get_widget (bouton_annuler), "Annule le dernier coup joué.");

  /* Insertion du bouton Annuler dans l'ihm */
  ihm_sdl_widget_append (ihm_sdl, button_sdl_get_widget (bouton_annuler));

  /*******************/

  /* Création du bouton CoudPous */
  t_button_sdl *bouton_coudpous = button_sdl_new (TEXTE, morp_sdl->police, (SDL_Rect){630, 190, 150, 40}, ihm_sdl_get_logs(ihm_sdl));
  button_sdl_set_text (bouton_coudpous, "CoudPous", TTF_STYLE_NORMAL);

  /* Change le curseur lorsque la souris est sur le bouton */
  widget_sdl_set_cursor_from_file (button_sdl_get_widget (bouton_coudpous), cursor_sdl);

  /* Affectation d'un callback lors du clic de la souris sur le bouton. */
  widget_sdl_set_mouse_clic_callback (button_sdl_get_widget (bouton_coudpous), clic_sur_le_bouton_coudpous, NULL);

  /* Pour changer la couleur de fond du bouton */
  widget_sdl_set_color (button_sdl_get_widget (bouton_coudpous), couleurTexteMarron, FOND);

  /* Ajout d'une bulle d'aide */
  widget_sdl_set_tooltip (button_sdl_get_widget (bouton_coudpous), "Propose un coup calculé par l'IA.");


  /* Insertion du bouton CoudPous dans l'ihm */
  ihm_sdl_widget_append (ihm_sdl, button_sdl_get_widget (bouton_coudpous));

  /*******************/

  /* Création d'un bouton pour l'outil de configuration de la taille du plateau */
  t_button_sdl *bouton_size_config = button_sdl_new (IMAGE, NULL, (SDL_Rect){630, 420, 150, 40}, ihm_sdl_get_logs(ihm_sdl));
  button_sdl_set_image_from_file (bouton_size_config, "ihm/images/config_size_3.png");

  /* Change le curseur lorsque la souris est sur le bouton */
  widget_sdl_set_cursor_from_file (button_sdl_get_widget (bouton_size_config), cursor_sdl);

  /* Affectation d'un callback lors du clic de la souris sur le bouton. */
  widget_sdl_set_mouse_clic_callback (button_sdl_get_widget (bouton_size_config), clic_sur_le_bouton_size_config, ihm_sdl);

  /* Pour changer la couleur de fond du bouton */
  widget_sdl_set_color (button_sdl_get_widget (bouton_size_config), couleurTexteMarron, FOND);

  /* Ajout d'une bulle d'aide */
  widget_sdl_set_tooltip (button_sdl_get_widget (bouton_size_config), "Modification de la taille du plateau de jeu.");

  /* Insertion du bouton CoudPous dans l'ihm */
  ihm_sdl_widget_append (ihm_sdl, button_sdl_get_widget (bouton_size_config));

  /*******************/

  /* Création du bouton Tournoi */
  t_button_sdl *bouton_tournoi = button_sdl_new (TEXTE, morp_sdl->police, (SDL_Rect){630, 350, 150, 40}, ihm_sdl_get_logs(ihm_sdl));
  button_sdl_set_text (bouton_tournoi, "Tournoi", TTF_STYLE_NORMAL);

  /* Change le curseur lorsque la souris est sur le bouton */
  widget_sdl_set_cursor_from_file (button_sdl_get_widget (bouton_tournoi), cursor_sdl);

  /* Affectation d'un callback lors du clic de la souris sur le bouton. */
  widget_sdl_set_mouse_clic_callback (button_sdl_get_widget (bouton_tournoi), clic_sur_le_bouton_tournoi, NULL);

  /* Pour changer la couleur de fond du bouton */
  widget_sdl_set_color (button_sdl_get_widget (bouton_tournoi), couleurTexteMarron, FOND);

  /* Ajout d'une bulle d'aide */
  widget_sdl_set_tooltip (button_sdl_get_widget (bouton_tournoi), "Lance un tournoi qui peut aller jusqu'à dix manches.");

  /* Insertion du bouton Tournoi dans l'ihm */
  ihm_sdl_widget_append (ihm_sdl, button_sdl_get_widget (bouton_tournoi));

  /*******************/

  /* Création de deux boutons à bascule pour choisir les adversaires */
  t_toggle_button_sdl *left_toogle_button = toggle_button_sdl_new ((SDL_Rect){650, 390, 40, 20}, ihm_sdl_get_logs(ihm_sdl));

  /* Récupération des deux boutons pour configuration */
  t_state_button_sdl *button1 = toggle_button_sdl_get_state_button (left_toogle_button, BUTTON1);
  t_state_button_sdl *button2 = toggle_button_sdl_get_state_button (left_toogle_button, BUTTON2);

  /* Ajout d'une image pour chaque bouton */
  state_button_sdl_set_image (button1, "./Ressources/image_humain.png");
  state_button_sdl_set_image (button2, "./Ressources/image_ordi.png");

  /* Change le curseur lorsque la souris est sur le bouton */
  widget_sdl_set_cursor_from_file (state_button_sdl_get_widget (button1), cursor_sdl);
  widget_sdl_set_cursor_from_file (state_button_sdl_get_widget (button2), cursor_sdl);

  /* Affectation d'un callback lors du clic de la souris sur chaque bouton. */
  widget_sdl_set_mouse_clic_callback (toggle_button_sdl_get_widget (left_toogle_button), clic_sur_un_bouton_a_bascule, NULL);

  /* Ajout d'une bulle d'aide */
  widget_sdl_set_tooltip (toggle_button_sdl_get_widget (left_toogle_button), "Choix du joueur qui débute la partie.");

  /* Insertion du bouton à bascule dans l'ihm */
  ihm_sdl_widget_append (ihm_sdl, toggle_button_sdl_get_widget (left_toogle_button));

  t_toggle_button_sdl *right_toogle_button = toggle_button_sdl_new ((SDL_Rect){720, 390, 40, 20}, ihm_sdl_get_logs(ihm_sdl));

  /* Récupération des deux boutons pour configuration */
  button1 = toggle_button_sdl_get_state_button (right_toogle_button, BUTTON1);
  button2 = toggle_button_sdl_get_state_button (right_toogle_button, BUTTON2);

  /* Ajout d'une image pour chaque bouton */
  state_button_sdl_set_image (button1, "./Ressources/image_humain.png");
  state_button_sdl_set_image (button2, "./Ressources/image_ordi.png");

  /* Affectation d'un callback lors du clic de la souris sur chaque bouton. */
  widget_sdl_set_mouse_clic_callback (toggle_button_sdl_get_widget (right_toogle_button), clic_sur_un_bouton_a_bascule, NULL);

  /* Ajout d'une bulle d'aide */
  widget_sdl_set_tooltip (toggle_button_sdl_get_widget (right_toogle_button), "Choix du joueur qui joue en second.");

  /* Insertion du bouton à bascule dans l'ihm */
  ihm_sdl_widget_append (ihm_sdl, toggle_button_sdl_get_widget (right_toogle_button));

  /*******************/

  /* Création du bouton Quitter */
  t_button_sdl *bouton_quitter = button_sdl_new (TEXTE, morp_sdl->police, (SDL_Rect){630, 500, 150, 40}, ihm_sdl_get_logs(ihm_sdl));
  button_sdl_set_text (bouton_quitter, "quitter", TTF_STYLE_NORMAL);

  /* Change le curseur lorsque la souris est sur le bouton */
  widget_sdl_set_cursor_from_file (button_sdl_get_widget (bouton_quitter), cursor_sdl);

  /* Affectation d'un callback lors du clic de la souris sur le bouton. */
  widget_sdl_set_mouse_clic_callback (button_sdl_get_widget (bouton_quitter), clic_sur_le_bouton_quitter, NULL);

  /* Pour changer la couleur de fond du bouton */
  widget_sdl_set_color (button_sdl_get_widget (bouton_quitter), couleurTexteMarron, FOND);

  /* Insertion du bouton Quitter dans l'ihm */
  ihm_sdl_widget_append (ihm_sdl, button_sdl_get_widget (bouton_quitter));

  /* Création et insertion du plateau de jeu */
  creation_plateau_de_jeu (ihm_sdl, 3);

  return ihm_sdl;
}
