#include "config_cursor_sdl.h"

/* Affiche le bouton sur le renderer transmis */
static void config_cursor_sdl_update (t_widget_sdl *widget, void *userdata);
/* Callback activé lorsque le bouton de la souris est enfoncé */
static void config_cursor_sdl_mouse_button_down_cb (t_config_cursor_sdl *config_cursor, void *userdata);
/* Callback activé lorsque la souris se déplace */
static void config_cursor_sdl_mouse_move_cb(t_config_cursor_sdl *config_cursor, void *userdata);
/* Callback activé lorsque le bouton de la souris est relâché */
static void config_cursor_sdl_mouse_button_up_cb(t_config_cursor_sdl *config_cursor, void *userdata);

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
  config_cursor->start_position = (SDL_Rect){size.x, size.y, size.w, size.h};
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
  config_cursor->new_xoffset = 0;
  config_cursor->new_yoffset = 0;

	config_cursor->mouse_posx=0;
	config_cursor->mouse_posy=0;
  config_cursor->first_use = true;
  config_cursor->pressed_button = false;

  /* Affectation d'un callback activé lorsque le bouton de la souris est enfoncé */
  widget_sdl_set_mouse_down_clic_callback(config_cursor->widget, config_cursor_sdl_mouse_button_down_cb, NULL);

  /* Affectation d'un callback activé lorsque la souris se déplace */
  widget_sdl_set_mouse_on_callback (config_cursor->widget, config_cursor_sdl_mouse_move_cb, NULL);

  /* Affectation d'un callback activé lorsque le bouton de la souris est relâché */
  widget_sdl_set_mouse_clic_callback (config_cursor->widget, config_cursor_sdl_mouse_button_up_cb, NULL);

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
config_cursor_sdl_set_type (t_config_cursor_sdl *config_cursor, TYPE_OF_CONFIG_CURSOR alignment) {
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
config_cursor_sdl_set_position (t_config_cursor_sdl *config_cursor, int position) {
  if (config_cursor == NULL) {
    fprintf (stderr, "Erreur dans %s(); : config_cursor ne doit pas être NULL.\n", __func__);
    return;
  }

  config_cursor->position = position;  /* Mise à jour graphique */  int corres_graph [] = {0,0,0,0,1,2,3,4,5,6,7,8,9,10,11};  SDL_Rect widget_size;  SDL_Rect size;  widget_sdl_get_size (config_cursor->widget, &widget_size);  widget_sdl_get_size (config_cursor->widget, &size);  if (config_cursor->cursor_type == HORIZONTAL) {    size.x = config_cursor->start_position.x + (corres_graph[position])*config_cursor->offset;    widget_sdl_set_size (config_cursor->widget, &size.x, &size.y, &widget_size.w, &widget_size.h);    config_cursor->new_yoffset = 0;  }  if (config_cursor->cursor_type == VERTICAL) {    size.y = config_cursor->start_position.y + (corres_graph[position])*config_cursor->offset;    widget_sdl_set_size (config_cursor->widget, &size.x, &size.y, &widget_size.w, &widget_size.h);    config_cursor->new_yoffset = 0;  }
}

int
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
		size.x = widget_size.x + ((config_cursor->new_xoffset)*config_cursor->offset) + widget_size.w/2 - size.w/2;
		size.y = widget_size.y + widget_size.h/2 - size.h/2;
		/* Remise à zéro du nouveau décalage puisqu'il vient d'être pris en compte */
		config_cursor->new_xoffset = 0;
	} else {
		size.x = widget_size.x + widget_size.w/2 - size.w/2;
		size.y = widget_size.y + ((config_cursor->new_yoffset)*config_cursor->offset) + widget_size.h/2 - size.h/2;
		/* Remise à zéro du nouveau décalage puisqu'il vient d'être pris en compte */
		config_cursor->new_yoffset = 0;
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

  /* Pour éviter un warning à la compilation. userdata n'est pas utilisé ici */
  (void)userdata;

  if (config_cursor->first_use) {
		config_cursor->mouse_posx = config_cursor->widget->events->motion.x;
		config_cursor->mouse_posy = config_cursor->widget->events->motion.y;
		config_cursor->first_use = false;
		config_cursor->pressed_button = true;
		return;
  }
}

