#ifndef __WIDGET_SDL__
#define __WIDGET_SDL__

#include <stdbool.h>
#include <SDL2/SDL.h>// pour utiliser la sdl pour coller autre que des bmp
#include <SDL2/SDL_ttf.h> // Pour utiliser les polices avec la SDL
#include <SDL2/SDL_mixer.h> // Pour utiliser les sons avec la SDL

#include "logs.h"
#include "define_sdl.h"
#include "tooltip_sdl.h"
#include "chain_list.h"

/*! \class t_widget_sdl
    \brief Objet parent à tous les widgets.

    Il gère leur affichage ainsi que les fonctions configurables liées au évènements de la SDL.
*/

/*! \enum WIDGET_SDL_TYPE_OF_COLOR
 * Choix du composant pour midifer ou récupérer sa couleur.
 */
typedef enum {FOND, ACTIVE, CADRE, INSENSIBLE} WIDGET_SDL_TYPE_OF_COLOR;

/*!
 * \private Structure privée.
 * L'utilisateur final ne doit pas accéder à un de ses membres directement.
 */
typedef struct t_widget_sdl t_widget_sdl;

/*!
 * \private Structure privée.
 * L'utilisateur final ne doit pas accéder à un de ses membres directement.
 */
struct t_widget_sdl {
  SDL_Renderer *renderer;
  t_logs *logs;
  FILE *file_error;                  // Descripteur du fichier des logs errors
  SDL_Event *events;                 // Liste des évènements actuels transmis
  char *name;                        // Nom du widget. Utile pour le retrouver dans la liste des widgets incorporés dans l'ihm
  SDL_Surface *cursor;
  void *widget_child;                // pointeur sur le widget enfant qui implémente le widget actuel du type spécifique au widget enfant
  void (*destroy_widget_child_fct)(void **widget_whild); // Pointeur sur la fonction spécifique de destruction du widget enfant
  t_liste *child_widget_list;        // Liste des widgets enfants incorporés dans ce widget (contient des pointeurs de type t_widget_sdl)
  int x, y;                          // position de la souris
  SDL_Color couleur_fond;            // Couleur noire transparente
  SDL_Color couleur_active;          // Couleur grise à moitié transparente
  SDL_Color couleur_cadre;           // Couleur du cadre
  SDL_Color couleur_insensible;      // Couleur du widget lorsqu'il est insensible aux évènements
  int original_window_with;          // Largeur originelle de la fenêtre
  int original_window_height;        // Hauteur originelle de la fenêtre
  SDL_Rect original_size;            // Dimension et position originelle du widget
  SDL_Rect actual_size;              // Taille réelle du widget
  bool activate;                     // État du widget (true lorsque la souris est dessus)
  bool sensitive;                    // État du widget (true si le widget est sensible aux évènements)
  bool visible;                      // Affichage ou non du widget
  bool modale;                       // Si vaut true le widget doit être affiché par dessus les autres et tous les autres sont désactivés tant que ce dernier est apparent.
  void (*create_texture_cb)(t_widget_sdl *widget, void *userdata);  // callback de calcul de la texture du widget enfant
  void *create_texture_userdata;     // donnée personnelle du callback du même nom
  void (*draw_on_renderer_cb)(t_widget_sdl *widget);                // callback pour dessiner la texture dans le renderer
  t_liste *userdata;                 // données personnelles attachées au widget
  t_liste *mouse_clic_cb_list;       // liste des fonctions à appeler lorsque un clic de la souris survient
  t_liste *mouse_on_cb_list;         // liste des fonctions à appeler lorsque la souris est sur le widget
  t_liste *mouse_down_clic_cb_list;  // liste des fonctions à appeler lorsque le bouton de la souris reste enfoncée.
  t_tooltip_sdl *tooltip;            // Bulle d'aide affichée si elle existe
};

/*!
 * \private Structure privée.
 * L'utilisateur final ne doit pas accéder à un de ses membres directement.
 */
typedef struct {
  /* Callback activé lors du clic de la souris sur le widget.
   * Le paramètre any_widget pourra être de n'importe quel type
   * de widget enfant à t_widget_sdl. */
  void (*mouse_clic_cb)(void *any_widget, void *userdata);
  void *userdata;                                            // donnée personnelle pour le callback mouse_is_on_cb
} t_mouse_clic;

/*!
 * \private Structure privée.
 * L'utilisateur final ne doit pas accéder à un de ses membres directement.
 */
