#include "string.h"
#include "ihm_sdl.h"
#include <stdbool.h>

#define couleurMarron (SDL_Color){187, 122, 87, 255}
#define couleurMarron_active (SDL_Color){195, 195, 195, 128}

  #define nbre_casex  10
  #define nbre_casey  10

  t_case_sdl *case_sdl [nbre_casex*nbre_casey];

/* Création d'une structure qui va intégrer les informations internes d'une case.
 * La donnée de ce type sera transmise au callback lors d'un clic sur une case.
 * Les données seront alors affichées en console */
typedef struct {
  int x, y; // coordonnées de la case dans le tableau
  TYPE_OF_CASE_SDL etat; // état de la case (vide, rond, croix,...)
} t_case_jeu;

/****************************/

/* Callback appelé lorsque le bouton est cliqué */
void on_button_clic (t_button_sdl *button, void *userdata) {
  char *donnee_personnelle = (char*)userdata;
  printf ("%s\n", donnee_personnelle);
}

/* Callback appelé lorsque le bouton image est cliqué */
void on_button_image_clic (t_button_sdl *button, void *userdata) {
  /* Récupération du bouton à état pour changer son état à chaque clic sur le bouton image */
  t_state_button_sdl *state_button = (t_state_button_sdl*)userdata;

  bool etat = state_button_sdl_is_active (state_button);
  state_button_sdl_set_active (state_button, !etat);
}

/* Callback appelé lorsque le bouton de fermeture par défaut de la fenêtre de dialogue est cliqué */
void on_close_button_clic (t_button_sdl *button, void *userdata) {
 /* Récupération de l'ihm */
  t_ihm_sdl *ihm = (t_ihm_sdl*)userdata;

  /* Désactivation du plateau de jeu */
  for (int j=0; j < nbre_casey; j++) {
    for (int i=0; i < nbre_casex; i++) {
      widget_sdl_set_sensitive (case_sdl_get_widget (case_sdl[i + j*nbre_casex]), true);
    }
  }
}

/* Callback appelé lorsque le bouton configuration est cliqué */
void on_button_config_clic (t_button_sdl *button, void *userdata) {
 /* Récupération de l'ihm */
  t_ihm_sdl *ihm = (t_ihm_sdl*)userdata;

  /* Désactivation du plateau de jeu */
  for (int j=0; j < nbre_casey; j++) {
    for (int i=0; i < nbre_casex; i++) {
      widget_sdl_set_sensitive (case_sdl_get_widget (case_sdl[i + j*nbre_casex]), false);
    }
  }

  /* Récupération de la taille de la fenêtre SDL */
  int wwin, hwin;
  SDL_Window *window = SDL_RenderGetWindow (ihm_sdl_get_renderer (ihm));
  SDL_GetWindowSize(window, &wwin, &hwin);

  /* Déclaration d'une boite de dialogue en static. Lors de la première ouverture
   * si elle n'existe pas on l'a crée. Dans l'autre cas il suffit de l'afficher.
   * Lors de sa fermeture elle est simplement cachée. */
  static t_dialog_sdl *dialog = NULL;

  /* Création d'une fenêtre de dialogue si elle n'existe pas */
  int wdiag = 400; // Taille de la fenêtre de dialogue
  int hdiag = 350; // Taille de la fenêtre de dialogue
  if (dialog == NULL) {
    t_dialog_sdl *dialog = dialog_sdl_new ((SDL_Rect){wwin / 2 - wdiag / 2, hwin /2 - hdiag / 2, wdiag, hdiag});
    if (dialog == NULL)
      return;

    /* Affichage d'un titre */
    dialog_sdl_set_title (dialog, "Changement de la taille du jeu");

    /* Récupération du bouton de fermeture par défaut */
    t_button_sdl *close_button = dialog_sdl_get_close_button (dialog);

    /* Affectation d'un callback lors du clic sur le bouton de fermeture par défaut */
    widget_sdl_set_mouse_clic_callback (button_sdl_get_widget (close_button), on_close_button_clic, ihm);

    /* Insertion de la fenêtre de dialogue dans l'ihm. L'affichage est automatique */
    ihm_sdl_widget_append (ihm, dialog_sdl_get_widget (dialog));
  } else // La fenêtre de dialogue existe, on l'affiche
    widget_sdl_set_visible (dialog_sdl_get_widget (dialog), true);
}

