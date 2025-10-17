#ifndef __CONFIG_CURSOR_SDL__
#define __CONFIG_CURSOR_SDL__

#include <string.h>
#include "widget_sdl.h"

/*! \class t_config_cursor_sdl
    \brief Un curseur positionnable

    Il est spécifique au configurateur de taille du tableau _config_game_sdl.
*/


/*! \enum TYPE_OF_CONFIG_CURSOR
 * Indique le type de curseur.
 */
typedef enum {HORIZONTAL, VERTICAL} TYPE_OF_CONFIG_CURSOR; // type de déplacement du curseur

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
  t_widget_sdl *widget;              // widget parent qui gère les fonctions principales graphiques
  SDL_Surface *image;
  SDL_Surface *surface;              // Surface pour dessiner le curseur selon son état  SDL_Rect start_position;           // Position graphique de départ du curseur (taille 3)
  TYPE_OF_CONFIG_CURSOR cursor_type; // Choix de déplacement du curseur
  int offset;                        // Décalage en pixels pour chaque pas de déplacement
  int new_xoffset, new_yoffset;      // Prend la valeur 1 à chaque fois que le curseur se déplace. Utilisé pour calculer la nouvelle position graphique du curseur
  unsigned int position;             // La position du curseur. Démarre à 0
  int mouse_posx, mouse_posy;        // Sauvegarde de la position de la souris lorsque le bouton de la souris vient d'être enfoncé sur le curseur
  bool first_use;                    // Si le bouton vient d'être enfoncé.
  bool pressed_button;               // Vaut true tant que le bouton est enfoncé
};

/*!
 * \brief Constructeur d'un nouvel objet t_config_cursor_sdl.
 * @param size Position est taille initiale du curseur.
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
 * @param config_cursor Le curseur.
 * @param image Image à insérer.
 *
 * Si une erreur survient la fonction est sans effet et un message est affiché en console.
 */
bool config_cursor_sdl_set_image_from_surface (t_config_cursor_sdl *config_cursor, SDL_Surface *image);

/*!
 * \brief Change le type de déplacement du cureseur
 * @param config_cursor Le curseur.
 * @param alignment Type de déplacement (HORIZONTAL par défaut).
 *
 * Si une erreur survient la fonction est sans effet et un message est affiché en console.
 */
void config_cursor_sdl_set_type (t_config_cursor_sdl *config_cursor, TYPE_OF_CONFIG_CURSOR alignment);

/*!
 * \brief Donne la position du curseur [0, ..]
 * @param config_cursor Le curseur.
 * @param position La nouvelle position
 *
 * Si une erreur survient la fonction est sans effet et un message est affiché en console.
 */
void config_cursor_sdl_set_position (t_config_cursor_sdl *config_cursor, int position);

/*!
 * \brief Renvoie la position actuelle du curseur
 * @param config_cursor Le curseur.
 *
 * Si une erreur survient la fonction est sans effet et un message est affiché en console.
 */
int config_cursor_sdl_get_position (t_config_cursor_sdl *config_cursor);


#endif
