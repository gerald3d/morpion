#ifndef DEF_Affichage
#define DEF_Affichage

#include <SDL2/SDL.h>// pour utiliser la sdl
#include <SDL2/SDL_image.h>// pour coller autre que des bmp
#include <SDL2/SDL_ttf.h> // Pour utiliser les polices avec la SDL
#include <SDL2/SDL_mixer.h> // Pour utiliser les sons avec la SDL

#include "logs.h"
#include "Define.h"
#include "morp_sdl.h"
#include "ihm/src/ihm_sdl.h"

void SDL_RenderCopyTransparent(t_morp_sdl *morp_sdl, SDL_Texture * texture,SDL_Rect *selection,SDL_Rect *position);

void AfficheTitre(t_morp_sdl *morp_sdl); //Affiche le titre et les boutons dans la marge
void AfficheRectVide(t_morp_sdl *morp_sdl, SDL_Rect position, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
void AfficheRectPlein(t_morp_sdl *morp_sdl, SDL_Rect position, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

void AfficheBoutons(t_morp_sdl *morp_sdl, Joueur Joueur, DimPlateau DimJeu, EcranBouton EcranBouton);
void AfficheBoutonTexte(t_morp_sdl *morp_sdl, int NumTexte, BUTTON_SDL_TYPE_OF_BUTTON_STATE Etat);
void AfficheBoutonConfigType(t_morp_sdl *morp_sdl, Joueur Joueur, jeu TypeJeu, BUTTON_SDL_TYPE_OF_BUTTON_STATE Etat); //Affichage de l'image du bouton Config selon le type
void AfficheBoutonConfigDim(t_morp_sdl *morp_sdl, DimPlateau DimJeu, BUTTON_SDL_TYPE_OF_BUTTON_STATE Etat); //Affichage de l'image du bouton Config selon le type

void AffichePlateau(t_morp_sdl *morp_sdl, DimPlateau DimJeu);

void AffichePointeur(t_morp_sdl *morp_sdl, Case *plateau, Joueur Joueur, DimPlateau DimJeu, SDL_Rect Souris, CaracteristiqueJeu Jeu); //Affiche le bon pointeur de souris
void AfficheJeu(t_morp_sdl *morp_sdl, Case *plateau, Joueur Joueur, DimPlateau DimJeu, int PartieFinie);


t_ihm_sdl *creation_interface (t_morp_sdl *morp_sdl);
#endif

