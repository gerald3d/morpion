#ifndef __GAME_CONFIG_SDL__
#define __GAME_CONFIG_SDL__

#include "widget_sdl.h"

/*! \class t_game_config_sdl
    \brief Graphique de configuration de la taille du jeu.

    Il permet d'afficher un repère gradué avec deux curseurs réglables.
*/

/*!
 * \private Structure privée.
 * L'utilisateur final ne doit pas accéder à un de ses membres directement.
 */
typedef struct t_game_config_sdl t_game_config_sdl;

/*!
 * \private Structure privée.
 * L'utilisateur final ne doit pas accéder à un de ses membres directement.
 */
struct t_game_config_sdl {
  t_widget_sdl *widget;    // widget parent qui gère les fonctions principales graphiques
  SDL_Surface *image;      // image globale du widget
  SDL_Surface *surface;    // Surface qui contient le curseur rouge
  unsigned char columns;   // nbre de colonnes du plateau de jeu (par défaut 3)
  unsigned char lines;     // nbre de lignes du plateau de jeu (par défaut 3)
};

/*!
 * \brief Constructeur d'un nouvel objet t_game_config_sdl.
 * @param size Position est taille initiale du widget.
 *
 * Le constructeur renvoie NULL en cas d'erreur.
 */
t_game_config_sdl *game_config_sdl_new (SDL_Rect size, t_logs *logs);

/*!
 * \brief Destructeur de l'objet t_game_config_sdl.
 * @param game_config Pointeur de l'adresse de l'objet à détruire.
 *
 * Le pointeur vaut NULL au retour de la fonction.
 */
void game_config_sdl_free (void **game_config);

/*! \private
 * \brief Renvoie le widget parent.
 * @param game_config Le bouton.
 *
 * Si une erreur survient la fonction est sans effet et un message est affiché en console.
 */
t_widget_sdl *game_config_sdl_get_widget (t_game_config_sdl *game_config);


#endif
