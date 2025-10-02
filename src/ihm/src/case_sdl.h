#ifndef __CASE_SDL__
#define __CASE_SDL__
#include "widget_sdl.h"

/*! \class t_case_sdl
    \brief Une simple case cliquable (Ressemble à un t_button_sdl).

    Affiche une case dans laquelle il est possible d'insérer une image.
*/

/*! \enum TYPE_OF_CASE_SDL
 * Indique l'occupation de la case.
 */
typedef enum {VIDE, ROND, CROIX, RGAGNE, RPERD, OGAGNE, OPERD, COUDEPOUCE} TYPE_OF_CASE_SDL;

/*!
 * \private Structure privée.
 * L'utilisateur final ne doit pas accéder à un de ses membres directement.
 */
typedef struct {
  t_widget_sdl *widget;
  TYPE_OF_CASE_SDL type_de_case;
  SDL_Surface *image;
} t_case_sdl;

/*!
 * \brief Constructeur d'un nouvel objet t_case_sdl.
 * @param size Position est taille initiale du bouton.
 *
 * Le constructeur renvoie NULL en cas d'erreur.
 */
t_case_sdl *case_sdl_new (SDL_Rect size, t_logs *logs);

/*! \private
 * \brief Destructeur de l'objet t_case_sdl.
 * @param case_sdl Pointeur de l'adresse de l'objet à détruire.
 *
 * Le pointeur vaut NULL au retour de la fonction.
 */
void case_sdl_free (void **case_sdl);

/*! \private
 * \brief Renvoie le widget parent.
 * @param case_sdl Le bouton.
 *
 * Si une erreur survient la fonction est sans effet et un message est affiché en console.
 */
t_widget_sdl *case_sdl_get_widget (t_case_sdl *case_sdl);

/*!
 * \brief Choix de l'image à insérer dans le bouton s'il est du type IMAGE depuis une surface SDL.
 * @param case_sdl Le bouton.
 * @param image Image à insérer.
 *
 * Si image vaut NULL aucune image est insérée.
 *
 * Si une erreur survient la fonction est sans effet, renvoie false et un message est affiché en console.
 */
bool case_sdl_set_image (t_case_sdl *case_sdl, SDL_Surface *image);












#endif
