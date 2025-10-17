#ifndef __GAME_CONFIG_SDL__
#define __GAME_CONFIG_SDL__

#include "widget_sdl.h"
#include "config_cursor_sdl.h"

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
  t_widget_sdl *widget;               // widget parent qui gère les fonctions principales graphiques
  SDL_Surface *image;                 // image globale du widget
  t_config_cursor_sdl *vert_cursor;   // Curseur vertical à déplacer
  t_config_cursor_sdl *horiz_cursor;  // Curseur horizontal à déplacer
  int columns;                        // nbre de colonnes du plateau de jeu (par défaut 3)
  int lines;                          // nbre de lignes du plateau de jeu (par défaut 3)
  int *corres_position_cases;          // tableau qui renvoie le nombre de cases en fct de la position du curseur
  int *corres_cases_position;          // tableau qui renvoie la position du curseur en fct du nombre de cases
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
 * @param game_config Le widget de configuration.
 *
 * Si une erreur survient la fonction est sans effet et un message est affiché en console.
 */
t_widget_sdl *game_config_sdl_get_widget (t_game_config_sdl *game_config);

/*!
 * \brief Renvoie la taile du plateau de jeu configurée.
 * @param game_config Le widget de configuration.
 * @param horizontal Renvoie la taille horizontale.
 * @param vertical Renvoie la taille verticale.
 *
 * Il est possible de transmettre NULL à l'un ou l'autre des paramétres pour ne récupérer que celui désiré.
 *
 * Si une erreur survient la fonction est sans effet et un message est affiché en console.
 */
void game_config_sdl_get_tab_size (t_game_config_sdl *game_config, int *horizontal, int *vertical);

/*!
 * \brief Fixe la taile du plateau de jeu au moment de l'affichage.
 * @param game_config Le widget de configuration.
 * @param horizontal Taille horizontale.
 * @param vertical Taille verticale.
 *
 * Il est possible de transmettre NULL à l'un ou l'autre des paramétres pour ne saisir que celui désiré. les valeurs doivent être comprises entre [3, 30]
 *
 * Si une erreur survient la fonction est sans effet et un message est affiché en console.
 */
void game_config_sdl_set_tab_size (t_game_config_sdl *game_config, int *horizontal, int *vertical);

#endif