static void
config_cursor_sdl_mouse_button_up_cb (t_config_cursor_sdl *config_cursor, void *userdata) {
  if (config_cursor == NULL) {
    fprintf (stderr, "Erreur dans %s(); : config_cursor ne doit pas être NULL.\n", __func__);
    return;
  }

  /* Pour éviter un warning à la compilation. userdata n'est pas utilisé ici */
  (void)userdata;

  config_cursor->pressed_button = false;
  config_cursor->first_use = true;
  config_cursor->new_xoffset = 0;
  config_cursor->new_yoffset = 0;
}

static void
config_cursor_sdl_mouse_move_cb (t_config_cursor_sdl *config_cursor, void *userdata) {
  if (config_cursor == NULL) {
    fprintf (stderr, "Erreur dans %s(); : config_cursor ne doit pas être NULL.\n", __func__);
    return;
  }

  /* Pour éviter un warning à la compilation. userdata n'est pas utilisé ici */
  (void)userdata;

  /* Si la souris n'est plus sur le curseur (avec un delta de offset) et que le bouton est relâché on sort en initialisant le curseur */
//  if (widget_sdl_pt_is_in_rect (config_cursor->widget->x, config_cursor->widget->y, &config_cursor->widget->actual_size) == false &&
//			config_cursor->pressed_button == false){
//		config_cursor->pressed_button = false;
//		config_cursor->first_use = true;
//		config_cursor->new_xoffset = 0;
//		config_cursor->new_yoffset = 0;
//  	return;
//  }

  /* Si le bouton de la souris est toujours enfoncé on regarde le déplacement de la souris */
  if (config_cursor->pressed_button) {
		switch (config_cursor->cursor_type) {
			case HORIZONTAL : {
				int ecart = config_cursor->widget->events->motion.x - config_cursor->mouse_posx;
				if (ecart >= config_cursor->offset) {
					if (config_cursor->position < 14) {
						config_cursor->position ++;
						/* Mise à jour de la position de la souris */
						config_cursor->mouse_posx = config_cursor->widget->events->motion.x;
						config_cursor->mouse_posy = config_cursor->widget->events->motion.y;
						/* décalage graphique */
						config_cursor->new_xoffset = 1;
					}
					break;
				}

				if (ecart <= -config_cursor->offset) {
					if (config_cursor->position > 3) {
						config_cursor->position --;
						/* Mise à jour de la position de la souris */
					config_cursor->mouse_posx = config_cursor->widget->events->motion.x;
					config_cursor->mouse_posy = config_cursor->widget->events->motion.y;
					/* décalage graphique */
					config_cursor->new_xoffset = -1;
					}
					break;
				}
				break;
			}
			case VERTICAL : {
				int ecart = config_cursor->widget->events->motion.y - config_cursor->mouse_posy;
				if (ecart >= config_cursor->offset) {
					if (config_cursor->position < 14) {
						config_cursor->position ++;
						/* Mise à jour de la position de la souris */
						config_cursor->mouse_posx = config_cursor->widget->events->motion.x;
						config_cursor->mouse_posy = config_cursor->widget->events->motion.y;
						/* décalage graphique */
						config_cursor->new_yoffset = 1;
					}
					break;
				}

				if (ecart <= -config_cursor->offset) {
					if (config_cursor->position > 3) {
						config_cursor->position --;
						/* Mise à jour de la position de la souris */
					config_cursor->mouse_posx = config_cursor->widget->events->motion.x;
					config_cursor->mouse_posy = config_cursor->widget->events->motion.y;
					/* décalage graphique */
					config_cursor->new_yoffset = -1;
					}
					break;
				}
				break;
			}
			default :
				break;
		}
  }
}
