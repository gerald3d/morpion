#include "morp_sdl.h"

static bool morp_sdl_chargement_police (t_morp_sdl *morp_sdl);
static bool morp_sdl_chargement_ressources (t_morp_sdl *morp_sdl);

/**********************************/
t_morp_sdl*
morp_sdl_init (t_logs *logs)
{
  FILE *descripteur_fichier_standard = logs_descripteur_fichier (logs, LOG_STANDARD);
  FILE *descripteur_fichier_erreur = logs_descripteur_fichier (logs, LOG_ERROR);

  /* Allocation d'une donnée t_morp_sdl */
  t_morp_sdl *morp_sdl = malloc (sizeof (t_morp_sdl));
  if (morp_sdl == NULL) {
    fprintf (descripteur_fichier_erreur, "Erreur d'allocation mémoire dans %s ligne %d\n", __func__, __LINE__-2);
    return NULL;
  }

  /* Initialisation des données */
  morp_sdl->window = NULL;
  morp_sdl->renderer = NULL;
  morp_sdl->police = NULL;

  for (int i = 0; i < NbMusiquesMax; i++)
    morp_sdl->musique.Liste[i] = NULL;
  morp_sdl->musique.Actuel = 0;
  morp_sdl->musique.Ancien = 0;

  morp_sdl->jeton.Icone = NULL;

  for (int i = 0; i < NbImagesMax; i++) {
    morp_sdl->jeton.Image[i] = NULL;
    morp_sdl->jeton.DimensionImage[i].x = 0;
    morp_sdl->jeton.DimensionImage[i].y = 0;
    morp_sdl->jeton.DimensionImage[i].w = 0;
    morp_sdl->jeton.DimensionImage[i].h = 0;
  }

  for (int i = 0; i < NbImagesPionMax; i++) {
    morp_sdl->jeton.Pion[i] = NULL;
    morp_sdl->jeton.DimensionPion[i].x = 0;
    morp_sdl->jeton.DimensionPion[i].y = 0;
    morp_sdl->jeton.DimensionPion[i].w = 0;
    morp_sdl->jeton.DimensionPion[i].h = 0;
  }

  for (int i = 0; i < NbTexteMessageMax; i++) {
    morp_sdl->jeton.TexteMessage[i] = NULL;
    morp_sdl->jeton.DimensionTexteMessage[i].x = 0;
    morp_sdl->jeton.DimensionTexteMessage[i].y = 0;
    morp_sdl->jeton.DimensionTexteMessage[i].w = 0;
    morp_sdl->jeton.DimensionTexteMessage[i].h = 0;
  }

  for (int i = 0; i < NbTexteBoutonMax; i++) {
    morp_sdl->jeton.TexteBouton[i] = NULL;
    morp_sdl->jeton.DimensionTexteBouton[i].x = 0;
    morp_sdl->jeton.DimensionTexteBouton[i].y = 0;
    morp_sdl->jeton.DimensionTexteBouton[i].w = 0;
    morp_sdl->jeton.DimensionTexteBouton[i].h = 0;
  }

  morp_sdl->logs = logs;

  fprintf (descripteur_fichier_standard, "Chargement des windows, renderer, sdl, ttf,... : en cours.\n");

  //si chargement = erreur
  if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO )!=0) {
    //Si l'initialisation de la SDL a échoué, on retourne NULL
    fprintf (descripteur_fichier_erreur,
	     "Problème pour charger SDL_Init_Video et SDL_Init_AUDIO : %s", SDL_GetError());
    fprintf (descripteur_fichier_erreur, "\n%s - %s\n", __DATE__, __TIME__);

    /* Libération mémoire */
    morp_sdl_liberation (&morp_sdl);

    return NULL;
  }

  fprintf (descripteur_fichier_standard, "Chargement de SDL_Init (video et audio) : OK\n");

  //si creation = erreur
  if(SDL_CreateWindowAndRenderer(EcranLarg, EcranHaut, SDL_WINDOW_SHOWN,
				 &morp_sdl->window, &morp_sdl->renderer)!=0)
    {
      //Si l'initialisation de Window et Renderer a échoué, on retourne NULL
      fprintf(descripteur_fichier_erreur,
	      "Problème pour charger SDL_CreateWindowAndRenderer : %s\n", SDL_GetError());
      fprintf(descripteur_fichier_erreur, "\n%s - %s\n", __DATE__, __TIME__);

      /* Libération mémoire */
      morp_sdl_liberation (&morp_sdl);

      return NULL;
    }

  fprintf (descripteur_fichier_standard, "Chargement de Window et Renderer  : OK\n");

  //si initialisation = erreur
  if(TTF_Init()==-1) {
    //Si l'initialisation de la TTF_Init a échoué on retourne NULL
    fprintf(descripteur_fichier_erreur, "Problème pour charger TTF_Init : %s\n", SDL_GetError());
    fprintf(descripteur_fichier_erreur, "\n%s - %s\n", __DATE__, __TIME__);

    /* Libération mémoire */
    morp_sdl_liberation (&morp_sdl);

    return NULL;
  }

  fprintf (descripteur_fichier_standard, "Chargement de TTF_Init  : OK\n");

  //Initialisation SDL_mixer
  if( Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1 ) {
    //Si l'initialisation du mixer audio a échoué, on retourne NULL
    fprintf(descripteur_fichier_erreur, "Ouverture de la musique : echec. %s\n", SDL_GetError());
    fprintf(descripteur_fichier_erreur, "\n%s - %s\n", __DATE__, __TIME__);

    /* Libération mémoire */
    morp_sdl_liberation (&morp_sdl);

    return NULL;
  }

  fprintf (descripteur_fichier_standard, "Ouverture de la musique : OK\n");
  fprintf (descripteur_fichier_standard, "Initialisation SDL : OK\n");
  fprintf (descripteur_fichier_standard, "Chargement des polices : en attente...\n");

  /* Tentative de chargement des polices */
  if (morp_sdl_chargement_police (morp_sdl) == false) {
    /* Libération mémoire */
    morp_sdl_liberation (&morp_sdl);

    return NULL;
  }

  /* Tentative de chargement des ressources */
  if(morp_sdl_chargement_ressources(morp_sdl) == false) { //Si l'initialisation retourne faux
     /* Libération mémoire */
    morp_sdl_liberation (&morp_sdl);

    return NULL;
    }

  fprintf (descripteur_fichier_standard,
	   "Chargement des images, des messages et des musiques : OK\n");
  fprintf (descripteur_fichier_standard,
	   "Détermination des dimensions des images et des messages : OK\n");

  /* Configuration du gestionnaire SDL */
  SDL_SetRenderDrawBlendMode(morp_sdl->renderer, SDL_BLENDMODE_BLEND);//pour activer la transparence, avant SDL_RenderFillRect

  SDL_SetWindowIcon(morp_sdl->window, morp_sdl->jeton.Icone);// Chargement de l'icône, verifiée
  fprintf (descripteur_fichier_standard, "Chargement de l'icône de l'application : OK\n");

  SDL_SetWindowTitle(morp_sdl->window,"Jeu de Morpion");//affiche le titre de la fenetre

  //ignore tous les clics. A reactiver apres le titre avec SDL_EventState(SDL_MOUSEBUTTONUP, SDL_ENABLE);
  SDL_EventState(SDL_MOUSEBUTTONUP, SDL_IGNORE);

  return morp_sdl;
}

