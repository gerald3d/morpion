#ifndef __BUTTON_SDL__
#define __BUTTON_SDL__

#include <string.h>
#include "widget_sdl.h"

/*! \class t_button_sdl
    \brief Un simple bouton cliquable.

    Il permet d'afficher un texte ou une image.
*/

/*! \enum BUTTON_SDL_TYPE_OF_BUTTON_STATE
 * Indique dans quel état doit être le bouton.
 */
typedef enum {ACTIF, INACTIF, INVISIBLE} BUTTON_SDL_TYPE_OF_BUTTON_STATE; //pour afficher les boutons en gris ou en noir
/*! \enum BUTTON_SDL_TYPE_OF_BUTTON
 * Choix du type de bouton.
 */
typedef enum {IMAGE, TEXTE} BUTTON_SDL_TYPE_OF_BUTTON;

/*!
 * \private Structure privée.
 * L'utilisateur final ne doit pas accéder à un de ses membres directement.
 */
typedef struct t_button_sdl t_button_sdl;

/*!
 * \private Structure privée.
 * L'utilisateur final ne doit pas accéder à un de ses membres directement.
 */
struct t_button_sdl {
  t_widget_sdl *widget;    // widget parent qui gère les fonctions principales graphiques
  BUTTON_SDL_TYPE_OF_BUTTON button_type;   // type de bouton
  SDL_Surface *image;
  char *text;
  TTF_Font *font;
  SDL_Color couleur_text;  // Couleur blanche du texte
  SDL_Surface *surface;    // Surface pour dessiner le bouton selon son état
  int style;               // style d'affichage du texte
  void (*on_activate)(t_button_sdl *button, void*user_data); // callback activé lors du clic souris
  void *userdata;          // donnée personnelle transmise au callback
};

/*!
 * \brief Constructeur d'un nouvel objet t_button_sdl.
 * @param type_de_bouton Choix entre TEXTE ou IMAGE.
 * @param font Police utilisée si le bouton est de type TEXTE. Peut être à NULL si le bouton est de type IMAGE.
 * @param size Position est taille initiale du bouton.
 *
 * Le constructeur renvoie NULL en cas d'erreur.
 */
t_button_sdl *button_sdl_new (BUTTON_SDL_TYPE_OF_BUTTON type_de_bouton, TTF_Font *font, SDL_Rect size);

/*! \private
 * \brief Destructeur de l'objet t_button_sdl.
 * @param button Pointeur de l'adresse de l'objet à détruire.
 *
 * Le pointeur vaut NULL au retour de la fonction.
 */
void button_sdl_free (void **button);

/*! \private
 * \brief Renvoie le widget parent.
 * @param button Le bouton.
 *
 * Si une erreur survient la fonction est sans effet et un message est affiché en console.
 */
t_widget_sdl *button_sdl_get_widget (t_button_sdl *button);

/*!
 * \brief Choix de l'image à insérer dans le bouton s'il est du type IMAGE en chargeant l'image depuis le disque dur.
 * @param button Le bouton.
 * @param file Chemin complet de l'image à charger. Pour une gestion automatique de la transparence le fichier devra être du type .png.
 *
 * Si une erreur survient la fonction renvoie false avec un messsage en console.
 */
bool button_sdl_set_image_from_file (t_button_sdl *button, const char *file);

/*!
 * \brief Choix de l'image à insérer dans le bouton s'il est du type IMAGE depuis une surface SDL.
 * @param button Le bouton.
 * @param image Image à insérer.
 *
 * Si une erreur survient la fonction est sans effet et un message est affiché en console.
 */
bool button_sdl_set_image_from_surface (t_button_sdl *button, SDL_Surface *image);

/*!
 * \brief Change le texte affiché dans le bouton.
 * @param button Le bouton.
 * @param text Texte à insérer.
 * @param style Change le style d'affichage.
 *
 style peut prendre les valeurs suivantes :
 *
 *  TTF_STYLE_NORMAL
 *
 *  TTF_STYLE_BOLD
 *
 *  TTF_STYLE_ITALIC
 *
 *  TTF_STYLE_UNDERLINE
 *
 *  TTF_STYLE_STRIKETHROUGH
 */
void button_sdl_set_text (t_button_sdl *button, const char *text, int style);

/*!
 * \brief Renvoie le texte du bouton. S'il n'y a pas de texte la fonction renvoie NULL
 * @param button Le bouton.
 *
 * Si une erreur survient un message est affiché en console.
 */
const char *button_sdl_get_text (t_button_sdl *button);

/* Change le style d'affichage de la police */
/*!
 * \brief Change le style d'affichage de la police.
 * @param button Le bouton.
 * @param style Change le style du texte (voir button_sdl_set_text()).
 *
 * Si une erreur survient un message est affiché en console.
 */
void button_sdl_set_style (t_button_sdl *button, int style);

#endif
