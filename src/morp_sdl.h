#ifndef __MORP_SDL__
#define __MORP_SDL__

#include "Define.h"
#include <SDL2/SDL.h>// pour utiliser la sdl
#include <SDL2/SDL_image.h>// pour coller autre que des bmp
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include "ihm/src/logs.h"

typedef struct
{
    SDL_Surface *Icone;
    SDL_Texture *Image[NbImagesMax];//tableau de NbImagesMax images
    SDL_Texture *Pion[NbImagesPionMax];//tableau de tous les pions
    SDL_Texture *TexteMessage[NbTexteMessageMax];//tableau de NbMessagesMax messages
    SDL_Texture *TexteBouton[NbTexteBoutonMax];//tableau de NbMessagesMax messages
    SDL_Rect DimensionImage[NbImagesMax]; //pour chaque image, dimensionner w et h
    SDL_Rect DimensionPion[NbImagesPionMax];
    SDL_Rect DimensionTexteMessage[NbTexteMessageMax]; //pour chaque texte, dimensionner w et h
    SDL_Rect DimensionTexteBouton[NbTexteBoutonMax]; //pour chaque texte, dimensionner w et h
} Images;

typedef struct {
  SDL_Window *window;
  SDL_Renderer *renderer;
  TTF_Font *police;
  Musique musique;
  Images jeton;
  t_logs *logs;
} t_morp_sdl;

/* Initialise tous les pointeurs nécessaires à la gestion SDL.
 * Renvoie NULL si une erreur est survenue. */
t_morp_sdl *morp_sdl_init (t_logs *logs);

/* Désalloue tous les pointeurs initialisés. Ferme proprement le gestionnaide SDL.
 * Le pointeur morp_sdl vaut NULL en retour. */
void morp_sdl_liberation (t_morp_sdl **morp_sdl);

/* Inscrit les derniers messages dans les logs */
void morp_sdl_quit_SDL(t_morp_sdl *sdl);

//Libération des images+pion
void morp_sdl_liberation_ressources(t_morp_sdl *morp_sdl, int NbImagesChargees, int NbPionsCharges,
				    int NbMessagesCharges , int NbMessagesBoutonCharges, int NbMusiqueChargees);

void videJeu(Case *plateau, CaracteristiqueJeu *Jeu, DimPlateau DimJeu); //Fonction pour vider le plateau de jeu

#endif