void morp_sdl_liberation (t_morp_sdl **morp_sdl) {
  if (morp_sdl == NULL || *morp_sdl == NULL)
    return;

  if ((*morp_sdl)->renderer)
    SDL_DestroyRenderer((*morp_sdl)->renderer);//fermeture renderer, window, sdl

  if ((*morp_sdl)->window)
    SDL_DestroyWindow((*morp_sdl)->window);

  if ((*morp_sdl)->police)
    TTF_CloseFont((*morp_sdl)->police);

  Mix_CloseAudio();// quit SDL_mixer
  TTF_Quit();
  SDL_Quit();

  /* Mise à NULL du pointeur morp_sdl */
  *morp_sdl = NULL;
}

/**********************************/
void morp_sdl_quit_SDL(t_morp_sdl *morp_sdl)
{
  FILE *descripteur_fichier_standard = logs_descripteur_fichier (morp_sdl->logs, LOG_STANDARD);
  FILE *descripteur_fichier_erreur = logs_descripteur_fichier (morp_sdl->logs, LOG_ERROR);

  fprintf (descripteur_fichier_erreur, "\n Au revoir \n");
  fprintf (descripteur_fichier_standard, "\n Au revoir \n");
  fprintf(descripteur_fichier_erreur, "\n%s - %s\n", __DATE__, __TIME__);
  fprintf (descripteur_fichier_standard, "\n%s - %s\n", __DATE__, __TIME__);
  fprintf(descripteur_fichier_erreur, "\n");
}

