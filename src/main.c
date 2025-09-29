#include "Main.h"

//dans cette version, on remplace int numeromusiqeue de tournoi par music musique

/*
  Quels que soient les pions choisis pour jouer, on considére qu'ils s'appellent toujours Rond pour J1 et Croix pour J2
  ou les remplacer par motif0 et motif1?

  Reste a faire :

  0.fait. remplacer par des structures

  00.fait. remplacer les 4-5 boutons config par un seul

  1.fait. modifier la fonction ordi joue et ia pour l'adapter a une fonction coup de pouce

  2.fait. introduire la fonction coup de pouce

  3.fait. Integrer le son

  4. fait. bouton tournoi :
  0. fait. rajouter l'icone musique
  a. fait. afficher la ligne de titre
  b. fait. afficher les lignes suivantes
  c. modification et zones de clic, definir des zones ?

  5. créer code erreur : 10001 : pour chaque chargement. Puis boucle : si erreur = 00000 (pas erreur) alors jeu, sinon (chaque est une erreur)
  selon chiffre 1 ou 0 liberation de tel ou tel tableau ou autre
  variable glogable erreur ??

  6. Etudier les symetries et rotations pour limiter les recherches?

  7. Animation des jetons en ajoutant de nouvelles lignes dans les images et un chrono pour els faire defiler

  8. augmenter le nombre de joueurs?

  9. animation de fin de partie ?
  arond-croix-triangle-carré : rien
  grelins ?
  pokemon ; pokebal de differentes couleurs


  voldemort : baguette de haut en bas lancant des sorts
  harry potter : vif dor s'envolant d'un coin
  edwige :  de dgauche a droite laissant tomber des lettres

  star wars : Xwing? Sabre laser ?

  mario : tortue rouge de haut en bas lancant des bananes



  On peut remplacer tous les "ecran" par SDL_GetVideoSurface et éviter de passer "ecran" en parametre.

*/

//on doit fournir "&Jeton" et modifier "*Jeton" pour modifier le pointeur dans la fonction

//Variables Globales
//IdentiteJoueur ListeJoueurs[2]= {{H, Rond, 0}, {H, Croix, 1}}; //2 joueurs humains pour commencer puis on changera en appuyant sur le bouton Config
//Mix_Music *TabMusique[NbMusiques]= {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL}; //Null,Gremlins,Hp,Mario,Poke1,Poke2,SW1,SW2