typedef struct {
  /* Callback activé lorsque la souris est sur le widget.
   * Le paramètre any_widget pourra être de n'importe quel type
   * de widget enfant à t_widget_sdl. */
  void (*mouse_on_cb)(void *any_widget, void *userdata);
  void *userdata;                                            // donnée personnelle pour le callback mouse_is_on_cb
} t_mouse_on;

/*!
 * \private Structure privée.
 * L'utilisateur final ne doit pas accéder à un de ses membres directement.
 */
typedef struct {
  /* Callback activé lorsque la souris est sur le widget.
   * Le paramètre any_widget pourra être de n'importe quel type
   * de widget enfant à t_widget_sdl. */
  void (*mouse_down_clic_cb)(void *any_widget, void *userdata);
  void *userdata;  // donnée personnelle pour le callback mouse_is_down_clic_cb
} t_mouse_down_clic;

/*!
 * \private Structure privée.
 * L'utilisateur final ne doit pas accéder à un de ses membres directement.
 */
typedef struct {
  char *name;
  void *userdata;
} t_userdata;

/*!
 * \brief Constructeur d'un nouvel objet t_widget_sdl.
 *
 * Le constructeur renvoie NULL en cas d'erreur.
 */
t_widget_sdl *widget_sdl_new (t_logs *logs);

/*!
 * \brief Destructeur de l'objet t_widget_sdl.
 * @param widget Pointeur de l'adresse de l'objet à détruire.
 *
 * Le pointeur vaut NULL au retour de la fonction.
 */
void widget_sdl_free (t_widget_sdl **widget);

/*! \private
 * \brief Affectation du renderer au widget
 * @param widget Le widget.
 * @param renderer Le renderer de la fenêtre SDL.
 *
 * Si une erreur survient la fonction est sans effet et un message est affiché en console.
 */
void widget_sdl_set_renderer (t_widget_sdl *widget, SDL_Renderer *renderer);

/*! \private
 * \brief Affecte la fonction callback pour calculer la texture du widget enfant
 * @param widget Le widget.
 * @param callback Fonction à appeler pour calculer la nouvelle texture.
 * @param userdata Donnée personnelle transmise au callback.
 *
 * Si une erreur survient la fonction est sans effet et un message est affiché en console.
 */
void widget_sdl_set_callback_create_texture (t_widget_sdl *widget, void *callback, void *userdata);

/*! \private
 * \brief Mise à jour graphique du renderer.
 * @param widget Le widget.
 *
 * Si une erreur survient la fonction est sans effet et un message est affiché en console.
 */
void widget_sdl_renderer_update (t_widget_sdl *widget);

/*! \private
 * \brief Affecte la fonction callback de dessin interne au widget enfant.
 * @param widget Le widget.
 *
 * Si une erreur survient la fonction est sans effet et un message est affiché en console.
 */
void widget_sdl_set_callback_renderer_update (t_widget_sdl *widget);

/*!
 * \brief Affecte la fonction callback lorsque un clic de souris est émis sur le widget
 * @param widget Le widget.
 * @param callback Fonction à appeler.
 * @param userdata Donnée personnelle transmise au callback.
 *
 * Si une erreur survient la fonction est sans effet et un message est affiché en console.
 */
void widget_sdl_set_mouse_clic_callback (t_widget_sdl *widget, void *callback, void *userdata);

/*!
 * \brief Affecte la fonction callback lorsque la souris se déplace sur le widget
 * @param widget Le widget.
 * @param callback Fonction à appeler.
 * @param userdata Donnée personnelle transmise au callback.
 *
 * Si une erreur survient la fonction est sans effet et un message est affiché en console.
 */
void widget_sdl_set_mouse_on_callback (t_widget_sdl *widget, void *callback, void *userdata);

/*!
 * \brief Affecte la fonction callback lorsqu'un bouton de la souris est enfoncé
 * @param widget Le widget.
 * @param callback Fonction à appeler.
 * @param userdata Donnée personnelle transmise au callback.
 *
 * Si une erreur survient la fonction est sans effet et un message est affiché en console.
 */
void widget_sdl_set_mouse_down_clic_callback (t_widget_sdl *widget, void *callback, void *userdata);

/*!
 * \brief Ajoute au widget courant un widget enfant.
 * @param widget Le widget.
 * @param widget_child Le widget enfant.
 *
 * Tout widget peut se comporter comme un conteneur. Par exemple une fenêtre de dialogue peut contenir un bouton de validation. Ce bouton de type t_button_sdl sera ajouté via cette fonction.
 *
 * Si une erreur survient la fonction est sans effet et un message est affiché en console.
 */