/**********************************/
static bool
morp_sdl_chargement_police (t_morp_sdl *morp_sdl)
{
  FILE *descripteur_fichier_standard = logs_descripteur_fichier (morp_sdl->logs, LOG_STANDARD);
  FILE *descripteur_fichier_erreur = logs_descripteur_fichier (morp_sdl->logs, LOG_ERROR);

  morp_sdl->police = TTF_OpenFont("./Ressources/times.ttf", 30);
  if(morp_sdl->police == NULL) { //Si l'initialisation de la police normale a échoué,
    fprintf (descripteur_fichier_erreur, "Erreur de chargement de la police : %s\n", SDL_GetError());
    fprintf (descripteur_fichier_erreur, "\n%s - %s\n", __DATE__, __TIME__);//et fermer le prog

    return false;
  }

  /* Choix par défaut du style du texte */
  TTF_SetFontStyle(morp_sdl->police, TTF_STYLE_NORMAL);

  fprintf(descripteur_fichier_standard, "Chargement de la police : OK\n");

  return true;
}

/**********************************/
static SDL_Texture*
ChargementImages(t_morp_sdl *morp_sdl, const char *file, Uint8 r, Uint8 g, Uint8 b)
{
  SDL_Texture *texture = NULL;//Texture à renvoyer
  SDL_Surface *surface = IMG_Load(file);//surface intermédiaire
  FILE *descripteur_fichier_erreur = logs_descripteur_fichier (morp_sdl->logs, LOG_ERROR);
  FILE *descripteur_fichier_standard = logs_descripteur_fichier (morp_sdl->logs, LOG_STANDARD);

  if(surface==NULL) {//si image non chargée
    fprintf(descripteur_fichier_erreur, "Problème lors du chargement de l'image  : %s. Erreur : %s\n",file, SDL_GetError());
    return NULL;
  }  else {//on crée la transparence pour UNE couleur precise de la texture
    fprintf(descripteur_fichier_standard, "Chargement de l'image %s : OK\n",file );

    if(SDL_SetColorKey(surface,SDL_TRUE,SDL_MapRGB(surface->format, r,g,b))!=0) {//couleur transparent SANS Alpha, avec sdl_surface
      fprintf(descripteur_fichier_erreur, "Problème pour charger la transparence d'une couleur de l'image : %s\n", SDL_GetError());
      return NULL;
    } else {//creation de la texture
      texture = SDL_CreateTextureFromSurface(morp_sdl->renderer, surface);
      if(texture == NULL ) {
	fprintf(descripteur_fichier_erreur, "Problème pour transformer la Surface en Texture : %s\n", SDL_GetError());
	return NULL;
      } else {
	SDL_FreeSurface(surface);//liberation de la surface devenue inutile
	return texture;//renvoi de la texture
      }
    }
  }
}

/**********************************/
static void
LiberationImages(Images *Jeton, int NbImagesChargees, t_logs *logs)//Libération des images
{
  int i;
  FILE *descripteur_fichier_standard = logs_descripteur_fichier (logs, LOG_STANDARD);

  SDL_FreeSurface(Jeton->Icone);//liberation de icone (icone est une surface, pas une texture)
  fprintf(descripteur_fichier_standard, "Libération icône : OK\n");//Si NbImagesChargees==0, il faut juste liberer l'icone

  if (NbImagesChargees!=0) {//Si NbImagesChargees!=0, il faut liberer les autres images
    /* char text[20]; */
    for (i=0; i<NbImagesChargees; i++) {
      SDL_DestroyTexture(Jeton->Image[i]);
      fprintf(descripteur_fichier_standard, "Libération image n°%d : OK \n", i);
    }
  }
}

/**********************************/
static void
LiberationPions(Images *Jeton, int NbPionsCharges, t_logs *logs)//Libération des images
{
  int i;
  FILE *descripteur_fichier_standard = logs_descripteur_fichier (logs, LOG_STANDARD);

  if (NbPionsCharges!=0) {//Si NbPionsCharges!=0, il faut liberer les autres pions
    for (i=0; i<NbPionsCharges; i++) {
      SDL_DestroyTexture(Jeton->Pion[i]);
      fprintf(descripteur_fichier_standard, "Libération pion n°%d : OK \n", i);
    }
  }
}