/* Callback appelé lorsque le bouton à état est cliqué */
void on_state_button_clic (t_state_button_sdl *state_button, void *userdata) {
  /* Affichage de l'état du bouton */
  if (state_button_sdl_is_active (state_button) == true)
    printf ("Bouton à état enfoncé\n");
  else
    printf ("Bouton à état relâché\n");
}

/* Callback appelé lorsque le bouton à état est cliqué */
void on_toggle_button_clic (t_toggle_button_sdl *toggle_button, void *userdata) {
  printf ("Enter in %s\n", __func__);
  /* Affichage de l'état du bouton */
  if (toggle_button_sdl_button1_is_active (toggle_button) == true)
    printf ("Bouton 1 est actif\n");
  else
    printf ("Bouton 2 est actif\n");
}

/* Callback appelé lorsque la case est cliquée */
void on_case_clic (t_case_sdl *widget, void *userdata) {
  /* Affichage de la donnée personnelle */
  t_case_jeu *caract = (t_case_jeu*)userdata;
  printf ("donnée transmise :\n");
  printf ("coordonnées de la case : %d, %d\n", caract->x, caract->y);
  printf ("État de la case : %d\n", caract->etat);
}

/*****************************/

int main(int argc, char* argv[])
{
  SDL_Window *window = NULL;
  SDL_Renderer *renderer = NULL;
  int winx, winy;
  winx = 800;
  winy = 600;

  SDL_Init (SDL_INIT_VIDEO);
  window = SDL_CreateWindow("Exemple des différents widgets", SDL_WINDOWPOS_CENTERED,
			    SDL_WINDOWPOS_CENTERED, winx, winy, SDL_WINDOW_SHOWN);

  if(TTF_Init() == -1)     // Initialisation de SDL2_ttf
    {
      fprintf (stderr, "erreur d'initialisation de la SDL2_ttf\n");;
    }

  /* Chargement d'une police */
  TTF_Font *font = TTF_OpenFont(font_sdl, 25);

  /* Configuration de la SDL */
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

  /*--------------------------*/

  /* Création d'une interface homme-machine */
  t_ihm_sdl *ihm = ihm_sdl_new (renderer);
  if (ihm == NULL)
    return EXIT_FAILURE;

  /*--------------------------*/

  /* Création du plateau */


  /* Création des cases et insertion dans le plateau */
  t_case_jeu *case_caract [nbre_casex*nbre_casey]; // Caractéristiques de la case
  int dim_casex, dim_casey;
  dim_casey = (int)(winy/(double)nbre_casey);
  dim_casex = dim_casey;

  for (int i = 0; i < nbre_casey; i++) {
    for (int j = 0; j < nbre_casex; j++) {
      case_sdl[j + i*nbre_casex] = case_sdl_new ((SDL_Rect){j*dim_casex, i*dim_casey, dim_casex, dim_casey});
      case_caract [j + i*nbre_casex] = malloc (sizeof(t_case_jeu));
      case_caract [j + i*nbre_casex]->x = j;
      case_caract [j + i*nbre_casex]->y = i;
      case_caract [j + i*nbre_casex]->etat = VIDE;

      /* Changement de la couleur de fond lorsque la case est ensensible */
      widget_sdl_set_color (case_sdl_get_widget (case_sdl[j + i*nbre_casex]), couleurMarron, INSENSIBLE);

      /* Désactivation de la case pour essai */
      /* widget_sdl_set_sensitive (case_sdl_get_widget (case_sdl[j + i*nbre_casex]), false); */

      /* Change le cursor lorsque la souris est dessus */
      widget_sdl_set_cursor_from_file (case_sdl_get_widget (case_sdl[j + i*nbre_casex]), "images/croix_noire.png");

      /* Ajout d'une bulle d'aide */
      char bulle [50];

      char *etat = NULL;
      switch (case_caract [j + i*nbre_casex]->etat) {
      case VIDE:
	etat = strdup ("Vide");
	break;
      case  ROND:
	etat = strdup ("Rond");
	break;
      case CROIX:
	etat = strdup ("Croix");
	break;
      case RGAGNE:
	etat = strdup ("Rond gagne");
	break;
      case RPERD:
	etat = strdup ("Rond perd");
	break;
      case OGAGNE:
	etat = strdup ("Ordinateur gagne");
	break;
      case OPERD:
	etat = strdup ("Ordinateur perd");
	break;
      case COUDEPOUCE :
	etat = strdup ("Coup de pouce");
	break;
      default:
      }

      sprintf (bulle, "Case [%d, %d]\nÉtat de la case : %s\n", j, i, etat);
      free (etat);

      widget_sdl_set_tooltip (case_sdl_get_widget (case_sdl[j + i*nbre_casex]), bulle);

      /* Affectation d'un callback lors du clic de la souris sur la case.
       * Pour l'exemple affiche les caractèritisques de la case en console */
      widget_sdl_set_mouse_clic_callback (case_sdl_get_widget (case_sdl[j + i*nbre_casex]), on_case_clic, case_caract [j + i*nbre_casex]);

      /* Insertion du bouton image dans l'ihm */
      ihm_sdl_widget_append (ihm, case_sdl_get_widget (case_sdl[j + i*nbre_casex]));
    }
  }

  /*--------------------------*/

  /* Création d'un bouton Valider */
  t_button_sdl *bouton_valider = button_sdl_new (TEXTE, font, (SDL_Rect){winx-150, 100, 100, 40});
  button_sdl_set_text (bouton_valider, "Valider", TTF_STYLE_NORMAL);

  /* widget_sdl_set_modale (button_sdl_get_widget (bouton_valider), true); */

  /* Affectation d'unn curseur personnalisé lorsque la souris est dessus */
  widget_sdl_set_cursor_from_file (button_sdl_get_widget (bouton_valider), cursor_sdl);

  /* Affectation d'un callback lors du clic de la souris sur le bouton.
   * Pour l'exemple le bouton affiche une fenêtre de dialogue */
  widget_sdl_set_mouse_clic_callback (button_sdl_get_widget (bouton_valider), on_button_clic, "Donnée personnelle");

  /* Ajout d'une bulle d'aide */
  widget_sdl_set_tooltip (button_sdl_get_widget (bouton_valider), "Annule le dernier coup joué.");

  /* Désactivation du bouton valider */
  /* widget_sdl_set_sensitive (button_sdl_get_widget (bouton_valider), false); */
  /* Cache le bouton valider */
  /* widget_sdl_set_visible (button_sdl_get_widget (bouton_valider), false); */

  /* Insertion du bouton valider dans l'ihm */
  ihm_sdl_widget_append (ihm, button_sdl_get_widget (bouton_valider));

  /*--------------------------*/

  /* Création d'un bouton à état */
  t_state_button_sdl *state_button = state_button_sdl_new ((SDL_Rect){winx-150, 200, 30, 30});
  state_button_sdl_set_image (state_button, "images/image_humain.png");

  state_button_sdl_set_active (state_button, true);

  /* Affectation d'un callback lors du clic de la souris sur le bouton. */
  widget_sdl_set_mouse_clic_callback (state_button_sdl_get_widget (state_button), on_state_button_clic, NULL);

  /* Ajout d'une bulle d'aide */
  widget_sdl_set_tooltip (state_button_sdl_get_widget (state_button), "Bulle d'aide bouton à état");

  /* Insertion du bouton à état dans l'ihm */
  ihm_sdl_widget_append (ihm, state_button_sdl_get_widget (state_button));

  /*--------------------------*/

  /* Création d'un bouton Image */
  t_button_sdl *bouton_image = button_sdl_new (IMAGE, font, (SDL_Rect){winx-150, 150, 30, 30});
  button_sdl_set_image_from_file (bouton_image, "images/image_humain.png");

  /* Rendre le bouton modale */
  /* widget_sdl_set_modale (button_sdl_get_widget (bouton_image), true); */

  /* Affectation d'un callback lors du clic de la souris sur le bouton.
   * Pour l'exemple on attache un texte à la donnée personnelle */
  widget_sdl_set_mouse_clic_callback (button_sdl_get_widget (bouton_image), on_button_image_clic,
				      state_button);

  /* Ajout d'une bulle d'aide */
  widget_sdl_set_tooltip (button_sdl_get_widget (bouton_image), "Bulle d'aide image");

  /* Insertion du bouton image dans l'ihm */
  ihm_sdl_widget_append (ihm, button_sdl_get_widget (bouton_image));

  /*--------------------------*/

  /* Création d'un bouton à bascule */
  t_toggle_button_sdl *toggle_button = toggle_button_sdl_new ((SDL_Rect){winx-150, 250, 60, 30});

  /* Récupération des deux boutons pour configuration */
  t_state_button_sdl *button1 = toggle_button_sdl_get_state_button (toggle_button, BUTTON1);
  t_state_button_sdl *button2 = toggle_button_sdl_get_state_button (toggle_button, BUTTON2);

  /* Ajout d'une image pour chaque bouton */
  state_button_sdl_set_image (button1, "images/image_humain.png");
  state_button_sdl_set_image (button2, "images/image_ordi.png");

  /* Affectation d'un callback lors du clic de la souris sur chaque bouton. */
  widget_sdl_set_mouse_clic_callback (toggle_button_sdl_get_widget (toggle_button), on_toggle_button_clic, NULL);

  /* Ajout d'une bulle d'aide */
  widget_sdl_set_tooltip (toggle_button_sdl_get_widget (toggle_button), "Bulle d'aide bouton à bascule");

  /* Insertion du bouton à bascule dans l'ihm */
  ihm_sdl_widget_append (ihm, toggle_button_sdl_get_widget (toggle_button));

  /*--------------------------*/

  /* Création d'un bouton Image pour la configuration de la taille */
  t_button_sdl *bouton_config = button_sdl_new (IMAGE, NULL, (SDL_Rect){winx-150, 290, 100, 40});
  button_sdl_set_image_from_file (bouton_config, "images/config_size_3.png");

  /* Affectation d'un callback lors du clic de la souris sur le bouton.
   * Pour l'exemple on attache un texte à la donnée personnelle */
  widget_sdl_set_mouse_clic_callback (button_sdl_get_widget (bouton_config), on_button_config_clic, ihm);

  /* Ajout d'une bulle d'aide */
  widget_sdl_set_tooltip (button_sdl_get_widget (bouton_config), "Bulle d'aide configuration");

  /* Insertion du bouton image dans l'ihm */
  ihm_sdl_widget_append (ihm, button_sdl_get_widget (bouton_config));

  /*--------------------------*/



  int attendre = 1;
  SDL_Event event;

  // Boucle d'evenements
  while(attendre)
    {
      /* Effacement de la fenêtre */
      SDL_SetRenderDrawColor(renderer, 187, 122, 87, 255);
      SDL_RenderClear (renderer);

      /* Récupération des évènements */
      SDL_PollEvent (&event);

      /* Transmission des évènements à l'ihm */
      ihm_sdl_set_events (ihm, &event);

      switch(event.type)
        {
	case SDL_QUIT:
	  attendre = 0;
	  break;
        }

      /* Mise à jour graphique des widgets */
      ihm_sdl_renderer_update (ihm);

      // Actualisation de la fenêtre
      SDL_RenderPresent(renderer);
    }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit ();

  /* Libération mémoire de l'interface homme-machine */
  ihm_sdl_free (&ihm);

  /* Libération du bouton valider */
  button_sdl_free (&bouton_valider);

  /* Libération du bouton image */
  button_sdl_free (&bouton_image);

  /* Libération du bouton à état */
  state_button_sdl_free (&state_button);

  /* Libération de toutes les cases du jeu */
  for (int i=0; i < nbre_casex*nbre_casey; i++) {
    /* Libération de la donnée attachée qui est allouée dans le tas */
    free (case_caract[i]);
    /* Libération de la case */
    case_sdl_free (&case_sdl[i]);
  }


  return EXIT_SUCCESS;
}