void widget_sdl_add_child_widget (t_widget_sdl *widget, t_widget_sdl *widget_child);

/*!
 * \brief Renvoie la liste des widgets enfants.
 * @param widget Le widget.
 *
 * Si aucun widget enfant n'est présent la fonction renvoie NULL.
 *
 * Si une erreur survient la fonction renvoie NULL et un message est affiché en console.
 */
t_liste *widget_sdl_get_child_widgets_list (t_widget_sdl *widget);

/*!
 * \brief Change la couleur par défaut d'une des composantes du widget
 * @param widget Le widget.
 * @param color Couleur à affecter.
 * @param type_of_color Choix du composant graphique à modifier.
 *
 * Si une erreur survient la fonction est sans effet et un message est affiché en console.
 */
void widget_sdl_set_color (t_widget_sdl *widget, SDL_Color color, WIDGET_SDL_TYPE_OF_COLOR type_of_color);

/*!
 * \brief Renvoie la couleur du composant choisi.
 * @param widget Le widget.
 * @param type_of_color Choix de la partie graphique à modifier.
 *
 * Si une erreur survient la fonction renvoie NULL et un message est affiché en console.
 */
SDL_Color *widget_sdl_get_color (t_widget_sdl *widget, WIDGET_SDL_TYPE_OF_COLOR type_of_color);

/*!
 * \brief Change la taille et la position du widget dans la fenêtre.
 * @param widget Le widget.
 * @param x Abscisse du widget.
 * @param y Ordonnée du widget.
 * @param w Largeur du widget.
 * @param h Hauteur du widget.
 *
 * Il est possible de transmettre NULL aux différents composants ce qui permet de ne modifier que l'un d'entre-eux par exemple.
 *
 * Si une erreur survient la fonction est sans effet et un message est affiché en console.
 */
void widget_sdl_set_size (t_widget_sdl *widget, int *x, int *y, int *w, int *h);

/*!
 * \brief Renvoie la position et la taille du widget dans la fenêtre dans le pointeur size.
 * @param widget Le widget.
 * @param size Position et taille du widgt renvoyés.
 *
 * Si une erreur survient la fonction renvoie false et un message est affiché en console.
 */
bool widget_sdl_get_size (t_widget_sdl *widget, SDL_Rect *size);

/*!
 * \brief Indique si le widget doit être affiché par dessus tous les autres.
 * @param widget Le widget.
 * @param modale true si le widget doit être prioritaires par rapport aux autres (false par défaut).
 *
 * Si le widget est modale tous les autres widgets sont désactivés tant que ce dernier est actif.
 *
 * Si une erreur survient la fonction est sans effet et un message est affiché en console.
 */
void widget_sdl_set_modale (t_widget_sdl *widget, bool modale);

/*!
 * \brief Renvoie l'état du widget.
 * @param widget Le widget.
 *
 * Par défaut tous les widgets ne sont pas modaux. Ils renvoient alors false.
 *
 * Si une erreur survient la fonction renvoie false et un message est affiché en console.
 */
bool widget_sdl_is_modale (t_widget_sdl *widget);

/*! \private
 * \brief Renvoie la position de la souris.
 * @param widget Le widget.
 * @param x Abscisse de la souris.
 * @param y Ordonnée de la souris.
 *
 * Si une erreur survient la fonction est sans effet et un message est affiché en console.
 */
void widget_sdl_get_mouse_position (t_widget_sdl *widget, int *x, int *y);

/*! \private
 * \brief Renvoie true si la souris est sur le widget.
 * @param widget Le widget.
 *
 * Si une erreur survient la fonction renvoie false et un message est affiché en console.
 */
bool widget_sdl_mouse_on (t_widget_sdl *widget);

/*! \private
 * \brief Renvoie la position de la souris
 * @param widget Le widget.
 * @param events Évènements en cours de la SDL.
 *
 * Si une erreur survient la fonction est sans effet et un message est affiché en console.
 */
void widget_sdl_set_events (t_widget_sdl *widget, SDL_Event *events);

/*!
 * \brief Rend visible ou non le widget.
 * @param widget Le widget.
 * @param visible true pour visible (par défaut).
 *
 * Si une erreur survient la fonction est sans effet et un message est affiché en console.
 */
void widget_sdl_set_visible (t_widget_sdl *widget, bool visible);

