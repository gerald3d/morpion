#ifndef __IHM_SDL__
#define __IHM_SDL__

#include <stdbool.h>
#include <SDL2/SDL.h>// pour utiliser la sdl
#include <SDL2/SDL_image.h>// pour coller autre que des bmp
#include <SDL2/SDL_ttf.h> // Pour utiliser les polices avec la SDL
#include <SDL2/SDL_mixer.h> // Pour utiliser les sons avec la SDL

#include "../../logs.h"
#include "widget_sdl.h"
#include "tooltip_sdl.h"
#include "button_sdl.h"
#include "state_button_sdl.h"
#include "toggle_button_sdl.h"
#include "dialog_sdl.h"
#include "game_config_sdl.h"

#include "case_sdl.h"

/*! \class t_ihm_sdl
    \brief Cet objet contient tous les widgets qui doivent être affichés.

    Il intégre le renderer de la fenêtre SDL ainsi que les évènements qu'il transmet à tous les widgets.
*/

/*!
 * \private Structure privée.
 * L'utilisateur final ne doit pas accéder à un de ses membres directement.
 */
typedef struct {
  SDL_Renderer *renderer;
  t_logs *logs;
  FILE *file_error;                      // Descripteur du fichier des erreurs
  t_liste *widget_list;             // Liste de tous les widgets contenus dans l'interface
  t_liste *insensible_widgets_list; // Liste des widgets désactivés lorsque l'un d'entre-eux et modale
} t_ihm_sdl;


/*!
 * \brief Constructeur d'un nouvel objet t_ihm_sdl.
 * @param renderer Le renderer de la fenêtre SDL.
 *
 * Le constructeur renvoie NULL en cas d'erreur.
 */
t_ihm_sdl *ihm_sdl_new (SDL_Renderer *renderer, t_logs *logs);

/*!
 * \brief Destructeur de l'objet t_ihm_sdl.
 * @param ihm_sdl pointeur de l'adresse de l'objet à détruire.
 *
 * Le pointeur vaut NULL au retour de la fonction.
 */
void ihm_sdl_free (t_ihm_sdl **ihm_sdl);

/*!
 * \brief Renvoie le renderer dela neêtre SDL.
 * @param ihm_sdl Pointeur de l'ihm.
 *
 * Si une erreur survient la fonction renvoie NULL et un message est affiché en console.
 */
SDL_Renderer *ihm_sdl_get_renderer (t_ihm_sdl *ihm_sdl);

/*!
 * \brief Ajoute un widget à l'ihm.
 * @param ihm_sdl Pointeur de l'ihm.
 * @param widget Widget à insérer.
 *
 * Si une erreur survient la fonction est sans effet et un message est affiché en console.
 */
void ihm_sdl_widget_append (t_ihm_sdl *ihm_sdl, t_widget_sdl *widget);

/*!
 * \private Fonction privée à l'objet.
 * @param ihm_sdl Pointeur de l'ihm.
 *
 * Renvoie la liste des widgets insérés dnas l'ihm.
 * Cette liste est classée pour permettre l'affichage correcte des tooltips.
 */
t_liste *ihm_get_widget_list (t_ihm_sdl *ihm_sdl);

/*!
 * \brief Transmission des évènements.
 * @param ihm_sdl Pointeur de l'ihm.
 * @param event Évènements générés par la SDL.
 *
 * À chaque itération de la boucle principale SDL cette fonction permet la transmission de ces derniers à l'ihm.
 */
void ihm_sdl_set_events (t_ihm_sdl *ihm_sdl, SDL_Event *event);

/*!
 * \brief Mise à jour graphique de tous les widgets et affichage dans le renderer.
 * @param ihm_sdl Pointeur de l'ihm.
 *
 * À chaque itération de la boucle principale SDL cette fonction demande à chaque widget de mettre à jour son aspect graphique et l'affichage dans le renderer.
 */
void ihm_sdl_renderer_update (t_ihm_sdl *ihm_sdl);














#endif
