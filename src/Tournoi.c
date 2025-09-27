#include "Main.h"

extern FILE* fichierErreur;//extern car la variable globale est deja initialisee dans main.c

/*********************************************/
void initTournoi(t_morp_sdl *morp_sdl, MancheTournoi *Tournoi, int MaxMancheTournoi, SDL_Surface **TitreColonne,SDL_Rect *positionTitreColonne,SDL_Rect *PositionZonesCliquables,SDL_Rect *PositionZonesTournoiAAfficher,SDL_Rect *PositionZonesTournoiASelectionner)
{
    /*
       int i,j;
       int PositionY=50;
       char *Mot[4]= {"Manche ", "Joueur 1", "Joueur 2", "Dimensions"}; //liste des titres a afficher
       char texte[10];

       for (i=0; i<4; i++) //Creation des zones de titres
       {
           TitreColonne[i]=TTF_RenderText_Blended(police.normal, Mot[i], couleurTexteNoir); //creation des zones de titres en fonction du mot à afficher, avec la couleur noire

           if(i==0) //creation des position des mots titres
               positionTitreColonne[0].x=50;
           else
               positionTitreColonne[i].x=positionTitreColonne[i-1].x+TitreColonne[i-1]->w+10; //creation des positions de la colonne 0

           positionTitreColonne[i].y=PositionY;
           positionTitreColonne[i].w=TitreColonne[i]->w;
           positionTitreColonne[i].h=TitreColonne[i]->h;
       }

       for(i=0; i<NbLignesTournoi; i++) //on initialise les 10 manches possibles
       {
           Tournoi[i].numeroDeManche=i+1; //definit le numero
           Tournoi[i].Joueur1.joueur=H; //definit le joueur 1 : H ou O
           Tournoi[i].Joueur1.jeton=Rond; //definit le joueur 1 : Rond ou Croix
           Tournoi[i].Joueur1.NumeroPion=0;
           Tournoi[i].Joueur2.joueur=H; //definit le joueur 2 : H ou O
           Tournoi[i].Joueur2.jeton=Rond; //definit le joueur 1 : Rond ou Croix
           Tournoi[i].Joueur2.NumeroPion=1;
         //  Tournoi[i].NumeroMusique=0;

         memcpy(Tournoi[i].Musique.Liste,Musique.Liste,sizeof(Musique.Liste));//copie dans AncienDimJeu des données de DimJeu
           Tournoi[i].Musique.Actuel=0;
           Tournoi[i].Musique.Ancien=0;
           Tournoi[i].TypeJeu=(Tournoi[i].Joueur1.joueur)*2+Tournoi[i].Joueur2.joueur;
           Tournoi[i].dimM=3; //definit la dimension M
           Tournoi[i].dimN=3; //definit la dimension N
           Tournoi[i].NbJetonsAAligner=3; //definit le nombre de jetons à aligner
       }


      // musique a[10];
      // for(i=0; i<NbMatchMax; i++)
      //     a[i]=Tournoi[i].Musique;

    //liste des zones cliquables du mode tournoi : il y a 10*7+3=73 zones cliquables (10 lignes de 7 zones (n°, icone J1, type J1, icone J2, type J2, musique, dim), les boutons valider et quitter, et une zone null)
    //initialisation de la 1ere ligne
    //initialisation des lignes 2->10
    //modification de la composante w de la ligne 10 (car 10 et plus long à ecrire que 1) et qu'il faut centrer l'image
       sprintf(texte, "%d", 1); //Determine la taille de la sdl_surface pour definir le numero de la manche (1 par exemple)
       SDL_Surface *TexteColonne1=TTF_RenderText_Blended(police.normal, texte, couleurTexteNoir); //peu importe la couleur, on veut juste la taille

       SDL_Rect positionColonne23= {(positionTitreColonne[1].x+positionTitreColonne[2].x+positionTitreColonne[2].w-BoutonLargeur)/2, 2*PositionY, BoutonLargeur, BoutonHauteur}; //Image centrée sur les mots "Joueur 1 et 2"

       PositionZonesCliquables[ZC_Manche].x= positionTitreColonne[0].x+(positionTitreColonne[0].w-TexteColonne1->w)/2; //centré horizontalement sous le mot "Manche" et verticalement sur l'image
       PositionZonesCliquables[ZC_Manche].y= positionTitreColonne[0].y+PositionY+(Jeton->BoutonConfigType->h-TexteColonne1->h)/2;
       PositionZonesCliquables[ZC_Manche].w= TexteColonne1->w;
       PositionZonesCliquables[ZC_Manche].h= TexteColonne1->h;

       PositionZonesTournoiAAfficher[0].x= PositionZonesCliquables[0].x; //pour afficher le numero de la manche
       PositionZonesTournoiAAfficher[0].y= PositionZonesCliquables[0].y;
       PositionZonesTournoiAAfficher[0].w= PositionZonesCliquables[0].w;
       PositionZonesTournoiAAfficher[0].h= PositionZonesCliquables[0].h;

       PositionZonesCliquables[ZC_J1Icone].x= positionColonne23.x+21; //Image centrée sur les mots "Joueur 1 et 2", affichage de l'icone J1
       PositionZonesCliquables[ZC_J1Icone].y= positionColonne23.y+5;
       PositionZonesCliquables[ZC_J1Icone].w= 20;
       PositionZonesCliquables[ZC_J1Icone].h= 20;

       PositionZonesTournoiAAfficher[1].x= positionColonne23.x; //pour afficher le bouton configType centré sous J1 et J2
       PositionZonesTournoiAAfficher[1].y= positionColonne23.y;
       PositionZonesTournoiAAfficher[1].w= positionColonne23.w;
       PositionZonesTournoiAAfficher[1].h= positionColonne23.h;

       PositionZonesTournoiAAfficher[2].x= PositionZonesCliquables[1].x; //position affichage de l'icone J1
       PositionZonesTournoiAAfficher[2].y= PositionZonesCliquables[1].y;
       PositionZonesTournoiAAfficher[2].w= PositionZonesCliquables[1].w;
       PositionZonesTournoiAAfficher[2].h= PositionZonesCliquables[1].h;

       PositionZonesCliquables[ZC_J1type].x= positionColonne23.x+11; //centrée sur les mots "Joueur 1 et 2", affichage du typeJeu J1
       PositionZonesCliquables[ZC_J1type].y= positionColonne23.y+25;
       PositionZonesCliquables[ZC_J1type].w= 40;
       PositionZonesCliquables[ZC_J1type].h= 20;

       PositionZonesCliquables[ZC_J2Icone].x= positionColonne23.x+109; //centrée sur les mots "Joueur 1 et 2", affichage de l'icone J2
       PositionZonesCliquables[ZC_J2Icone].y= positionColonne23.y+5;
       PositionZonesCliquables[ZC_J2Icone].w= 20;
       PositionZonesCliquables[ZC_J2Icone].h=20;

       PositionZonesTournoiAAfficher[3].x= PositionZonesCliquables[3].x; //position affichage de l'icone J2
       PositionZonesTournoiAAfficher[3].y= PositionZonesCliquables[3].y;
       PositionZonesTournoiAAfficher[3].w= PositionZonesCliquables[3].w;
       PositionZonesTournoiAAfficher[3].h= PositionZonesCliquables[3].h;

       PositionZonesCliquables[ZC_J2type].x= positionColonne23.x+99; //Image centrée sur les mots "Joueur 1 et 2", affichage du typeJeu J2
       PositionZonesCliquables[ZC_J2type].y= positionColonne23.y+25;
       PositionZonesCliquables[ZC_J2type].w= 40;
       PositionZonesCliquables[ZC_J2type].h=20;

       PositionZonesCliquables[ZC_Musique].x= positionColonne23.x+65; //position de collage icone de la musique
       PositionZonesCliquables[ZC_Musique].y= positionColonne23.y+5;
       PositionZonesCliquables[ZC_Musique].w= 20;
       PositionZonesCliquables[ZC_Musique].h= 20;

       PositionZonesTournoiAAfficher[4].x= PositionZonesCliquables[5].x; //centré horizontalement sous le mot "Manche" et verticalement sur l'image
       PositionZonesTournoiAAfficher[4].y= PositionZonesCliquables[5].y;
       PositionZonesTournoiAAfficher[4].w= PositionZonesCliquables[5].w;
       PositionZonesTournoiAAfficher[4].h= PositionZonesCliquables[5].h;

       PositionZonesCliquables[ZC_Dim].x= positionTitreColonne[3].x+(positionTitreColonne[3].w-BoutonLargeur)/2; //centré horizontalement sous le mot "Dimension" et verticalement sur l'image, affichage du bouton configDim
       PositionZonesCliquables[ZC_Dim].y= positionColonne23.y;
       PositionZonesCliquables[ZC_Dim].w= BoutonLargeur;
       PositionZonesCliquables[ZC_Dim].h= BoutonHauteur; //fin initialisation de la ligne 1

       PositionZonesTournoiAAfficher[5].x= PositionZonesCliquables[6].x; //centré horizontalement sous le mot "Dimension" et verticalement sur l'image, affichage du bouton configDim
       PositionZonesTournoiAAfficher[5].y= PositionZonesCliquables[6].y;
       PositionZonesTournoiAAfficher[5].w= PositionZonesCliquables[6].w;
       PositionZonesTournoiAAfficher[5].h= PositionZonesCliquables[6].h;

       PositionZonesTournoiAAfficher[6].x= PositionZonesCliquables[6].x+(PositionZonesCliquables[6].w-TexteColonne1->w)/2; //affiche la dimension largeur centrée horizontalement
       PositionZonesTournoiAAfficher[6].y= PositionZonesCliquables[6].y-5;
       PositionZonesTournoiAAfficher[6].w= TexteColonne1->w; //dimension pour les chiffres
       PositionZonesTournoiAAfficher[6].h= TexteColonne1->h;

       PositionZonesTournoiAAfficher[7].x= PositionZonesCliquables[6].x+10; //affiche la dimension hauteur/verticale
       PositionZonesTournoiAAfficher[7].y= PositionZonesCliquables[6].y+(PositionZonesCliquables[6].h-TexteColonne1->h)/2; // affiche la dimension hauteur centré verticalement
       PositionZonesTournoiAAfficher[7].w= TexteColonne1->w;
       PositionZonesTournoiAAfficher[7].h= TexteColonne1->h;

       for (i=1; i<NbLignesTournoi; i++) //pour chaque ligne y compris la derniere ligne (n°10)
       {
           for (j=0; j<NbZonesCliquablesParLigneTournoi; j++) //detail des colonnes
           {
               PositionZonesCliquables[j+NbZonesCliquablesParLigneTournoi*i].x = PositionZonesCliquables[j].x ;
               PositionZonesCliquables[j+NbZonesCliquablesParLigneTournoi*i].y = PositionZonesCliquables[j].y+i*PositionY; //On décalle tout d'une ligne de hauteur PositionY
               PositionZonesCliquables[j+NbZonesCliquablesParLigneTournoi*i].w = PositionZonesCliquables[j].w;
               PositionZonesCliquables[j+NbZonesCliquablesParLigneTournoi*i].h = PositionZonesCliquables[j].h;
           }

           for (j=0; j<NbZonesAffichablesParLigneTournoi; j++) //detail des colonnes
           {
               PositionZonesTournoiAAfficher[j+NbZonesAffichablesParLigneTournoi*i].x= PositionZonesTournoiAAfficher[j].x;
               PositionZonesTournoiAAfficher[j+NbZonesAffichablesParLigneTournoi*i].y= PositionZonesTournoiAAfficher[j].y+i*PositionY;
               PositionZonesTournoiAAfficher[j+NbZonesAffichablesParLigneTournoi*i].w= PositionZonesTournoiAAfficher[j].w;
               PositionZonesTournoiAAfficher[j+NbZonesAffichablesParLigneTournoi*i].h=PositionZonesTournoiAAfficher[j].h;
           }
       }

       //correction de la derniere ligne , 1ere colonne, pour centrer correctement le numero de manche "10"
       sprintf(texte, "%d", 10); //Determine la taille de la sdl_surface pour definir le numero de la manche (1 par exemple)
       TexteColonne1=TTF_RenderText_Blended(police.normale, texte, couleurTexteNoir); //peu importe la couleur, on veut juste la taille

       PositionZonesCliquables[(NbLignesTournoi-1)*NbZonesCliquablesParLigneTournoi].x = positionTitreColonne[0].x+(positionTitreColonne[0].w-TexteColonne1->w)/2; //modification de la 63eme case (10eme ligne)
       PositionZonesCliquables[(NbLignesTournoi-1)*NbZonesCliquablesParLigneTournoi].w = TexteColonne1->w; //changement de la largeur de la zone cliquable car 10 est plus long a ecrire que 1
       PositionZonesTournoiAAfficher[(NbLignesTournoi-1)*NbZonesAffichablesParLigneTournoi].x = PositionZonesCliquables[(NbLignesTournoi-1)*NbZonesCliquablesParLigneTournoi].x; //modification de la 72eme case (10eme ligne)
       PositionZonesTournoiAAfficher[(NbLignesTournoi-1)*NbZonesAffichablesParLigneTournoi].w = PositionZonesCliquables[(NbLignesTournoi-1)*NbZonesCliquablesParLigneTournoi].w;

       PositionZonesCliquables[NbLignesTournoi*NbZonesCliquablesParLigneTournoi+0].x=BoutonX; //bouton valider (zone 70)
       PositionZonesCliquables[NbLignesTournoi*NbZonesCliquablesParLigneTournoi+0].y=BoutonYValider;
       PositionZonesCliquables[NbLignesTournoi*NbZonesCliquablesParLigneTournoi+0].w=BoutonLargeur;
       PositionZonesCliquables[NbLignesTournoi*NbZonesCliquablesParLigneTournoi+0].h=BoutonHauteur;

       PositionZonesCliquables[NbLignesTournoi*NbZonesCliquablesParLigneTournoi+1].x=BoutonX; //bouton quitter (zone 71)
       PositionZonesCliquables[NbLignesTournoi*NbZonesCliquablesParLigneTournoi+1].y=BoutonYQuitter;
       PositionZonesCliquables[NbLignesTournoi*NbZonesCliquablesParLigneTournoi+1].w=BoutonLargeur;
       PositionZonesCliquables[NbLignesTournoi*NbZonesCliquablesParLigneTournoi+1].h=BoutonHauteur; //maintenant, on connait toutes les coordonnées des zones cliquables (modifiable+boutons)

       //Initialisation des PositionZonesTournoiASelectionner pour afficher la bonne portion d'image
       PositionZonesTournoiASelectionner[0].x = Tournoi[0].TypeJeu*BoutonLargeur; //ConfigType : selection du bon typeJeu : HH-OH-HO-OO en noir
       PositionZonesTournoiASelectionner[0].y = 0;
       PositionZonesTournoiASelectionner[0].w = BoutonLargeur; //selection du bon typeJeu
       PositionZonesTournoiASelectionner[0].h = BoutonHauteur;

       PositionZonesTournoiASelectionner[1].x = 874; //selection de la derniere icone, la plus petite sur la 1ere ligne
       PositionZonesTournoiASelectionner[1].y = 0; // sur la 1ere ligne en couleur
       PositionZonesTournoiASelectionner[1].w = 20; //dimension de l'icone pour J1
       PositionZonesTournoiASelectionner[1].h = 20; //dimension de l'icone pour J1

       PositionZonesTournoiASelectionner[2].x = 874; //selection de la derniere icone, la plus petite sur la 1ere ligne
       PositionZonesTournoiASelectionner[2].y = 0; // sur la 1ere ligne en couleur
       PositionZonesTournoiASelectionner[2].w = 20; //dimension de l'icone pour J2
       PositionZonesTournoiASelectionner[2].h = 20; //dimension de l'icone pour J2

       PositionZonesTournoiASelectionner[3].x = 0; //selection de l'icone de musique selon i/MaxMancheTournoi=0 ou 1
       PositionZonesTournoiASelectionner[3].y = 20*Tournoi[0].Musique.Actuel;
       PositionZonesTournoiASelectionner[3].w =20; //selection de l'icone de musique
       PositionZonesTournoiASelectionner[3].h =20; //selection de l'icone de musique

       PositionZonesTournoiASelectionner[4].x = (Tournoi[0].NbJetonsAAligner-3)*BoutonLargeur; //selection de ConfigDim en noir
       PositionZonesTournoiASelectionner[4].y = 0; //selection de ConfigDim en noir, ou gris selon i/MaxMancheTournoi=0 ou 1
       PositionZonesTournoiASelectionner[4].w = BoutonLargeur;
       PositionZonesTournoiASelectionner[4].h = BoutonHauteur;

       for (i=1; i<NbLignesTournoi; i++) //initialisation des autres lignes de selection en noir
       {
           for (j=0; j<5; j++) //detail des colonnes
           {
               PositionZonesTournoiASelectionner[5*i+j].x = PositionZonesTournoiASelectionner[j].x; //selection du bon typeJeu : HH-OH-HO-OO en noir
               PositionZonesTournoiASelectionner[5*i+j].y = PositionZonesTournoiASelectionner[j].y;
               PositionZonesTournoiASelectionner[5*i+j].w = PositionZonesTournoiASelectionner[j].w; //selection du bon typeJeu
               PositionZonesTournoiASelectionner[5*i+j].h = PositionZonesTournoiASelectionner[j].h;
           }
       }

       //modification de ceux en gris
       for (i=MaxMancheTournoi; i<NbLignesTournoi; i++) //initialisation des autres lignes de selection en noir
       {
           PositionZonesTournoiASelectionner[5*i+0].x = 4*BoutonLargeur; //ConfigType
           PositionZonesTournoiASelectionner[5*i+1].y = 20; // icone pour J1
           PositionZonesTournoiASelectionner[5*i+2].y = 20; // icone pour J2
           PositionZonesTournoiASelectionner[5*i+3].x = 20; //icone de musique
           PositionZonesTournoiASelectionner[5*i+4].y = BoutonHauteur; //ConfigDim
       }

       SDL_FreeSurface(TexteColonne1);*/
}