/**********************************/
void
morp_sdl_liberation_ressources(t_morp_sdl *morp_sdl, int NbImagesChargees, int NbPionsCharges,
			       int NbMessagesCharges, int NbMessagesBoutonCharges,
			       int NbMusiqueChargees) //Libération des images+pions
{
  int i;
  FILE *descripteur_fichier_standard = logs_descripteur_fichier (morp_sdl->logs, LOG_STANDARD);

  SDL_FreeSurface(morp_sdl->jeton.Icone);//liberation de icone (icone est une surface, pas une texture)
  fprintf(descripteur_fichier_standard, "Libération icône : OK \n");//Si NbImagesChargees==0, il faut juste liberer l'icone

  if (NbImagesChargees!=0) {//Si NbImagesChargees!=0, il faut liberer les autres images
    for (i=0; i<NbImagesChargees; i++) {
      SDL_DestroyTexture(morp_sdl->jeton.Image[i]);
      fprintf(descripteur_fichier_standard, "Libération image n°%d : OK \n", i);
    }
  }

  if (NbPionsCharges!=0) {//Si NbPionsCharges!=0, il faut liberer les autres pions
    for (i=0; i<NbPionsCharges; i++) {
      SDL_DestroyTexture(morp_sdl->jeton.Pion[i]);
      fprintf(descripteur_fichier_standard, "Libération pion n°%d : OK \n", i);
    }
  }

  if (NbMessagesCharges!=0) {//Si NbMessagesCharges!=0, il faut liberer les autres messages
    for (i=0; i<NbMessagesCharges; i++) {
      SDL_DestroyTexture(morp_sdl->jeton.TexteMessage[i]);
      fprintf(descripteur_fichier_standard, "Libération message n°%d : OK \n", i);
    }
  }

  if (NbMessagesBoutonCharges!=0) {//Si NbMessagesBoutonCharges!=0, il faut liberer les autres texteBouton
    for (i=0; i<NbMessagesBoutonCharges; i++) {
      SDL_DestroyTexture(morp_sdl->jeton.TexteMessage[i]);
      fprintf(descripteur_fichier_standard, "Libération message Bouton n°%d : OK \n", i);
    }
  }

  if (NbMusiqueChargees!=0) {//Si NbMessagesBoutonCharges!=0, il faut liberer les autres musiques
    for (i=0; i<NbMusiqueChargees; i++) {
      Mix_FreeMusic(morp_sdl->musique.Liste[i]); //liberation des musiques
      fprintf(descripteur_fichier_standard, "Libération musique n°%d : OK \n", i);
    }
  }
}

/**********************************/
static SDL_Texture*
ChargementMessages(t_morp_sdl *morp_sdl, const char *file)
{
  SDL_Texture *texture = NULL;//Texture à renvoyer
  FILE *descripteur_fichier_standard = logs_descripteur_fichier (morp_sdl->logs, LOG_STANDARD);
  FILE *descripteur_fichier_erreur = logs_descripteur_fichier (morp_sdl->logs, LOG_ERROR);

  /* Choix du style d'écriture */
  TTF_SetFontStyle(morp_sdl->police, TTF_STYLE_NORMAL);
  /* Écrit le texte directement dans une Texture */
  texture = SDL_CreateTextureFromSurface(morp_sdl->renderer, TTF_RenderText_Blended(morp_sdl->police, file, couleurTexteNoir));

  if(texture == NULL ) {
    fprintf(descripteur_fichier_erreur, "Problème pour charger le texte \" %s \"en texture: %s\n",file, SDL_GetError());
    return NULL;
  }

  fprintf(descripteur_fichier_standard, "Chargement du texte \" %s \": OK\n",file );//ecrire \" pour afficher le guillemet en texte

  return texture;//renvoi de la texture

}