int main(int argc, char **argv)
{
/* Rédirection du canal stderr dans error_logs.txt */
//freopen("error_logs.txt","w",stderr);
/* Rédirection du canal stdout dans standard_logs.txt */
//freopen("standard_logs.txt","w",stdout);

  /* Initialisation des logs */
  t_logs *logs = logs_init (NULL, NULL);
  if (logs == NULL)
    exit(EXIT_FAILURE);

  /* Tentative d'initialisation de la SDL.
  * Cette initialisation englobe tous les chargements : musiques, images, polices, ressources */
  t_morp_sdl *morp_sdl = NULL;
  if ((morp_sdl = morp_sdl_init (logs)) == NULL) {
    logs_liberation (logs);
    exit (EXIT_FAILURE);
  }

  SDL_Event event; //pour gérer les évenements souris et clavier


  int i;
  int JeuFini = NON; //[NON : on continue][OUI : on arrête le jeu], pour la boucle de jeu
  DimPlateau DimJeu= (DimPlateau){3, 3, zonePlateauLarg/maximumEntre(DimJeu.N, DimJeu.M), 3,HH}; //[N, M, DimCase, NbJetonsAAligner, Musique,typeJeu]
  DimPlateau AncienDimJeu=DimJeu;
  SDL_Rect positionSouris= {0, 0, /*20,20*/DimJeu.DimCase*DimJeu.N, DimJeu.DimCase*DimJeu.M}; //position du clic (x, y) et dimension de l'ecran (w, h)
  Case *plateau=FonctionAllocation1D(DimJeu.N*DimJeu.M, sizeof(Case), logs);

  CaracteristiqueJeu Jeu; //[ NbCasesJouees;, *HistoriqueJeton, *ListeCasesVides, taille, *tabMeilleuresCases, NbMeilleuresCases]
  Jeu.NumeroJEnCours=0;
  Jeu.NbCasesJouees=0;
  Jeu.taille=DimJeu.N*DimJeu.M;
  Jeu.HistoriqueJeton=FonctionAllocation1D(Jeu.taille, sizeof(*Jeu.HistoriqueJeton), logs);
  Jeu.ListeCasesVides=FonctionAllocation1D(5*Jeu.taille, sizeof(*Jeu.ListeCasesVides), logs); //5 * taille car contient tes les ceintures a la suite
  Jeu.tabMeilleuresCases=FonctionAllocation1D(Jeu.taille, sizeof(*Jeu.tabMeilleuresCases), logs);
  Jeu.NbMeilleuresCases=0;

  int NbAlignements=0; //Compte le nombre d'alignements gagnants

  Joueur Joueur= {{{H, Rond, 0}, {H, Croix, 1}}}; //Liste des joueur et numero courant=0
  //IdentiteJoueur joueurEnCours=Joueur.ListeJ[0]; //il faut trouver un moyen de modifier la liste des joueurs lorsque l'on modifie la config



  // int NumMatch=0; //indique le numero du match que l'on est en train de jouer
  //char numMatch[2]=""; //pour afficher les numéros du texte en chaine de caracteres
  zone ZoneEvenement = MARGE;
  int TournoiInitialise = NON; //pour savoir si les positions de collage ont été déja calculées
  MancheTournoi Tournoi[10]; //tableau de 10 manches maximum
  int MaxMancheTournoi=4; //valeur fixée au départ pour faire comprendre que l'on peut aller jusqu'a 10
  SDL_Surface *TitreColonne[4]= {NULL,NULL,NULL,NULL}; //titre des colonnes du mode tournoi
  SDL_Rect positionTitreColonne[4];
  SDL_Rect PositionZonesCliquables[72]; //liste des zones cliquables du mode tournoi : il y a 10*7+3=73 zones cliquables (10 lignes de 7 zones (n°, icone J1, type J1, icone J2, type J2, musique, dim), les boutons valider et quitter, et une zone null)
  SDL_Rect PositionZonesTournoiAAfficher[80]; //liste des zones où l'on va afficher les elements du mode tournoi : il y a 10*9=90 zones d'affichage (10 lignes de 9 zones (n°,BtonConfigType, icone J1, icone J2, musique,Bton ConfigDIm, dimHoriz, dimVert))
  SDL_Rect PositionZonesTournoiASelectionner[50]; //liste des zonesASelectionner du mode tournoi : il y a 10*5=50 zones affichables (10 lignes de 5 zones (zoneColonne23ConfigTypeSelectionnee, zoneColonne23ZoneIcone1Selectionnee, zoneColonne23ZoneIcone2Selectionnee, zoneColonne23MusiqueSelectionnee, zoneColonne4ConfigDimSelectionnee))
  int TournoiLance=NON; //[OUI : mode tournoi lance, il faut initialiser en fonction de la bonne manche][NON : mode normal]

  /**********************/ //debut du programme

  /* Création de l'ihm */
  t_ihm_sdl *ihm_sdl = creation_interface (morp_sdl);
  if (ihm_sdl == NULL) {
    morp_sdl_liberation_ressources(morp_sdl, NbImagesMax, NbImagesPionMax,
				   NbTexteMessageMax, NbTexteBoutonMax, NbMusiquesMax);
    morp_sdl_quit_SDL(morp_sdl);
    morp_sdl_liberation (&morp_sdl);
    logs_liberation (logs);
    exit (EXIT_FAILURE);
  }


  AfficheTitre(morp_sdl); //Affiche le titre du jeu sur fond marron
  AfficheBoutons(morp_sdl, Joueur, DimJeu, EcranJeu);

  SDL_RenderPresent(morp_sdl->renderer);//Actualisation
  SDL_Delay(1000); //attend 1000ms = 1 seconde avant de passer a la suite

  srand(time(NULL)*1000); //

  while(JeuFini==NON) //Maintenant que l'initialisation est valide, on entre dans la boucle de jeu
    {

      videJeu(plateau, &Jeu, DimJeu); //Vide le plateau de jeu
      AfficheJeu(morp_sdl, plateau, Joueur, DimJeu, Jeu.PartieFinie); //affiche le jeu vide
      /* AfficheBoutons(morp_sdl, Joueur, DimJeu, EcranJeu); */

      SDL_EventState(SDL_MOUSEBUTTONUP, SDL_ENABLE); //reactive les clics

      while(Jeu.PartieFinie==NON)
        {
	  //          Joueur.NumeroJEnCours=(Jeu.NbCasesJouees)%2;//juste le numero 0 ou 1
	  if(Joueur.ListeJ[Jeu.NumeroJEnCours].joueur==O) //Si c'est a l'ordi de jouerr
            {
	      SDL_Delay(500);
	      //              DefinirZoneEtendue(Jeu, DimJeu, &InfoZoneDeRecherche); //determine la zone a etudier (InfoZoneDeRecherche)
	      //              DefinirListeCaseVide(&Jeu, DimJeu, InfoZoneDeRecherche); //Détermine le Jeu.TableauCaseVide
	      OrdiJoue(plateau, Joueur, &Jeu, DimJeu, logs);
	      if(Jeu.NbCasesJouees>=2*DimJeu.NbJetonsAAligner-1) //on verifie s'il y a alignement au bout du 5eme jeton rond posé, (Jouées = 3*o+2*x)
                {
		  verifFini(plateau, Joueur, DimJeu, &Jeu, &NbAlignements); //Fonction pour vérifier si la partie est terminée
                }
            }

	  while(SDL_PollEvent(&event)) //Traiter les évènements
            {
	      /* Effacement de la fenêtre */
	      SDL_SetRenderDrawColor(morp_sdl->renderer, 187, 122, 87, 255);
	      SDL_RenderClear (morp_sdl->renderer);

	      /* Transmission des évènements à l'ihm */
	      ihm_sdl_set_events (ihm_sdl, &event);

	      switch(event.type)
                {
                case SDL_QUIT: //Si on a cliqué sur la croix
		  ZoneEvenement=QUITTER;
		  break;
                case SDL_KEYUP: //Si touche du clavier enfoncée
		  switch (event.key.keysym.sym) //selon la touche enfoncée
                    {
                    case SDLK_ESCAPE: // Appui sur la touche Echap, on va arrêter le jeu
                    case SDLK_a: // Appui sur la touche q (Attention a<->q avec les claviers qwerty-azerty), on va arrêter le jeu
		      ZoneEvenement=QUITTER;
		      break;
                    case SDLK_q: // Appui sur la touche a (Attention a<->q avec les claviers qwerty-azerty), on va annuler le dernier coup
		      ZoneEvenement=ANNULER;
		      break;
                    case SDLK_n: //si on presse le n de nouveau
		      ZoneEvenement=NOUVEAU;
		      break;
                    case SDLK_c: // Appui sur la touche a (Attention a<->q avec les claviers qwerty-azerty), on va annuler le dernier coup
		      ZoneEvenement=COUPDEPOUCE;
		      break;
                    case SDLK_t: //si on presse le t de Tournoi
		      ZoneEvenement=TOURNOI;
		      break;
                    /* case SDLK_i: //si on presse le i de Instruction */
		    /*   ZoneEvenement=INSTRUCTION; */
		    /*   break; */
                    case SDLK_v: //si on presse le v de Valider
                    case SDLK_RETURN: //si on presse Return
		      ZoneEvenement=VALIDER;
		      break;
                    case SDLK_d: //si on presse le d de configDim
		      ZoneEvenement=CONFIGDIMENSION;
		      break;
                    default : //sinon rien pour eviter message d'alerte lors de la compilation
		      break;
                    }
		  break;
                case SDL_MOUSEMOTION: //Si souris déplacée
		  positionSouris.x=event.motion.x; //position souris
		  positionSouris.y=event.motion.y;
		  ZoneEvenement=MARGE;
		  break;
                case SDL_MOUSEBUTTONUP: //Si clic souris
		  positionSouris.x=event.button.x; //position clic
		  positionSouris.y=event.button.y;
		  ZoneEvenement = QuelleZoneCliquee(DimJeu,positionSouris, Joueur,Jeu);
		  break;
                default: //Sinon on ne fait rien
		  break;
                }

	      switch(ZoneEvenement)
                {
                case JEU :
		  HumainJoue(plateau, Joueur, &Jeu, DimJeu, positionSouris); //gestion du clic souris
		  if((Jeu.NbCasesJouees)>=2*DimJeu.NbJetonsAAligner-1) //on verifie s'il y a alignement au bout du 5eme jeton rond posé, (Jouées = 3*o+2*x)
		    verifFini(plateau, Joueur, DimJeu, &Jeu, &NbAlignements); //Fonction pour vérifier si la partie est terminée
		  break;
                case NOUVEAU:
		  videJeu(plateau, &Jeu, DimJeu);
		  Jeu.PartieFinie=OUI; //On met PartieFinie a OUI pour quitter la boucle secondaire*/
		  break;
                case ANNULER:
		  if(Joueur.ListeJ[(Jeu.NbCasesJouees-1)%2].joueur==O) //si le joueur precedent est l'ordi, alors il faut annuler 2 jetons
		    AnnulerCoup(plateau, &Jeu); //on annule le coup obligatoire
		  AnnulerCoup(plateau, &Jeu); //on annule le coup obligatoire
		  break;
                case COUPDEPOUCE:
		  if(Joueur.ListeJ[Jeu.NumeroJEnCours].joueur==H) //Le COUPDEPOUCE n'est valable que si je joueur est Humain et pas Ordi
                    {
		      //                     DefinirZoneEtendue(Jeu, DimJeu, &InfoZoneDeRecherche); //determine la zone a etudier (InfoZoneDeRecherche)
		      //                     DefinirListeCaseVide(&Jeu, DimJeu, InfoZoneDeRecherche); //Détermine le Jeu.TableauCaseVide
		      /**/ //                       ia(plateau, Joueur, &Jeu, DimJeu);
		      for (i=0; i<Jeu.NbMeilleuresCases; i++)
			plateau[Jeu.tabMeilleuresCases[i]]=CoupDePouce; //indique les meilleures cases
                    }
		  break;
                case CONFIGTYPE:
		  ConfigType(morp_sdl, &DimJeu.TypeJeu, &Joueur, positionSouris.x-BoutonX, positionSouris.y-BoutonYConfigType); //Les coordonnéees envoyées sont relatives au coin superieur gauche du bouton Config
		  JouerMusique(morp_sdl->musique);
		  break;
                case CONFIGDIMENSION:
		  memcpy(&AncienDimJeu,&DimJeu,sizeof(*plateau));//copie dans AncienDimJeu des données de DimJeu
		  ConfigDimension(morp_sdl, Joueur, &DimJeu,Jeu); //on ouvre la configuration pour determiner le nombre de lignes et de colonnes du plateau de jeu et le nombre de jetons a aligner pour gagner
		  if((AncienDimJeu.N !=DimJeu.N)||(AncienDimJeu.M!=DimJeu.M)) //si on modifie les dimensions du plateau de jeu alors on vide redimensionne et vide
		    ModificationConfigDimension(AncienDimJeu, DimJeu, &positionSouris, &plateau, &Jeu, logs);

		  if(AncienDimJeu.NbJetonsAAligner!=DimJeu.NbJetonsAAligner) //si on modifie juste le NbJetonsAAligner alors vide seulement le plateu
		    videJeu(plateau, &Jeu, DimJeu);
		  break;
                case TOURNOI:

		  if(TournoiInitialise==NON) //si non initialise, alors on lance la fonction de calcul des positions de collage et de clic
                    {
		      initTournoi(morp_sdl, Tournoi, MaxMancheTournoi,TitreColonne, positionTitreColonne,PositionZonesCliquables,PositionZonesTournoiAAfficher,PositionZonesTournoiASelectionner);
		      TournoiInitialise=OUI; //On valide l'initialisation}
                    }

		  //     AfficheMancheTournoi(police,Tournoi,MaxMancheTournoi,&Jeton, DimJeu, TitreColonne,positionTitreColonne,PositionZonesCliquables,PositionZonesTournoiAAfficher,PositionZonesTournoiASelectionner); //affichage
		  ModifMancheTournoi(morp_sdl, Tournoi, &MaxMancheTournoi, Joueur, DimJeu, TitreColonne,positionTitreColonne,PositionZonesCliquables, PositionZonesTournoiAAfficher,PositionZonesTournoiASelectionner);
		  break;
                case INSTRUCTION:
		  Instruction(morp_sdl, Joueur, Jeu, DimJeu); //on ouvre le menu d'Instruction
		  break;
                case QUITTER:
		  JeuFini=OUI; //On met JeuFini a OUI pour quitter la 1ere boucle principale
		  Jeu.PartieFinie=OUI; //On met PartieFinie a OUI pour quitter la 2eme boucle principale
		  ZoneEvenement=MARGE;
		  break;
                case MARGE:
		  break;
                default: //Sinon on ne fait rien
		  break;
                }
            }

	  AfficheJeu(morp_sdl, plateau, Joueur, DimJeu, Jeu.PartieFinie);
	  AfficheBoutons(morp_sdl, Joueur, DimJeu, EcranJeu);
	  AffichePointeur(morp_sdl, plateau, Joueur, DimJeu, positionSouris, Jeu); //pour afficher le pointeur quand on clique et que l'on ne bouge pas
	  ZoneEvenement=MARGE;

	  /* Mise à jour graphique de l'ihm */
	  ihm_sdl_renderer_update (ihm_sdl);

	  SDL_RenderPresent(morp_sdl->renderer); //Rafraichissement de l'écran. Il faut le sortir du while car sinon il attend de bouger la souris pour faire jouer le joueur suivant
        }
      SDL_Delay(1000); //Attendre avant de fermer ou de commencer une nouvelle partie : 1000 ms = 1 s
    }

  for (i=0; i<4; i++) //liberation des mots des colonnes du mode tournoi
    {
      free(TitreColonne[i]);
      TitreColonne[i]=NULL;
    }
  free(plateau); //liberation du plateau de jeu
  plateau=NULL;
  free(Jeu.HistoriqueJeton);
  Jeu.HistoriqueJeton=NULL;
  free(Jeu.ListeCasesVides);
  Jeu.ListeCasesVides=NULL;
  /*  free(Jeu.ListeCasesVides_C1);
      Jeu.ListeCasesVides_C1=NULL;
      free(Jeu.ListeCasesVides_C2);
      Jeu.ListeCasesVides_C2=NULL;
      free(Jeu.ListeCasesVides_C3);
      Jeu.ListeCasesVides_C3=NULL;
      free(Jeu.ListeCasesVides_C4);
      Jeu.ListeCasesVides_C4=NULL;*/

  morp_sdl_liberation_ressources(morp_sdl, NbImagesMax, NbImagesPionMax,
				 NbTexteMessageMax, NbTexteBoutonMax, NbMusiquesMax);

  morp_sdl_quit_SDL(morp_sdl);

  morp_sdl_liberation (&morp_sdl);

  logs_liberation (logs);

  ihm_sdl_free (&ihm_sdl);

  return EXIT_SUCCESS; //on quitte le jeu avec succes
}
