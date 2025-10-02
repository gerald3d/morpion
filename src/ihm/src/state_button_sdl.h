#ifndef __STATE_BUTTON_SDL__
#define __STATE_BUTTON_SDL__

#include "widget_sdl.h"
#include "state_button_sdl.h"

/*! \class t_state_button_sdl
    \brief Un simple bouton cliquable à état..

    Il ne permet d'afficher qu'image. La couleur de fond change en fonction de son état relâché ou enfoncé.
*/

/*!
 * \private Structure privée.
 * L'utilisateur final ne doit pas accéder à un de ses membres directement.
 */
typedef struct t_state_button_sdl t_state_button_sdl;

/*!
 * \private Structure privée.
 * L'utilisateur final ne doit pas accéder à un de ses membres directement.
 */
struct t_state_button_sdl {
  t_widget_sdl *widget; // objet parent
  t_state_button_sdl *button1;
  t_state_button_sdl *button2;
  SDL_Surface *image;
  bool state;           // bouton enfoncé ou pas (false pour relâché)
};

/*!
 * \brief Constructeur d'un nouvel objet t_state_button_sdl.
 * @param size Position est taille initiale du bouton à état.
 *
 * Le constructeur renvoie NULL en cas d'erreur.
 */
t_state_button_sdl *state_button_sdl_new (SDL_Rect size);

/*! \private
 * \brief Destructeur de l'objet t_state_button_sdl.
 * @param state_button Pointeur de l'adresse de l'objet à détruire.
 *
 * Le pointeur vaut NULL au retour de la fonction.
 */
void state_button_sdl_free (void **state_button);

/*! \private
 * \brief Renvoie le widget parent.
 * @param state_button Le bouton à état.
 *
 * Si une erreur survient la fonction renvoie NULL et un message est affiché en console.
 */
t_widget_sdl *state_button_sdl_get_widget (t_state_button_sdl *state_button);

/*!
 * \brief Choix de l'image à insérer dans le bouton à état.
 * @param state_button Le bouton à état.
 * @param file Chemin complet de l'image à charger. Pour une gestion automatique de la transparence le fichier devra être du type .png.
 *
 * Si une erreur survient la fonction renvoie false avec un messsage en console.
 */
bool state_button_sdl_set_image (t_state_button_sdl *state_button, const char *file);

/*!
 * \brief Renvoie l'état du bouton
 * @param state_button Le bouton à état.
 *
 * Retourne false pour relâché et true pour enfoncé.
 *
 * Si une erreur survient la fonction renvoie false avec un messsage en console.
 */
bool state_button_sdl_is_active (t_state_button_sdl *state_button);

/*!
 * \brief Change l'état du bouton
 * @param state_button Le bouton à état.
 * @param state false pour relâché et true pour enfoncé.
 *
 * Si une erreur survient la fonction renvoie false avec un messsage en console.
 */
void state_button_sdl_set_active (t_state_button_sdl *state_button, bool state);

/* Affiche le bouton sur le renderer transmis */
void state_button_sdl_update (t_widget_sdl *widget, void *userdata);










#endif
