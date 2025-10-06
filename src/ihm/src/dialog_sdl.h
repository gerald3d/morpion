#ifndef __DIALOG_SDL__
#define __DIALOG_SDL__

#include <string.h>
#include "widget_sdl.h"
#include "button_sdl.h"
#include "game_config_sdl.h"

/*! \class t_dialog_sdl
 * \brief Une fenêtre de dialogue modale
 *
 * Ce widget est un simple conteneur. Il est possible d'y ajouter autant de widgets que désiré. La position de ces widgets n'est pas relative au rectangle du t_dialog_sdl mais liée à la taille de la fenêtre.
 *
 * Ce widget est modale par défaut. Lors de son affichage il devient prioritaire au reste de tous les autres widgets affichés. Tous les widgets enfants sont aussi modaux.
*/

/*!
 * \private Structure privée.
 * L'utilisateur final ne doit pas accéder à un de ses membres directement.
 */
typedef struct t_dialog_sdl t_dialog_sdl;

/*!
 * \private Structure privée.
 * L'utilisateur final ne doit pas accéder à un de ses membres directement.
 */
struct t_dialog_sdl {
  t_widget_sdl *widget;           // widget parent qui gère les fonctions principales graphiques
  SDL_Surface *surface;           // Surface utilisée pour afficher le titre
  t_button_sdl *close_button;     // bouton de fermeture
  t_button_sdl *valid_button;     // Bouton de validation
  t_button_sdl *annul_button;     // Bouton d'annulation
  TTF_Font *font;                 // Police utilisée pour afficher les boutons et les différents textes
  char *title;                    // Titre de la fenêtre
};

/*!
 * \brief Constructeur d'un nouvel objet t_dialog_sdl.
 * @param size Position est taille initiale du bouton.
 *
 * Le constructeur renvoie NULL en cas d'erreur.
 */
t_dialog_sdl *dialog_sdl_new (SDL_Rect size, t_logs *logs);

/*!
 * \brief Destructeur de l'objet t_dialog_sdl.
 * @param dialog Pointeur de l'adresse de l'objet à détruire.
 *
 * Le pointeur vaut NULL au retour de la fonction.
 */
void dialog_sdl_free (void **dialog);

/*! \private
 * \brief Renvoie le widget parent.
 * @param dialog Le bouton.
 *
 * Si une erreur survient la fonction est sans effet et un message est affiché en console.
 */
t_widget_sdl *dialog_sdl_get_widget (t_dialog_sdl *dialog);

/*!
 * \brief Change, ajoute ou supprime le titre de la fenêtre de dialogue.
 * @param dialog La fenêtre de dialogue.
 * @param title Titre de la fenêtre.
 *
 * title peut prendre la valeur NULL. Dans ce cas aucun titre n'est affiché.
 *
 * Si une erreur survient la fonction est sans effet et un message est affiché en console.
 */
void dialog_sdl_set_title (t_dialog_sdl *dialog, const char *title);

/*!
 * \brief Renvoie le bouton de fermeture par défaut de la fenêtre.
 * @param dialog La fenêtre de dialogue.
 *
 * Si une erreur survient la fonction renvoie NULL et un message est affiché en console.
 */
t_button_sdl *dialog_sdl_get_close_button  (t_dialog_sdl *dialog);

/*!
 * \brief Renvoie le bouton d'annulation de la fenêtre.
 * @param dialog La fenêtre de dialogue.
 *
 * Si une erreur survient la fonction renvoie NULL et un message est affiché en console.
 */
t_button_sdl *dialog_sdl_get_annul_button  (t_dialog_sdl *dialog);

/*!
 * \brief Renvoie le bouton de validation de la fenêtre.
 * @param dialog La fenêtre de dialogue.
 *
 * Si une erreur survient la fonction renvoie NULL et un message est affiché en console.
 */
t_button_sdl *dialog_sdl_get_valid_button  (t_dialog_sdl *dialog);
#endif
