#include "config_cursor_sdl.h"

/* Affiche le bouton sur le renderer transmis */
static void config_cursor_sdl_update (t_widget_sdl *widget, void *userdata);

/* Callback appelé tant que le bouton de la souris est appuyé */
static void config_cursor_sdl_mouse_button_down_cb (t_config_cursor_sdl *config_cursor, void *userdata);


t_config_cursor_sdl*
config_cursor_sdl_new (SDL_Rect size, t_logs *logs) {
  /* Tentative d'allocation d'un nouveau t_config_cursor_sdl */
  t_config_cursor_sdl *config_cursor = malloc (sizeof(t_config_cursor_sdl));

  if (config_cursor == NULL) {
		FILE *error = logs_descripteur_fichier (logs, LOG_ERROR);
    fprintf (error, "Erreur dans %s(); : erreur d'allocation mémoire de *config_cursor.\n", __func__);
    return NULL;
  }

  config_cursor->widget = widget_sdl_new (logs);
  if (config_cursor->widget == NULL) {
    config_cursor_sdl_free ((void**)&config_cursor);
    return NULL;
  }

  /* Affectation du pointeur de l'objet à l'objet parent */
  config_cursor->widget->widget_child = config_cursor;

  /* Affectation de la fonction de destruction du bouton au widget */
  config_cursor->widget->destroy_widget_child_fct = config_cursor_sdl_free;

  widget_sdl_set_size (config_cursor->widget, &size.x, &size.y, &size.w, &size.h);

  /* transparence du cadre et du fond */
  widget_sdl_set_color (config_cursor->widget, (SDL_Color){0, 0, 0, 0}, CADRE);
  widget_sdl_set_color (config_cursor->widget, (SDL_Color){0, 0, 0, 0}, FOND);

  config_cursor->image = NULL;

  /* Affectation du callback du dessin de la texture au widget parent */
  widget_sdl_set_callback_create_texture (config_cursor->widget, config_cursor_sdl_update, config_cursor);

  config_cursor->cursor_type = HORIZONTAL;
  config_cursor->offset = 14;
  config_cursor->position = 3;

  /* Affectation d'un callback activé tant que le bouton de la souris est enfoncé */
  widget_sdl_set_mouse_down_clic_callback(config_cursor->widget, config_cursor_sdl_mouse_button_down_cb, NULL);

  /* Change le curseur lorsque la souris est sur le curseur */
  widget_sdl_set_cursor_from_file (config_cursor->widget, cursor_sdl);

  return config_cursor;
}

void
config_cursor_sdl_free (void **config_cursor) {
  if (config_cursor == NULL) {
    fprintf (stderr, "Erreur dans %s(); : **config_cursor ne doit pas être NULL.\n", __func__);
    return;
  }

  if (*config_cursor == NULL) {
    fprintf (stderr, "Erreur dans %s(); : config_cursor ne doit pas être NULL.\n", __func__);
    return;
  }

  t_config_cursor_sdl *intern_config_cursor = *config_cursor;

  if (intern_config_cursor->image)
    SDL_FreeSurface (intern_config_cursor->image);

  free (intern_config_cursor);

  intern_config_cursor = NULL;
}

t_widget_sdl*
config_cursor_sdl_get_widget (t_config_cursor_sdl *config_cursor) {
  if (config_cursor == NULL) {
    fprintf (stderr, "Erreur dans %s(); : config_cursor ne doit pas être NULL.\n", __func__);
    return NULL;
  }

 return config_cursor->widget;
}

bool
config_cursor_sdl_set_image_from_file (t_config_cursor_sdl *config_cursor, const char *file) {
  if (config_cursor == NULL) {
    fprintf (stderr, "Erreur dans %s(); : config_cursor ne doit pas être NULL.\n", __func__);
    return false;
  }

  if (file == NULL) {
    fprintf (config_cursor->widget->file_error, "Erreur dans %s(); : file ne doit pas être NULL.\n", __func__);
    return false;
  }

  if (config_cursor->image)
    SDL_FreeSurface (config_cursor->image);

  config_cursor->image = IMG_Load(file);
  if(config_cursor->image==NULL) {
    fprintf (config_cursor->widget->file_error, "Erreur dans %s(); : %s\n", __func__, SDL_GetError());
    return false;
  }

  return true;
}

bool
config_cursor_sdl_set_image_from_surface (t_config_cursor_sdl *config_cursor, SDL_Surface *image) {
  if (config_cursor == NULL) {
    fprintf (stderr, "Erreur dans %s(); : config_cursor ne doit pas être NULL.\n", __func__);
    return false;
  }

  if (image == NULL) {
    fprintf (config_cursor->widget->file_error, "Erreur dans %s(); : image ne doit pas être NULL.\n", __func__);
    return false;
  }

  if (config_cursor->image)
    SDL_FreeSurface (config_cursor->image);

  config_cursor->image = image;
  if(config_cursor->image==NULL) {
    fprintf (config_cursor->widget->file_error, "Erreur dans %s(); : %s\n", __func__, SDL_GetError());
    return false;
  }

  return true;
}

