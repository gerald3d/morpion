#ifndef __CONFIG_CURSOR_SDL__
#define __CONFIG_CURSOR_SDL__

#include <string.h>
#include "widget_sdl.h"

/*! \class t_config_cursor_sdl
    \brief Un curseur positionnable

    Il est spécifique au configurateur de taille du tableau _config_game_sdl.
*/

//
///*! \enum BUTTON_SDL_TYPE_OF_BUTTON_STATE
// * Indique dans quel état doit être le bouton.
// */
//typedef enum {ACTIF, INACTIF, INVISIBLE} BUTTON_SDL_TYPE_OF_BUTTON_STATE; //pour afficher les boutons en gris ou en noir
///*! \enum BUTTON_SDL_TYPE_OF_BUTTON
// * Choix du type de bouton.
// */
//typedef enum {IMAGE, TEXTE} BUTTON_SDL_TYPE_OF_BUTTON;

/*!
 * \private Structure privée.
 * L'utilisateur final ne doit pas accéder à un de ses membres directement.
 */
typedef struct t_config_cursor_sdl t_config_cursor_sdl;

/*!
 * \private Structure privée.
 * L'utilisateur final ne doit pas accéder à un de ses membres directement.
 */
struct t_config_cursor_sdl {
  t_widget_sdl *widget;    // widget parent qui gère les fonctions principales graphiques
  SDL_Surface *image;
  SDL_Surface *surface;    // Surface pour dessiner le bouton selon son état
//  void (*on_activate)(t_button_sdl *button, void*user_data); // callback activé lors du clic souris
//  void *userdata;          // donnée personnelle transmise au callback
};

/*!
 * \brief Constructeur d'un nouvel objet t_config_cursor_sdl.
 * @param size Position est taille initiale du bouton.
 * @param logs Objet pour inscrire les logs dans des fichiers
 *
 * Le constructeur renvoie NULL en cas d'erreur.
 */
t_config_cursor_sdl *config_cursor_sdl_new (SDL_Rect size, t_logs *logs);

/*! \private
 * \brief Destructeur de l'objet t_button_sdl.
 * @param config_cursor Pointeur de l'adresse de l'objet à détruire.
 *
 * Le pointeur vaut NULL au retour de la fonction.
 */
void config_cursor_sdl_free (void **config_cursor);

/*! \private
 * \brief Renvoie le widget parent.
 * @param config_cursor Le curseur.
 *
 * Si une erreur survient la fonction est sans effet et un message est affiché en console.
 */
t_widget_sdl *config_cursor_sdl_get_widget (t_config_cursor_sdl *config_cursor);

/*!
 * \brief Choix de l'image pour représenter le curseur depuis le disque dur.
 * @param config_cursor Le curseur.
 * @param file Chemin complet de l'image à charger. Pour une gestion automatique de la transparence le fichier devra être du type .png.
 *
 * Si une erreur survient la fonction renvoie false avec un messsage en console.
 */
bool config_cursor_sdl_set_image_from_file (t_config_cursor_sdl *config_cursor, const char *file);

/*!
 * \brief Choix de l'image pour représenter le curseur depuis une surface SDL.
 * @param config_cursor Le bouton.
 * @param image Image à insérer.
 *
 * Si une erreur survient la fonction est sans effet et un message est affiché en console.
 */
bool config_cursor_sdl_set_image_from_surface (t_config_cursor_sdl *config_cursor, SDL_Surface *image);


#endif