/**********************************/
static SDL_Texture*
ChargementMessagesBouton(t_morp_sdl *morp_sdl, const char *file, int i)
{
  SDL_Texture *texture = NULL;//Texture à renvoyer
  FILE *descripteur_fichier_standard = logs_descripteur_fichier (morp_sdl->logs, LOG_STANDARD);
  FILE *descripteur_fichier_erreur = logs_descripteur_fichier (morp_sdl->logs, LOG_ERROR);

  if(i!= NbTexteBoutonMax) {//revoir la limite
    switch (i%5) {
    case 0 : // noir, normal
      /* Choix du style d'écriture */
      TTF_SetFontStyle(morp_sdl->police, TTF_STYLE_NORMAL);
      texture = SDL_CreateTextureFromSurface(morp_sdl->renderer, TTF_RenderText_Blended(morp_sdl->police, file, couleurTexteNoir));//texte, normal, noir
      break;
    case 1: // noir souligne
      /* Choix du style d'écriture */
      TTF_SetFontStyle(morp_sdl->police, TTF_STYLE_UNDERLINE);
      texture = SDL_CreateTextureFromSurface(morp_sdl->renderer, TTF_RenderText_Blended(morp_sdl->police, file, couleurTexteNoir));//texte, souligné, noir
      break;
    case 2 : // noir normal
      /* Choix du style d'écriture */
      TTF_SetFontStyle(morp_sdl->police, TTF_STYLE_NORMAL);
      texture = SDL_CreateTextureFromSurface(morp_sdl->renderer, TTF_RenderText_Blended(morp_sdl->police, file, couleurTexteNoir));//texte, normal, noir
      break;
    case 3 :// gris souligné
      /* Choix du style d'écriture */
      TTF_SetFontStyle(morp_sdl->police, TTF_STYLE_UNDERLINE);
      texture = SDL_CreateTextureFromSurface(morp_sdl->renderer, TTF_RenderText_Blended(morp_sdl->police, file, couleurTexteGris));//texte, souligné, gris
      break;
    case 4 :// gris normal
      /* Choix du style d'écriture */
      TTF_SetFontStyle(morp_sdl->police, TTF_STYLE_NORMAL);
      texture = SDL_CreateTextureFromSurface(morp_sdl->renderer, TTF_RenderText_Blended(morp_sdl->police, file, couleurTexteGris));//texte, normal, gris
      break;
    }
  }  else { //si dernier message cad  ""
    /* Choix du style d'écriture */
    TTF_SetFontStyle(morp_sdl->police, TTF_STYLE_NORMAL);
    texture = SDL_CreateTextureFromSurface(morp_sdl->renderer, TTF_RenderText_Blended(morp_sdl->police, file, couleurTexteNoir));//texte, noir
  }

  if(texture == NULL ) {
    fprintf(descripteur_fichier_erreur, "Problème pour charger le texte \" %s \" en texture: %s\n",file, SDL_GetError());
    return NULL;
  } else {
    fprintf(descripteur_fichier_standard, "Chargement du texte \" %s \": OK\n",file );//ecrire \" pour afficher le guillemet en texte
    return texture;//renvoi de la texture
  }
}