void
config_cursor_set_type (t_config_cursor_sdl *config_cursor, TYPE_OF_CONFIG_CURSOR alignment) {
  if (config_cursor == NULL) {
    fprintf (stderr, "Erreur dans %s(); : config_cursor ne doit pas être NULL.\n", __func__);
    return;
  }

    if (alignment != VERTICAL && alignment!=HORIZONTAL) {
    fprintf (config_cursor->widget->file_error, "Erreur dans %s(); : alignment doit être VERTICAL ou HORIZONTAL.\n", __func__);
    return;
  }

  config_cursor->cursor_type = alignment;
}

void
config_cursor_set_position (t_config_cursor_sdl *config_cursor, unsigned int position) {
  if (config_cursor == NULL) {
    fprintf (stderr, "Erreur dans %s(); : config_cursor ne doit pas être NULL.\n", __func__);
    return;
  }

  config_cursor->position = position;
}

unsigned int
config_cursor_sdl_get_position (t_config_cursor_sdl *config_cursor) {
  if (config_cursor == NULL) {
    fprintf (stderr, "Erreur dans %s(); : config_cursor ne doit pas être NULL.\n", __func__);
    return 0;
  }

  return config_cursor->position;
}


/*------------------------------------------------------------------*/

static void
config_cursor_sdl_update (t_widget_sdl *widget, void *userdata) {
  if (widget == NULL) {
    fprintf (stderr, "Erreur dans %s(); : widget ne doit pas être NULL.\n", __func__);
    return;
  }

  if (userdata == NULL) {
    fprintf (widget->file_error, "Erreur dans %s(); : userdata ne doit pas être NULL.\n", __func__);
    return;
  }
  t_config_cursor_sdl *config_cursor = (t_config_cursor_sdl*)userdata;

  SDL_Color color = config_cursor->widget->couleur_fond;

  /* Remplissage du fond du bouton */
  SDL_SetRenderDrawColor(config_cursor->widget->renderer, color.r, color.g, color.b, color.a);
  SDL_RenderFillRect(widget_sdl_get_renderer (config_cursor->widget), &config_cursor->widget->actual_size);

  SDL_Texture *texture = SDL_CreateTextureFromSurface(widget_sdl_get_renderer (config_cursor->widget), config_cursor->image);

  /* Récupération de la texture du texte ou de l'image */
  if (texture == NULL)
    return;

  /* Remplissage du bouton */
  SDL_Rect widget_size;
  widget_sdl_get_size (widget, &widget_size);

  SDL_Rect size;
	SDL_QueryTexture(texture, NULL, NULL, &size.w, &size.h);

	/* Calcul de la position de la texture (centrée) dans le cadre */
	if (config_cursor->cursor_type == HORIZONTAL) {
		size.x = widget_size.x + ((config_cursor->position-3)*config_cursor->offset) + widget_size.w/2 - size.w/2;
		size.y = widget_size.y + widget_size.h/2 - size.h/2;
	} else {
		size.x = widget_size.x + widget_size.w/2 - size.w/2;
		size.y = widget_size.y + ((config_cursor->position-3)*config_cursor->offset) + widget_size.h/2 - size.h/2;
	}

	widget_sdl_set_size (widget, &size.x, &size.y, &widget_size.w, &widget_size.h);

  /* Application de la texture dans le rendu */
  SDL_RenderCopy(widget_sdl_get_renderer (widget), texture, NULL, &widget_size);

  SDL_DestroyTexture (texture);
}

static void
config_cursor_sdl_mouse_button_down_cb (t_config_cursor_sdl *config_cursor, void *userdata) {
  if (config_cursor == NULL) {
    fprintf (stderr, "Erreur dans %s(); : config_cursor ne doit pas être NULL.\n", __func__);
    return;
  }
fprintf (logs_descripteur_fichier(config_cursor->widget->logs, LOG_ERROR), "Enter in %s\n", __func__);
  fprintf (logs_descripteur_fichier(config_cursor->widget->logs, LOG_ERROR), "widget name %s\n", widget_sdl_get_name(config_cursor->widget));

  static int mouse_posx=0, mouse_posy=0;
  static bool first_use = true;

  if (first_use) {
		mouse_posx = config_cursor->widget->events->motion.x;
		mouse_posy = config_cursor->widget->events->motion.y;
		first_use = false;
		return;
  }

//  t_config_cursor_sdl *config_cursor = (t_config_cursor_sdl*)widget->widget_child;
  switch (config_cursor->cursor_type) {
		case HORIZONTAL :
			if (config_cursor->widget->events->motion.x - mouse_posx <=-config_cursor->offset ||
					config_cursor->widget->events->motion.x + mouse_posx >=config_cursor->offset) {
				/* Mise à jour du décalage */
				if (config_cursor->widget->events->motion.x - mouse_posx <=-config_cursor->offset) {
					if (config_cursor->position >0)
						config_cursor->position --;
				} else {
					if (config_cursor->position <27)
						config_cursor->position ++;
				}
			}
			break;
		case VERTICAL :
			if (config_cursor->widget->events->motion.y - mouse_posy <=-config_cursor->offset ||
					config_cursor->widget->events->motion.y + mouse_posy >=config_cursor->offset) {
				/* Mise à jour du décalage */
				if (config_cursor->widget->events->motion.y - mouse_posy <=-config_cursor->offset) {
					if (config_cursor->position >0)
						config_cursor->position --;
				} else {
					if (config_cursor->position <27)
						config_cursor->position ++;
				}

			}
			break;
		default :
			break;
  }
}
