#ifndef __TOGGLE_BUTTON_SDL__
#define __TOGGLE_BUTTON_SDL__

#include "state_button_sdl.h"

/*! \enum TOGGLE_BUTTON_NUMBER
 * Choix du bouton.
 */
typedef enum {BUTTON1=1, BUTTON2=2} TOGGLE_BUTTON_NUMBER;

/*!
 * \private Structure privée.
 * L'utilisateur final ne doit pas accéder à un de ses membres directement.
 */
typedef struct t_toggle_button_sdl t_toggle_button_sdl;

/*!
 * \private Structure privée.
 * L'utilisateur final ne doit pas accéder à un de ses membres directement.
 */
struct t_toggle_button_sdl {
  t_widget_sdl *widget; // objet parent
  t_state_button_sdl *button1, *button2; // les deux boutons qui basculent entre-eux
  TOGGLE_BUTTON_NUMBER num1, num2;       // données personnelles des boutons à état.
  bool button1_is_activated;
};

/*!
 * \brief Constructeur d'un nouvel objet t_toggle_button_sdl.
 * @param size Position est taille initiale du bouton à bascule. Sa taille doit contenir les deux boutons internes.
 *
 * Le constructeur renvoie NULL en cas d'erreur.
 */
t_toggle_button_sdl *toggle_button_sdl_new (SDL_Rect size);

/*!
 * \brief Destructeur de l'objet t_toggle_button_sdl.
 * @param toggle_button Pointeur de l'adresse de l'objet à détruire.
 *
 * Le pointeur vaut NULL au retour de la fonction.
 */
void toggle_button_sdl_free (t_toggle_button_sdl **toggle_button);

/*!
 * \brief Renvoie le bouton interne sélectionné.
 * @param toggle_button Le bouton à bascule.
 * @param button_number Peut prendre la valeur BUTTON1 ou BUTTON2
 *
 * Si une erreur survient la fonction renvoie NULL et un message est affiché en console.
 */
t_state_button_sdl *toggle_button_sdl_get_state_button (t_toggle_button_sdl *toggle_button, TOGGLE_BUTTON_NUMBER button_number);

/*! \private
 * \brief Renvoie le widget parent.
 * @param button Le bouton.
 *
 * Si une erreur survient la fonction est sans effet et un message est affiché en console.
 */
t_widget_sdl *toggle_button_sdl_get_widget (t_toggle_button_sdl *toggle_button);

/*!
 * \brief Renvoie l'état du bouton 1 (celui de gauche).
 * @param toggle_button Le bouton à bascule.
 *
 * Avec une seule information nous pouvons ainsi savoir lequel des deux est actif.
 *
 * Si une erreur survient la fonction renvoie false et un message est affiché en console.
 */
bool toggle_button_sdl_button1_is_active (t_toggle_button_sdl *toggle_button);











#endif