/*!
 * \brief Renvoie l'état d'affichage du widget.
 * @param widget Le widget.
 *
 * Si une erreur survient la fonction renvoie false et un message est affiché en console.
 */
bool widget_sdl_is_visible (t_widget_sdl *widget);

/*!
 * \brief Rend sensible ou non le widget. Lorsqu'il est insensible le widget est grisé par défaut.
 * @param widget Le widget.
 * @param state true pour sensible (par défaut).
 *
 * Si une erreur survient la fonction est sans effet et un message est affiché en console.
 */
void widget_sdl_set_sensitive (t_widget_sdl *widget, bool state);

/*!
 * \brief  Renvoie l'état de sensibilisation du widget.
 * @param widget Le widget.
 *
 * Si une erreur survient la fonction renvoie false et un message est affiché en console.
 */
bool widget_sdl_is_sensitive (t_widget_sdl *widget);

/*!
 * \brief Change le nom du widget.
 * @param widget Le widget.
 * @param name Le nouveau nom.
 *
 * Si une erreur survient ou si name est NULL la fonction est sans effet et un message est affiché en console.
 */
void widget_sdl_set_name (t_widget_sdl *widget, const char *name);

/*!
 * \brief  Renvoie le nom du widget.
 * @param widget Le widget.
 *
 * Si une erreur survient la fonction renvoie NULL et un message est affiché en console.
 */
char *widget_sdl_get_name (t_widget_sdl *widget);

/*!
 * \brief  Renvoie le pointeur des logs de l'ihm.
 * @param widget Le widget.
 *
 * Si une erreur survient la fonction renvoie NULL et un message est affiché en console.
 */
t_logs *widget_sdl_get_logs (t_widget_sdl *widget);

/*! \private
 * \brief  Renvoie true si la souris est sur le widget.
 * @param widget Le widget.
 *
 * Si une erreur survient la fonction renvoie false et un message est affiché en console.
 */
bool widget_sdl_is_activate (t_widget_sdl *widget);

/*! \private
 * \brief  Renvoie le renderer de la SDL.
 * @param widget Le widget.
 *
 * Si une erreur survient la fonction renvoie NULL et un message est affiché en console.
 */
SDL_Renderer *widget_sdl_get_renderer (t_widget_sdl *widget);

/*!
 * \brief Ajoute ou modifie une bulle d'aide.
 * @param widget Le widget
 * @param text Le texte à afficher.
 *
 * Si text = NULL la bulle d'aide est supprimée.
 *
 * Si une erreur survient la fonction est sans effet et un message est affiché en console.
 */
void widget_sdl_set_tooltip (t_widget_sdl *widget, const char *text);

/*!
 * \brief Charge une nouvelle image du curseur personnalisée lorsqu'il se déplace sur le widget.
 * @param widget Le widget
 * @param file Chemin clompet de l'image du curseur.
 *
 * Si une erreur survient la fonction renvoie false et un message est affiché en console.
 */
bool widget_sdl_set_cursor_from_file(t_widget_sdl *widget, const char *file);

/*!
 * \brief Change l_image du curseur depuis une surface.
 * @param widget Le widget
 * @param cursor Nouvelle image pour le curseur.
 *
 * Si une erreur survient la fonction est sans effet et un message est affiché en console.
 */
void widget_sdl_set_cursor_from_surface (t_widget_sdl *widget, SDL_Surface *cursor);

/* Attache une donnée personnelle */
/*!
 * \brief Attache une donnée personnelle.
 * @param widget Le widget
 * @param name Nom unique de la donnée personnelle.
 * @param userdata La donnée personnelle.
 *
 * Il est possible d'attacher un nombre "infini" de données personnelles.
 * Le nom doit être unique. Si une donnée porte déjà le même nom un message d'avertissement est affiché en console et la fonction est sans effet.
 *
 * La libération mémoire de la donnée attachée incombe à l'utilisateur.
 *
 * Si une erreur survient la fonction est sans effet et un message est affiché en console.
 */
void widget_sdl_set_user_data (t_widget_sdl *widget, const char *name, void *userdata);

/* Récupère la donnée personnelle */
/*!
 * \brief  Renvoie la donnée personnelle sélectionnée.
 * @param widget Le widget.
 * @param name Nom de la donnée recherchée.
 *
 * Si une erreur survient la fonction renvoie NULL et un message est affiché en console.
 */
void* widget_sdl_get_user_data (t_widget_sdl *widget, const char*name);

#endif
