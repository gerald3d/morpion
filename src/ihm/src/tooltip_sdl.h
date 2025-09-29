#ifndef __TOOLTIP__
#define __TOOLTIP__

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <SDL2/SDL.h>// pour utiliser la sdl
#include <SDL2/SDL_image.h>// pour coller autre que des bmp
#include <SDL2/SDL_ttf.h> // Pour utiliser les polices avec la SDL

#include "define_sdl.h"

/*! \class t_tooltip_sdl
    \brief Widget d'affichage d'une bulle d'aide pour chaque widget.

    Il est intégré à chaque widget et ne doit pas être manipulé depuis ce dernier.
*/

/*!
 * \private Structure privée.
 * L'utilisateur final ne doit pas accéder à un de ses membres directement.
 */
typedef struct {
  SDL_Renderer *renderer;
  char* text;
  SDL_Texture *texture;
  SDL_Surface *surface;
  TTF_Font *font;
} t_tooltip_sdl;



/*!
 * \brief Constructeur d'un nouvel objet t_ihm_sdl.
 * @param text Texte à insérer. Si NULL alors il n'y aura pas d'affichage d'une bulle d'aide.
 *
 * Le constructeur renvoie NULL en cas d'erreur.
 */
t_tooltip_sdl *tooltip_sdl_new (const char* text);

/*!
 * \brief Destructeur de l'objet t_tooltip_sdl.
 * @param tooltip pointeur de l'adresse de l'objet à détruire.
 *
 * Le pointeur vaut NULL au retour de la fonction.
 */
void tooltip_sdl_free (t_tooltip_sdl **tooltip);

void tooltip_sdl_set_text (t_tooltip_sdl *tooltip, const char *text);

/* Affiche le tooltip dans le rendu SDL.
 * size_button permet de positionner la bulle d'aide au bon endroit */
void tooltip_sdl_update (t_tooltip_sdl *tooltip, SDL_Rect *size_button);














#endif