/*********************************************/

void ModifPositionZonesTournoiASelectionner(int AncienMaxMancheTournoi, int NouveauMaxMancheTournoi,
					    SDL_Rect *PositionZonesTournoiASelectionner)
{
    /*
       //au debut de la fonction, [0->AncienMaxMancheTournoi: en noir][AncienMaxMancheTournoi->NouveauMaxMancheTournoi : en gris]
       //si AncienMaxMancheTournoi<NouveauMaxMancheTournoi, il faut transformer du gris en noir
       //si NouveauMaxMancheTournoi<AncienMaxMancheTournoi, il faut transformer du noir en gris
       int i,j;

       if(AncienMaxMancheTournoi<NouveauMaxMancheTournoi)
       {
           for (i=AncienMaxMancheTournoi; i<NouveauMaxMancheTournoi; i++) //initialisation des autres lignes de selection en noir
           {
               for (j=0; j<5; j++) //detail des colonnes
               {
                   PositionZonesTournoiASelectionner[5*i+j].x = PositionZonesTournoiASelectionner[j].x; //selection du bon typeJeu : HH-OH-HO-OO en noir
                   PositionZonesTournoiASelectionner[5*i+j].y = PositionZonesTournoiASelectionner[j].y;
                   PositionZonesTournoiASelectionner[5*i+j].w = PositionZonesTournoiASelectionner[j].w; //selection du bon typeJeu
                   PositionZonesTournoiASelectionner[5*i+j].h = PositionZonesTournoiASelectionner[j].h;
               }
           }
       }
       else//NouveauMaxMancheTournoi=<AncienMaxMancheTournoi
       {
           for (i=NouveauMaxMancheTournoi; i<AncienMaxMancheTournoi; i++) //initialisation des autres lignes de selection en noir
           {
               PositionZonesTournoiASelectionner[5*i+0].x = 4*BoutonLargeur; //ConfigType
               PositionZonesTournoiASelectionner[5*i+1].y = 20; // icone pour J1
               PositionZonesTournoiASelectionner[5*i+2].y = 20; // icone pour J2
               PositionZonesTournoiASelectionner[5*i+3].x = 20; //icone de musique
               PositionZonesTournoiASelectionner[5*i+4].y = BoutonHauteur; //ConfigDim
           }
       }*/
}
/*********************************************/
void AfficheMancheTournoi(t_morp_sdl *morp_sdl, MancheTournoi *Tournoi, int MaxMancheTournoi,
			  Joueur Joueur, DimPlateau DimJeu, SDL_Surface **TitreColonne,
			  SDL_Rect *positionTitreColonne, SDL_Rect *PositionZonesTournoiAAfficher,
			  SDL_Rect *PositionZonesTournoiASelectionner) //a supprimer
{
    /*
       SDL_Rect positionFond= {0, 0, EcranLarg, EcranHaut};
       int i=0;
       SDL_Color couleurTexte=couleurTexteNoir;
       char texte[10];
       SDL_Surface *TexteColonne1Manche[10];
       SDL_Surface *TexteColonne4Largeur[10];
       SDL_Surface *TexteColonne4Hauteur[10];

       for (i=0; i<NbLignesTournoi; i++) //creation des textes a afficher en noir et des boutons
       {
           if(i>=MaxMancheTournoi)
               couleurTexte=couleurTexteGris;

           sprintf(texte, "%d", Tournoi[i].numeroDeManche); //pour afficher le numero de la manche
           TexteColonne1Manche[i]=TTF_RenderText_Blended(police.normale, texte, couleurTexte); //texte en noir

           sprintf(texte, "%d", Tournoi[i].dimN); //pour afficher la dimension : N
           TexteColonne4Largeur[i]=TTF_RenderText_Blended(police.normale, texte, couleurTexte);

           sprintf(texte, "%d", Tournoi[i].dimM); //pour afficher la dimension : M
           TexteColonne4Hauteur[i]=TTF_RenderText_Blended(police.normale, texte, couleurTexte);
       }

       SDL_FillRect(Jeton->Ecran, &positionFond, SDL_MapRGB(Jeton->Ecran->format, coulMarron)); //on colle un fond marron pour effacer le tableau
       AfficheBoutons(police, Jeton, Joueur, DimJeu,Musique, EcranInstruction);

       for (i=0; i<4; i++)
           SDL_BlitSurface(TitreColonne[i], NULL, Jeton->Ecran, &positionTitreColonne[i]); //ecrit la legende de la ligne 1 : mots en noir

       for (i=0; i<10; i++) //affiche chaque ligne EN NOIR, nouvelle version
       {
           //PositionZonesTournoiASelectionner
           SDL_BlitSurface(TexteColonne1Manche[i], NULL, Jeton->Ecran, &PositionZonesTournoiAAfficher[0+8*i]); //ecrit le numero de la manche
           SDL_BlitSurface(Jeton->BoutonConfigType, &PositionZonesTournoiASelectionner[5*i+0], Jeton->Ecran, &PositionZonesTournoiAAfficher[1+8*i]); //colle le bon cadre en fonction de TypeJeu selectionné
           SDL_BlitSurface(Jeton->Pion[Tournoi[i].Joueur1.NumeroPion], &PositionZonesTournoiASelectionner[5*i+1], Jeton->Ecran, &PositionZonesTournoiAAfficher[2+8*i]); //On colle le pointeur1
           SDL_BlitSurface(Jeton->Pion[Tournoi[i].Joueur2.NumeroPion], &PositionZonesTournoiASelectionner[5*i+2], Jeton->Ecran, &PositionZonesTournoiAAfficher[3+8*i]); //On colle le pointeur2
    //  int a =PositionZonesTournoiASelectionner[5*i+3].y;
           SDL_BlitSurface(Jeton->Musique, &PositionZonesTournoiASelectionner[5*i+3], Jeton->Ecran, &PositionZonesTournoiAAfficher[4+8*i]); //On colle l'icone musique
           SDL_BlitSurface(Jeton->BoutonConfigDimension, &PositionZonesTournoiASelectionner[5*i+4], Jeton->Ecran, &PositionZonesTournoiAAfficher[5+8*i]); //affiche le bouton pour redimensionner le plateau de jeu
           SDL_BlitSurface(TexteColonne4Largeur[i], NULL, Jeton->Ecran, &PositionZonesTournoiAAfficher[6+8*i]); //affiche les dimensions de largeur chiffrées
           SDL_BlitSurface(TexteColonne4Hauteur[i], NULL, Jeton->Ecran, &PositionZonesTournoiAAfficher[7+8*i]); //affiche les dimensions de hauteur chiffrées
       }
           SDL_RenderPresent(Renderer);//Actualisation
       //  SDL_Delay(3000); //attend 1000ms = 1 seconde avant de passer a la suite
    */
}
/*********************************************/
/*void CopieStruct(MancheTournoi *AncienTournoi,MancheTournoi *NouveauTournoi) //copie AncienTournoi dans NouveauTournoi
{
  int i=0;

  for(i=0; i<NbMatchMax; i++) //on initilaise les 10 manches possibles
  {
    NouveauTournoi[i].numeroDeManche=AncienTournoi[i].numeroDeManche; //definit le numero
    NouveauTournoi[i].Joueur1.joueur=AncienTournoi[i].Joueur1.joueur; //definit le joueur 1 : H ou O
    NouveauTournoi[i].Joueur1.jeton=AncienTournoi[i].Joueur1.jeton; //definit le joueur 1 : Rond ou Croix
    NouveauTournoi[i].Joueur1.NumeroPion=AncienTournoi[i].Joueur1.NumeroPion;
    NouveauTournoi[i].Joueur2.joueur=AncienTournoi[i].Joueur2.joueur; //definit le joueur 2 : H ou O
    NouveauTournoi[i].Joueur2.jeton=AncienTournoi[i].Joueur2.jeton; //definit le joueur 1 : Rond ou Croix
    NouveauTournoi[i].Joueur2.NumeroPion=AncienTournoi[i].Joueur2.NumeroPion;
    NouveauTournoi[i].NumeroMusique=AncienTournoi[i].NumeroMusique;
    NouveauTournoi[i].TypeJeu=AncienTournoi[i].TypeJeu;
    NouveauTournoi[i].dimM=AncienTournoi[i].dimM; //definit la dimension M
    NouveauTournoi[i].dimN=AncienTournoi[i].dimN; //definit la dimension N
    NouveauTournoi[i].NbJetonsAAligner=AncienTournoi[i].NbJetonsAAligner; //definit le nombre de jetons à aligner
  }
}*/
/*********************************************/
void ModifMancheTournoi(t_morp_sdl *morp_sdl, MancheTournoi *Tournoi,int *MaxMancheTournoi, Joueur Joueur,
			DimPlateau DimJeu, SDL_Surface **TitreColonne,SDL_Rect *positionTitreColonne,
			SDL_Rect *PositionZonesCliquables, SDL_Rect *PositionZonesTournoiAAfficher,
			SDL_Rect *PositionZonesTournoiASelectionner) //a supprimer
{
    /*
       int TournoiValide=NON; //[OUI : on sort de la fonction][OUI : on reste dans la fonction]
       int Modif=NON; //[OUI : on valide les modifications][OUI : on annule les modifications]
       SDL_Event event;
       SDL_Rect clic= {0, 0, EcranLarg, EcranHaut}; //position du clic (x, y) et dimension de l'ecran (w, h)
       int LigneSupposee_Indice=0; //indice de la ligne sur laquelle on pense avoir cliqué avant de chercher la zone exacte  (si clic ligne 7 alors indice=6)
       int LigneSupposee_Affiche=0;//numero de ligne affichee (LigneSupposee_Affiche=LigneSupposee_Indice+1)
       MancheTournoi AncienTournoi[10];
       int zone=-1; //[- 1: hors ZoneCliquable][0,1,2,3,4,5,6,... : numero de la zone ]
       int i=0;
       int AncienMaxMancheTournoi;
       int NouveauMaxMancheTournoi;
       SDL_Rect positionAffichage;
       int a[10];//a supprimer

       memcpy(&AncienTournoi,&Tournoi,sizeof(MancheTournoi)); //On sauvegarde le Tournoi dans AncienTournoi. On va modifier Tournoi

       while (TournoiValide==NON) //on reste dans a boucle
       {
           AfficheMancheTournoi(police,Tournoi,*MaxMancheTournoi,Jeton, Joueur, DimJeu, Musique, TitreColonne,positionTitreColonne,PositionZonesTournoiAAfficher,PositionZonesTournoiASelectionner); //affichage, on passe "*MaxMancheTournoi" car on ne veut pas que ce soit un pointeur

           while(SDL_PollEvent(&event)) //Traiter les évènements
           {
    //pourquoi non actualisation de l'icone musique ?
    //parce que la fonction ne modifie pas l'icone a charger.
    //la fonction modiftournoi ne modifie que la partie grise et noire et pas les icones.

               switch(event.type)
               {
               case SDL_QUIT: //Si on a cliqué sur la croix
                   TournoiValide=OUI;
                   break;
               case SDL_KEYUP: //Si touche du clavier enfoncée
                   switch (event.key.keysym.sym) //selon la touche enfoncée
                   {
                   case SDLK_ESCAPE: // Appui sur la touche Echap, on va arrêter le jeu
                   case SDLK_a: //appui sur la touche a pour quitter
                   {
                       TournoiValide= OUI; //on valide et on sort de la boucle
                       Modif=NON; //en annulant les modifications
                   }
                   break;
                   case SDLK_v: //appui sur la touche v pour valider et quitter
                   {
                       TournoiValide= OUI; //on valide et on sort de la boucle
                       Modif=OUI; //en validant les modifications
                   }
                   break;
                   default : //sinon rien pour eviter message d'alerte lors de la compilation
                       break;
                   }
                   break;
               case SDL_MOUSEBUTTONUP: //Si clic souris
                   clic.x=event.button.x;
                   clic.y=event.button.y;

                   if(PlateauLarg<clic.x) //si clic dans la marge alors a-t-on clique sur un boutonb?
                   {
                       if((BoutonX<clic.x)&&(clic.x<BoutonX+BoutonLargeur)&&(BoutonYValider<clic.y)&&(clic.y<BoutonYValider+BoutonHauteur)) //Si on clique sur le bouton Instruction, alors
                       {
                           TournoiValide= OUI; //on termine la boucle et on sort de la boucle
                           Modif=OUI; //en validant les modifications
                       }
                       if((BoutonX<clic.x)&&(clic.x<BoutonX+BoutonLargeur)&&(BoutonYQuitter<clic.y)&&(clic.y<BoutonYQuitter+BoutonHauteur)) //Si on clique sur le bouton Quitter, alors
                       {
                           TournoiValide= OUI; //on valide et on sort de la boucle
                           Modif=NON; //en annulant les modifications
                       }
                   }
                   else //on a clique dasn le jeu, sur quelle ligne?
                   {
                       LigneSupposee_Indice=clic.y/50-2; //on pense etre sur une certaine ligne du tableau de Tournoi (si clic ligne n°1 alors manche n°0 du tableau). On doit determiner si on est dans une ZoneCliquable précise ou pas
                       LigneSupposee_Affiche=LigneSupposee_Indice+1;
                       if (LigneSupposee_Affiche>*MaxMancheTournoi)//si on clique sur une zone grise alors on active les lignes suivantes
                       {
                           AncienMaxMancheTournoi=*MaxMancheTournoi;//on sauvegarde l'AncienMaxMancheTournoi
                           *MaxMancheTournoi=LigneSupposee_Indice+1;//alors on modifie le MaxMancheTournoi, pour l'afficher en noir
                           NouveauMaxMancheTournoi=*MaxMancheTournoi;//
                           ModifPositionZonesTournoiASelectionner(AncienMaxMancheTournoi,  NouveauMaxMancheTournoi,PositionZonesTournoiASelectionner); //il faut aussi modifier les PositionZonesTournoiASelectionner pour afficher les icones en noir
                       }
                       if ((LigneSupposee_Affiche<*MaxMancheTournoi)&&(PositionZonesCliquables[NbZonesCliquablesParLigneTournoi*LigneSupposee_Indice].x<clic.x)&&(clic.x<PositionZonesCliquables[NbZonesCliquablesParLigneTournoi*LigneSupposee_Indice].x+PositionZonesCliquables[NbZonesCliquablesParLigneTournoi*LigneSupposee_Indice].w)&&(PositionZonesCliquables[NbZonesCliquablesParLigneTournoi*LigneSupposee_Indice].y<clic.y)&&(clic.y<PositionZonesCliquables[NbZonesCliquablesParLigneTournoi*LigneSupposee_Indice].y+PositionZonesCliquables[NbZonesCliquablesParLigneTournoi*LigneSupposee_Indice].h))//si on clique sur un numero de manche en noir, alors on grise toutes les manches de numero superieur
                       {
                           AncienMaxMancheTournoi=*MaxMancheTournoi;//on sauvegarde l'AncienMaxMancheTournoi
                           *MaxMancheTournoi=LigneSupposee_Indice+1;//alors on modifie le MaxMancheTournoi, pour l'afficher en noir
                           NouveauMaxMancheTournoi=*MaxMancheTournoi;//
                           ModifPositionZonesTournoiASelectionner(AncienMaxMancheTournoi,  NouveauMaxMancheTournoi,PositionZonesTournoiASelectionner); //il faut aussi modifier les PositionZonesTournoiASelectionner pour afficher les icones en noir
                       }
                       //A faire
                       zone=-1;//on reinitialise la marge hors ZoneCliquable a chaque clic
                       for (i=0; i<NbZonesCliquablesParLigneTournoi; i++) //on parcourt la ligneSuppose a la recherche de la bonne ZonesCliquables
                           if((PositionZonesCliquables[NbZonesCliquablesParLigneTournoi*LigneSupposee_Indice+i].x<clic.x)&&(clic.x<PositionZonesCliquables[NbZonesCliquablesParLigneTournoi*LigneSupposee_Indice+i].x+PositionZonesCliquables[NbZonesCliquablesParLigneTournoi*LigneSupposee_Indice+i].w)&&(PositionZonesCliquables[NbZonesCliquablesParLigneTournoi*LigneSupposee_Indice+i].y<clic.y)&&(clic.y<PositionZonesCliquables[NbZonesCliquablesParLigneTournoi*LigneSupposee_Indice+i].y+PositionZonesCliquables[NbZonesCliquablesParLigneTournoi*LigneSupposee_Indice+i].h)) //Si on clique sur le bouton Quitter, alors
                           {
                               zone=i;//on determine le numero de la zone  ZC {ZC_Manche=0, ZC_J1Icone=1, ZC_J1type=2, ZC_J2Icone=3, ZC_J2type=4,ZC_Musique=5,ZC_Dim=6}; //pour savoir quelle ZoneCliquable est accessible
                           }

                       switch (zone)
                       {
                       //       case ZC_Manche://rien
                       //           break;
                       //       case ZC_J1Icone :
                       //             ConfigTypePion(Jeton, 0, Joueur);
                       //           break;
                       //       case ZC_J1type :
                       //           ConfigTypeJeu( *TypeJeu, 0, Joueur *Joueur);
                       //           break;
                       //       case ZC_J2Icone :
                       //             ConfigTypePion(Jeton, 1, Joueur);
                       //           break;
                       //       case ZC_J2type:
                       //           ConfigTypeJeu( *TypeJeu, 1, Joueur *Joueur);
                       //           break;
                       case ZC_Musique:
                           positionAffichage.x=PositionZonesTournoiAAfficher[1+8*LigneSupposee_Indice].x;//position du boutoon configType...
                           positionAffichage.y=PositionZonesTournoiAAfficher[1+8*LigneSupposee_Indice].y;
                           ConfigTypeMusique(Jeton,positionAffichage, &(Tournoi[LigneSupposee_Indice].Musique)); //Determine la musique
                           PositionZonesTournoiASelectionner[5*LigneSupposee_Indice+3].y = 20*Tournoi[LigneSupposee_Indice].Musique.Actuel;//modifie l'icone de la musique selectionnee
                           break;
                       //     case ZC_Dim:
                       //         ConfigDimension(police, Joueur, *Jeton, *DimJeu, Musique); //modifie les dimensions et le nombre de jetons a aligner
                       //   break;
                       default:
                           break;
                       }

                       struct MancheTournoi
                       {
                       int numeroDeManche; //entre 1 et 10
                       IdentiteJoueur Joueur1; //H ou O et ROND ou CROIX
                       IdentiteJoueur Joueur2; //H ou O et ROND ou CROIX
                       jeu TypeJeu; // determine le type HH, HO, OH, OO, VIDE
                       int NumeroMusique;
                       int dimN; //largeur
                       int dimM; //hauteur
                       int NbJetonsAAligner; //Nombre de jetons a aligner pour gagner
                       };


                           il faut actualiser les composantes de la manche j1 icone, J1type,J2,..., dim...
                          switch (zone)
                             {
                             case ZC_Manche://rien
                                 break;
                             case ZC_J1Icone :
                                   ConfigTypePion(Jeton, 0, Joueur);
                                 break;
                             case ZC_J1type :
                                 ConfigTypeJeu( *TypeJeu, 0, Joueur *Joueur);
                                 break;
                             case ZC_J2Icone :
                                   ConfigTypePion(Jeton, 1, Joueur);
                                 break;
                             case ZC_J2type:
                                 ConfigTypeJeu( *TypeJeu, 1, Joueur *Joueur);
                                 break;
                             case ZC_Musique:
                              ConfigTypeMusique( *Jeton, *Musique); //Determine la musique
                                 break;
                             case ZC_Dim:
                                 ConfigDimension(police, Joueur, *Jeton, *DimJeu,*Jeu, Musique); //modifie les dimensions et le nombre de jetons a aligner

                                 break;
                             }


    //si ligneSupposee est noire ou grise, il faut modifier differemment
    //une fois la ZoneCliquable connue, il faut lancer la fonction correspondante et sauvegarder les modifications
                   }

                   //Où a-t-on clique? boutonValider, BoutonAnnuler, ZoneCliquable, autre?
                   break;
               default:  //Sinon on ne fait rien
                   break;
               }
           }

           //on sait s'il faut conserver les modifications ou pas
           if(Modif==NON)
               memcpy(&Tournoi,&AncienTournoi,sizeof(MancheTournoi)); //On retablit les données de l'AncienTournoi



       }




       //si modif = oui, on sort
       //si modif=non, copier lancienne structure

    */
}
/*********************************************/

/*void AfficheVainqueur(Images *Jeton, IdentiteJoueur *joueurGagnant) //Affiche la Coupegrande avec O ou X sur le socle selon le joueur gagnant
{
 SDL_Rect PositionCoupe = {zonePlateauLarg, zonePlateauLarg, 0, 0}; //Position centree sur le plateau par rapport à l'ecran
 SDL_Rect PositionGagnant= {0, 0, DimensionCase, DimensionCase}; //Position relative par rapport a la coupe

}*/

/*********************************************/