/**********************************/
//Chargement des images, des textes, des musiques
static bool
morp_sdl_chargement_ressources (t_morp_sdl *morp_sdl) {
  char *ListeImage[] = {"./Ressources/image-BoutonConfigDimension.bmp","./Ressources/image-BoutonConfigType.bmp","./Ressources/image-CaseGrise.bmp","./Ressources/image-CoupeGrande.bmp","./Ressources/image-CoupePetite.bmp","./Ressources/image-Curseur.bmp","./Ressources/image-CurseurAxe.bmp","./Ressources/image-CurseurCroix-Gris.bmp","./Ressources/image-CurseurCroix-Noir.bmp","./Ressources/image-Musique.bmp","./Ressources/image-Titre.bmp"};//liste des fichiers d'images à charger
  char *ListePion[] = {"./Ressources/image-Jeton-0.bmp","./Ressources/image-Jeton-1.bmp","./Ressources/image-Jeton-2.bmp","./Ressources/image-Jeton-3.bmp","./Ressources/image-Jeton-4.bmp","./Ressources/image-Jeton-5.bmp","./Ressources/image-Jeton-6.bmp","./Ressources/image-Jeton-7.bmp","./Ressources/image-Jeton-8.bmp","./Ressources/image-Jeton-9.bmp","./Ressources/image-Jeton-10.bmp","./Ressources/image-Jeton-11.bmp","./Ressources/image-Jeton-12.bmp","./Ressources/image-Jeton-13.bmp","./Ressources/image-Jeton-14.bmp","./Ressources/image-Jeton-15.bmp","./Ressources/image-Jeton-16.bmp","./Ressources/image-Jeton-17.bmp","./Ressources/image-Jeton-18.bmp","./Ressources/image-Jeton-19.bmp","./Ressources/image-Jeton-20.bmp","./Ressources/image-Jeton-21.bmp","./Ressources/image-Jeton-22.bmp","./Ressources/image-Jeton-23.bmp"};//liste des fichiers d'images à charger
  char *ListeTexteMessage[] = {"Pour jouer une partie de morpion classique, ","il suffit d'aligner (verticalement, horizontalement, ","ou diagonalement) trois jetons.","Ce bouton permet d'annuler plusieurs -----------> ","derniers coups joués. -------------------------------> ","Ce bouton permet de jouer contre l'ordinateur,","de choisir son jeton, de modifier la musique. -->","Ce bouton permet de changer le nombre ------->","de cases et le nombre de jetons à aligner. ------->","Ce bouton permet de définir un tournoi de -----> ","10 manches maximum. ---------------------------->","Manche ","Joueur 1", "Joueur 2","Dimensions"};//liste des textes a charger
  char *ListeTexteBouton[] = {"Annuler", "A", "nnuler","A", "nnuler","CoudPouS","C", "oudPouS","C", "oudPouS","Instruction","I", "nstruction","I", "nstruction","Nouveau","N", "ouveau","N", "ouveau","Quitter","Q", "uitter","Q", "uitter","Tournoi","T", "ournoi","T", "ournoi","Valider","V", "alider","V", "alider"," "};//liste des textes a charger
  char *ListeMusique[] = {"./Ressources/Musique-Gremlins.mp3","./Ressources/Musique-HP.mp3","./Ressources/Musique-Mario.mp3","./Ressources/Musique-Pokemon1.mp3","./Ressources/Musique-Pokemon2.mp3","./Ressources/Musique-SW1.mp3","./Ressources/Musique-SW2.mp3"};
  int i;
  FILE *descripteur_fichier_standard = logs_descripteur_fichier (morp_sdl->logs, LOG_STANDARD);
  FILE *descripteur_fichier_erreur = logs_descripteur_fichier (morp_sdl->logs, LOG_ERROR);

  morp_sdl->jeton.Icone=IMG_Load("./Ressources/image-Icone.ico");// chargement de l'icone
  if(morp_sdl->jeton.Icone==NULL) {//si image non chargée
    fprintf(descripteur_fichier_erreur, "Chargement de l'icône : Problème. %s\n", SDL_GetError());
    fprintf(descripteur_fichier_erreur, "\n%s - %s\n", __DATE__, __TIME__);

    return false;
  }

  fprintf(descripteur_fichier_standard, "Chargement de l'icône Icone.ico: OK \n");

  for (i=0; i<NbImagesMax; i++) {// chargement des  images
    morp_sdl->jeton.Image[i]=ChargementImages(morp_sdl, ListeImage[i], couleurImageBlanc);
    if(morp_sdl->jeton.Image[i]==NULL) {//si pb de chargement
      //liberation des images chargées (il y en a i), il ne restera plus que la SDL à libérer
      morp_sdl_liberation_ressources(morp_sdl, i, 0, 0, 0, 0);

      fprintf(descripteur_fichier_erreur, "\n%s - %s\n", __DATE__, __TIME__);

      return false;
    } else
      SDL_QueryTexture(morp_sdl->jeton.Image[i], NULL, NULL, &(morp_sdl->jeton.DimensionImage[i]).w, &(morp_sdl->jeton.DimensionImage[i]).h);//determination dimension de l'image
  }

  fprintf(descripteur_fichier_standard, "Nombre d'images correctement chargées : %d \n", i);//il y en a i images

  for (i=0; i<NbImagesPionMax; i++) {// chargement des pions
    morp_sdl->jeton.Pion[i]=ChargementImages(morp_sdl, ListePion[i], couleurImageBlanc);
    if(morp_sdl->jeton.Pion[i]==NULL) {//si pb de chargement
      morp_sdl_liberation_ressources(morp_sdl,NbImagesMax, i,0,0,0);//liberation des images chargées (il y en a i), il ne restera plus que la SDL à libérer
      fprintf(descripteur_fichier_erreur, "\n%s - %s\n", __DATE__, __TIME__);

      return false;
    } else
      SDL_QueryTexture(morp_sdl->jeton.Pion[i], NULL, NULL, &(morp_sdl->jeton.DimensionPion[i]).w, &(morp_sdl->jeton.DimensionPion[i]).h);//determination dimension de l'image
  }

  fprintf(descripteur_fichier_standard, "Nombre de pions correctement chargés : %d \n", i);//il y en a i pions

  for (i=0; i<NbTexteMessageMax; i++) {// chargement des TexteMessages
    morp_sdl->jeton.TexteMessage[i]=ChargementMessages(morp_sdl,ListeTexteMessage[i]);
    if(morp_sdl->jeton.TexteMessage[i]==NULL) {//si pb de chargement
      morp_sdl_liberation_ressources(morp_sdl,NbImagesMax, NbImagesPionMax,i,0, 0);//liberation des images chargées (il y en a i), il ne restera plus que la SDL à libérer
      fprintf(descripteur_fichier_erreur, "\n%s - %s\n", __DATE__, __TIME__);

      return false;
    }
    else
      SDL_QueryTexture(morp_sdl->jeton.TexteMessage[i], NULL, NULL, &(morp_sdl->jeton.DimensionTexteMessage[i]).w, &(morp_sdl->jeton.DimensionTexteMessage[i]).h);
  }

  fprintf(descripteur_fichier_standard, "Nombre de messages Texte correctement chargés : %d \n", i);//il y en a i messages

  for (i=0; i<NbTexteBoutonMax; i++) {// chargement des textes Bouton
    morp_sdl->jeton.TexteBouton[i]=ChargementMessagesBouton(morp_sdl,ListeTexteBouton[i],i);
    if(morp_sdl->jeton.TexteBouton[i]==NULL) {//si pb de chargement

      morp_sdl_liberation_ressources(morp_sdl, NbImagesMax, NbImagesPionMax,NbTexteMessageMax,i,0);//liberation des images chargées (il y en a i), il ne restera plus que la SDL à libérer

      fprintf(descripteur_fichier_erreur, "\n%s - %s\n", __DATE__, __TIME__);

      return false;
    } else
      SDL_QueryTexture(morp_sdl->jeton.TexteBouton[i], NULL, NULL, &(morp_sdl->jeton.DimensionTexteBouton[i]).w, &(morp_sdl->jeton.DimensionTexteBouton[i]).h);
  }

  fprintf(descripteur_fichier_standard, "Nombre de messages correctement chargés : %d \n", i);//il y en a i messages

  morp_sdl->musique.Liste[0]=NULL;//La musique[0] est nulle
  for (i=1; i<NbMusiquesMax; i++) {// chargement des autres musiques
    morp_sdl->musique.Liste[i]=Mix_LoadMUS(ListeMusique[i-1]);
    if(morp_sdl->musique.Liste[i]==NULL) {//si pb de chargement

      morp_sdl_liberation_ressources(morp_sdl, NbImagesMax, NbImagesPionMax,NbTexteMessageMax,NbTexteBoutonMax, i);

      fprintf(descripteur_fichier_erreur, "\n%s - %s\n", __DATE__, __TIME__);

      return false;
    } else {
      fprintf(descripteur_fichier_standard, "Chargement de la musique \" %s \": OK\n",ListeMusique[i-1] );//ecrire \" pour afficher le guillemet en texte
    }
  }

  fprintf(descripteur_fichier_standard, "Nombre de musiques correctement chargées : %d \n", i);//il y en a i messages

  return true;
}
/**********************************/

void videJeu(Case *plateau, CaracteristiqueJeu *Jeu, DimPlateau DimJeu) //Fonction pour vider le plateau de jeu
{
  int i;

  DimJeu.TypeJeu=HH; //HH, HO, OH, OO; puis rajouter N si 3 jetons ou plus
  for(i=0; i<DimJeu.N*DimJeu.M; i++) {//On vide le plateau
    plateau[i] = Vide; //toutes les cases sont vides
    Jeu->HistoriqueJeton[i] = -1; //on reinitialise le tableau a -1, pour signaler qu'il est vide
    Jeu->ListeCasesVides[i]=i; //on liste toutes les cases vides du tableau, c'est-a-dire toutes les cases
    Jeu->tabMeilleuresCases[i]=-1; //on reinitialise le tableau a -1, pour signaler qu'il est vide
  }

  Jeu->NumeroJEnCours=0;
  Jeu->NbCasesJouees = 0;
  Jeu->NbMeilleuresCases = 0;
  Jeu->PartieFinie = NON; //On commence une nouvelle partie
}
/*********************************************/


