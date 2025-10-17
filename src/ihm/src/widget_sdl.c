#include "widget_sdl.h"

static void widget_sdl_set_pointer_position (t_widget_sdl *widget, int x, int y);

t_widget_sdl*
widget_sdl_new (t_logs *logs) {
  t_widget_sdl *widget = malloc (sizeof(t_widget_sdl));

  if (widget == NULL) {
    fprintf (stderr, "Erreur dans %s(); : erreur d'allocation mémoire de widget.\n", __func__);
    return NULL;
  }

  widget->renderer = NULL;
  widget->logs = logs;
  widget->file_error = logs_descripteur_fichier (logs, LOG_ERROR);
  widget->tooltip = NULL;
  widget->name = strdup ("widget");

  widget->events = NULL;
  widget->cursor = NULL;

  widget->widget_child = NULL;
  widget->destroy_widget_child_fct = NULL;
  widget->child_widget_list = NULL;

  widget->create_texture_cb = NULL;
  widget->create_texture_userdata = NULL;
  widget->draw_on_renderer_cb = NULL;

  widget->userdata = NULL;
  widget->mouse_clic_cb_list = NULL;
  widget->mouse_on_cb_list = NULL;
  widget->mouse_down_clic_cb_list = NULL;

  widget->original_size = (SDL_Rect){0, 0, 0, 0};

  widget->couleur_fond = couleur_fond_sdl;
  widget->couleur_cadre = couleur_cadre_sdl;
  widget->couleur_active = couleur_active_sdl;
  widget->couleur_insensible = couleur_insensible_sdl;

  widget->activate = false;
  widget->sensitive = true;
  widget->visible = true;
  widget->modale = false;

  return widget;
}

void
widget_sdl_free (t_widget_sdl **widget) {
  if (widget == NULL) {
    fprintf (stderr, "Erreur dans %s(); : **widget ne doit pas être NULL.\n", __func__);
    return;
  }

  if (*widget == NULL) {
    fprintf (stderr, "Erreur dans %s(); : widget ne doit pas être NULL.\n", __func__);
    return;
  }

  if ((*widget)->name)
  	free ((*widget)->name);

  if ((*widget)->cursor)
    SDL_FreeSurface((*widget)->cursor);

  if ((*widget)->tooltip)
    tooltip_sdl_free (&(*widget)->tooltip);

  if ((*widget)->mouse_clic_cb_list) {
    t_liste *liste = (*widget)->mouse_clic_cb_list;
    while (liste) {
      t_mouse_clic *mouse_clic = (t_mouse_clic*)liste->donnee;
      free (mouse_clic);
      liste = liste->suivant;
    }
    liste_liberation (&(*widget)->mouse_clic_cb_list);
  }

   if ((*widget)->userdata)
    liste_liberation (&(*widget)->userdata);


  if ((*widget)->mouse_on_cb_list) {
    t_liste *liste = (*widget)->mouse_on_cb_list;
    while (liste) {
      t_mouse_on *mouse_on = (t_mouse_on*)liste->donnee;
      free (mouse_on);
      liste = liste->suivant;
    }
    liste_liberation (&(*widget)->mouse_on_cb_list);
  }
  
  if ((*widget)->mouse_down_clic_cb_list) {
    t_liste *liste = (*widget)->mouse_down_clic_cb_list;
    while (liste) {
      t_mouse_down_clic *mouse_down_clic = (t_mouse_down_clic*)liste->donnee;
      free (mouse_down_clic);
      liste = liste->suivant;
    }
    liste_liberation (&(*widget)->mouse_down_clic_cb_list);
  }
  
  if ((*widget)->destroy_widget_child_fct)
    (*widget)->destroy_widget_child_fct (&(*widget)->widget_child);

  if ((*widget)->child_widget_list) {
    t_liste *liste = (*widget)->child_widget_list;
    while (liste) {
      t_widget_sdl *child = (t_widget_sdl*)liste->donnee;
      if (child) {
	if (child->destroy_widget_child_fct)
	  child->destroy_widget_child_fct (&child->widget_child);

	widget_sdl_free (&child);
      }
      liste = liste->suivant;
    }
    liste_liberation (&(*widget)->child_widget_list);
  }

  free (*widget);

  *widget = NULL;
}

void
widget_sdl_set_renderer (t_widget_sdl *widget, SDL_Renderer *renderer) {
  if (widget == NULL) {
    fprintf (stderr, "Erreur dans %s(); : widget ne doit pas être NULL.\n", __func__);
    return;
  }

  if (renderer == NULL) {
    fprintf (widget->file_error, "Erreur dans %s(); : renderer ne doit pas être NULL.\n", __func__);
    return;
  }

  widget->renderer = renderer;
}

void
widget_sdl_set_callback_create_texture (t_widget_sdl *widget, void *callback, void *userdata) {
  if (widget == NULL) {
    fprintf (stderr, "Erreur dans %s(); : widget ne doit pas être NULL.\n", __func__);
    return;
  }

  widget->create_texture_cb = callback;
  widget->create_texture_userdata = userdata;
}

void
widget_sdl_renderer_update (t_widget_sdl *widget) {
  if (widget == NULL) {
     fprintf (stderr, "Erreur dans %s(); : widget ne doit pas être NULL.\n", __func__);
    return;
  }

  /* Mise à jour de la taille réelle du widget s'il n'est pas fixe  A VOIR PLUS TARD POUR LE REDIMENSSIONNEMENT DE LA FENETRE */
//  SDL_Window *window = SDL_RenderGetWindow(widget->renderer);
//  int w, h;
//  SDL_GetWindowSize(window, &w, &h);

  /* Le widget est-il sensible */
  if (widget_sdl_is_sensitive (widget) == false) {
    SDL_SetRenderDrawColor(widget->renderer, widget->couleur_insensible.r, widget->couleur_insensible.g,
			   widget->couleur_insensible.b, widget->couleur_insensible.a);
    SDL_RenderFillRect(widget->renderer, &widget->actual_size);
  }

  /* Dessin de la texture du widget enfant */
  if (widget->create_texture_cb)
    widget->create_texture_cb (widget, widget->create_texture_userdata);

  /* Dessin du cadre */
  SDL_SetRenderDrawColor(widget->renderer, widget->couleur_cadre.r, widget->couleur_cadre.g,
			 widget->couleur_cadre.b, widget->couleur_cadre.a);
  SDL_RenderDrawRect(widget->renderer, &widget->actual_size);

  /* Affichage du tooltip s'il est présent */
  if (widget->activate == true) {
    if (widget->tooltip && widget->activate) {
      widget->tooltip->renderer = widget->renderer;
      tooltip_sdl_update (widget->tooltip, &widget->actual_size);
    }
  }
}

void
widget_sdl_set_mouse_clic_callback (t_widget_sdl *widget, void *callback, void *userdata) {
  if (widget == NULL) {
    fprintf (stderr, "Erreur dans %s(); : widget ne doit pas être NULL.\n", __func__);
    return;
  }

  if (callback==NULL) {
    fprintf (stderr, "Erreur dans %s(); : callback ne doit pas être NULL.\n", __func__);
    return;
  }

  t_mouse_clic *mouse_clic = malloc (sizeof(t_mouse_clic));
  if (mouse_clic == NULL) {
    fprintf (widget->file_error, "Erreur dans %s(); : erreur d'allocation mémoire.\n", __func__);
    return;
  }

  mouse_clic->mouse_clic_cb = callback;
  mouse_clic->userdata = userdata;

  widget->mouse_clic_cb_list = liste_ajout_fin (widget->mouse_clic_cb_list, mouse_clic);
}

void
widget_sdl_set_mouse_on_callback (t_widget_sdl *widget, void *callback, void *userdata) {
  if (widget == NULL) {
    fprintf (stderr, "Erreur dans %s(); : widget ne doit pas être NULL.\n", __func__);
    return;
  }

  t_mouse_on *mouse_on = malloc (sizeof(t_mouse_on));
  if (mouse_on == NULL) {
    fprintf (widget->file_error, "Erreur dans %s(); : erreur d'allocation mémoire.\n", __func__);
    return;
  }

  mouse_on->mouse_on_cb = callback;
  mouse_on->userdata = userdata;

  widget->mouse_on_cb_list = liste_ajout_fin (widget->mouse_on_cb_list, mouse_on);
}

void
widget_sdl_set_mouse_down_clic_callback (t_widget_sdl *widget, void *callback, void *userdata) {
  if (widget == NULL) {
    fprintf (stderr, "Erreur dans %s(); : widget ne doit pas être NULL.\n", __func__);
    return;
  }

  t_mouse_down_clic *mouse_down_clic = malloc (sizeof(t_mouse_down_clic));
  if (mouse_down_clic == NULL) {
    fprintf (widget->file_error, "Erreur dans %s(); : erreur d'allocation mémoire.\n", __func__);
    return;
  }

  mouse_down_clic->mouse_down_clic_cb = callback;
  mouse_down_clic->userdata = userdata;

  widget->mouse_down_clic_cb_list = liste_ajout_fin (widget->mouse_down_clic_cb_list, mouse_down_clic);
}

void
widget_sdl_add_child_widget (t_widget_sdl *widget, t_widget_sdl *child) {
  if (widget == NULL) {
    fprintf (stderr, "Erreur dans %s(); : widget ne doit pas être NULL.\n", __func__);
    return;
  }

  if (child == NULL) {
    fprintf (widget->file_error, "Erreur dans %s(); : child ne doit pas être NULL.\n", __func__);
    return;
  }

  widget->child_widget_list = liste_ajout_fin (widget->child_widget_list, child);
}

t_liste*
widget_sdl_get_child_widgets_list (t_widget_sdl *widget) {
  if (widget == NULL) {
    fprintf (stderr, "Erreur dans %s(); : widget ne doit pas être NULL.\n", __func__);
    return NULL;
  }

  return widget->child_widget_list;
}

void
widget_sdl_set_size (t_widget_sdl *widget, int *x, int *y, int *w, int *h) {
  if (widget == NULL) {
    fprintf (stderr, "Erreur dans %s(); : widget ne doit pas être NULL.\n", __func__);
    return;
  }

  if (x)
    widget->original_size.x = *x;
  if (y)
    widget->original_size.y = *y;
  if (w)
    widget->original_size.w = *w;
  if (h)
    widget->original_size.h = *h;

  widget->actual_size = widget->original_size;
}

bool
widget_sdl_get_size (t_widget_sdl *widget, SDL_Rect *size) {
  if (widget == NULL) {
    fprintf (stderr, "Erreur dans %s(); : widget ne doit pas être NULL.\n", __func__);
    return false;
  }

  if (size == NULL) {
    fprintf (widget->file_error, "Erreur dans %s(); : size ne doit pas être NULL.\n", __func__);
    return false;
  }

  size->x = widget->actual_size.x;
  size->y = widget->actual_size.y;
  size->w = widget->actual_size.w;
  size->h = widget->actual_size.h;

  return true;
}

void
widget_sdl_set_modale (t_widget_sdl *widget, bool modale) {
  if (widget == NULL) {
    fprintf (stderr, "Erreur dans %s(); : widget ne doit pas être NULL.\n", __func__);
    return;
  }

  widget->modale = modale;
}

bool
widget_sdl_is_modale (t_widget_sdl *widget) {
  if (widget == NULL) {
    fprintf (stderr, "Erreur dans %s(); : widget ne doit pas être NULL.\n", __func__);
    return false;
  }

  return widget->modale;
}

void
widget_sdl_set_position (t_widget_sdl *widget, int x, int y){
  if (widget == NULL) {
    fprintf (stderr, "Erreur dans %s(); : widget ne doit pas être NULL.\n", __func__);
    return;
  }

  widget->original_size.x = x;
  widget->original_size.y = y;
  widget->actual_size.x = x;
  widget->actual_size.y = y;

}

void
widget_sdl_get_position (t_widget_sdl *widget, int *x, int *y) {
  if (widget == NULL) {
    fprintf (stderr, "Erreur dans %s(); : widget ne doit pas être NULL.\n", __func__);
    return;
  }

 if (x != NULL)
   *x = widget->actual_size.x;

 if (y != NULL)
   *y = widget->actual_size.y;
}

void
widget_sdl_get_mouse_position (t_widget_sdl *widget, int *x, int *y) {
  if (widget == NULL) {
    fprintf (stderr, "Erreur dans %s(); : widget ne doit pas être NULL.\n", __func__);
    return;
  }

 if (x != NULL)
   *x = widget->x;

 if (y != NULL)
   *y = widget->y;
}

bool
widget_sdl_mouse_on (t_widget_sdl *widget) {
  if (widget ==NULL) {
    fprintf (stderr, "Erreur dans %s(); : widget ne doit pas être NULL.\n", __func__);
    return false;
  }

  return widget->activate;
}

void
widget_sdl_set_visible (t_widget_sdl *widget, bool visible) {
  if (widget == NULL) {
    fprintf (stderr, "Erreur dans %s(); : widget ne doit pas être NULL.\n", __func__);
    return;
  }

  widget->visible = visible;
}

bool
widget_sdl_is_visible (t_widget_sdl *widget) {
  if (widget == NULL) {
    fprintf (stderr, "Erreur dans %s(); : widget ne doit pas être NULL.\n", __func__);
    return false;
  }

  return widget->visible;
}

t_logs*
widget_sdl_get_logs (t_widget_sdl *widget) {
  if (widget == NULL) {
    fprintf (stderr, "Erreur dans %s(); : widget ne doit pas être NULL.\n", __func__);
    return NULL;
  }

  return widget->logs;
}

static
void widget_sdl_mouse_motion_update (t_widget_sdl *widget, SDL_Event *event) {
  if (widget == NULL) {
    fprintf (stderr, "Erreur dans %s(); : widget ne doit pas être NULL.\n", __func__);
    return;
  }

  if (event == NULL) {
    fprintf (stderr, "Erreur dans %s(); : event ne doit pas être NULL.\n", __func__);
    return;
  }

  widget->x = event->motion.x;
  widget->y = event->motion.y;
  widget_sdl_set_pointer_position (widget, event->motion.x, event->motion.y);

  /* Exécution du callback lorsque la souris est sur le widget */
  if (widget_sdl_pt_is_in_rect (widget->x, widget->y, &widget->actual_size))
    widget->activate = true;
  else
    widget->activate = false;

  t_liste *liste = widget->mouse_on_cb_list;
  while (liste) {
    if (liste->donnee) {
      t_mouse_on *donnee = (t_mouse_on*)liste->donnee;
      donnee->mouse_on_cb (widget->widget_child, donnee->userdata);
    }
    liste = liste->suivant;
  }
}

static
void widget_sdl_mouse_button_down_update (t_widget_sdl *widget, SDL_Event *event) {
  if (widget == NULL) {
    fprintf (stderr, "Erreur dans %s(); : widget ne doit pas être NULL.\n", __func__);
    return;
  }

  if (event == NULL) {
    fprintf (widget->file_error, "Erreur dans %s(); : event ne doit pas être NULL.\n", __func__);
    return;
  }

  /* Mise à jour de la position de la souris */
  widget->x = event->motion.x;
  widget->y = event->motion.y;
  widget_sdl_set_pointer_position (widget, event->motion.x, event->motion.y);

  /* Exécution du callback pour l'appui du bouton de la souris */
  if (widget_sdl_pt_is_in_rect (widget->x, widget->y, &widget->actual_size)) {
    t_liste *liste = widget->mouse_down_clic_cb_list;
    while (liste) {
      if (liste->donnee) {
				t_mouse_down_clic *donnee = (t_mouse_down_clic*)liste->donnee;
				donnee->mouse_down_clic_cb (widget->widget_child, donnee->userdata);
      }
      liste = liste->suivant;
    }
  }
}

static
void widget_sdl_mouse_button_clic_update (t_widget_sdl *widget, SDL_Event *event) {
  if (widget == NULL) {
    fprintf (stderr, "Erreur dans %s(); : widget ne doit pas être NULL.\n", __func__);
    return;
  }

  if (event == NULL) {
    fprintf (widget->file_error, "Erreur dans %s(); : event ne doit pas être NULL.\n", __func__);
    return;
  }

  /* Mise à jour de la position de la souris */
  widget->x = event->motion.x;
  widget->y = event->motion.y;
  widget_sdl_set_pointer_position (widget, event->motion.x, event->motion.y);

  /* Exécution du callback pour le clic de la souris */
  if (widget_sdl_pt_is_in_rect (widget->x, widget->y, &widget->actual_size)) {
    t_liste *liste = widget->mouse_clic_cb_list;
    while (liste) {
      if (liste->donnee) {
 	t_mouse_clic *donnee = (t_mouse_clic*)liste->donnee;
	if (donnee->mouse_clic_cb)
	  donnee->mouse_clic_cb (widget->widget_child, donnee->userdata);
      }
      liste = liste->suivant;
    }
  }
}

void
widget_sdl_set_events (t_widget_sdl *widget, SDL_Event *event) {
  if (widget == NULL) {
    fprintf (stderr, "Erreur dans %s(); : widget ne doit pas être NULL.\n", __func__);
    return;
  }

  if (event == NULL) {
    fprintf (widget->file_error, "Erreur dans %s(); : event ne doit pas être NULL.\n", __func__);
    return;
  }

  /* Si le bouton est insensible on sort */
  if (widget->sensitive == false)
    return;

	widget->events = event;

  switch(event->type) {
  case SDL_MOUSEMOTION :
    /* Si le widget contient des widgets enfants il faut recommencer l'opération pour chacun d'eux */
    if (widget->child_widget_list) {
      t_liste *liste = widget->child_widget_list;
      t_widget_sdl *child = NULL;
      while (liste) {
				child = (t_widget_sdl*)liste->donnee;
				widget_sdl_mouse_motion_update (child, event);
				liste = liste->suivant;
      }
    }

    /* Mise à jour de la position de la souris */
    widget_sdl_mouse_motion_update (widget, event);

    break;
	case SDL_MOUSEBUTTONDOWN :
		{
			/* Si le widget contient des widgets enfants il faut recommencer l'opération pour chacun d'eux */
			if (widget->child_widget_list) {
				t_liste *liste = widget->child_widget_list;
				t_widget_sdl *child = NULL;
				while (liste) {
					child = (t_widget_sdl*)liste->donnee;
					widget_sdl_mouse_button_down_update (child, event);
					liste = liste->suivant;
				}
			}

			/* Mise à jour de la position de la souris */
			widget_sdl_mouse_button_down_update (widget, event);
			break;
		}
  case SDL_MOUSEBUTTONUP :
    {
      /* Si le widget contient des widgets enfants il faut recommencer l'opération pour chacun d'eux */
      if (widget->child_widget_list) {
				t_liste *liste = widget->child_widget_list;
				t_widget_sdl *child = NULL;
				while (liste) {
	  			child = (t_widget_sdl*)liste->donnee;
  				widget_sdl_mouse_button_clic_update (child, event);
	  			liste = liste->suivant;
				}
      }
     	widget_sdl_mouse_button_clic_update (widget, event);
      break;
    }
  default :
  }
}

void
widget_sdl_set_sensitive (t_widget_sdl *widget, bool sensitive) {
  if (widget == NULL) {
    fprintf (stderr, "Erreur dans %s(); : widget ne doit pas être NULL.\n", __func__);
    return;
  }

 widget->sensitive = sensitive;
}

bool
widget_sdl_is_sensitive (t_widget_sdl *widget) {
  if (widget == NULL) {
    fprintf (stderr, "Erreur dans %s(); : widget ne doit pas être NULL.\n", __func__);
    return false;
  }

 return widget->sensitive;
}

void
widget_sdl_set_name (t_widget_sdl *widget, const char *name) {
  if (widget == NULL) {
    fprintf (stderr, "Erreur dans %s(); : widget ne doit pas être NULL.\n", __func__);
    return;
  }

	if (name == NULL)
  	return;

	if (widget->name)
		free (widget->name);

	widget->name = strdup (name);
}

char*
widget_sdl_get_name (t_widget_sdl *widget) {
  if (widget == NULL) {
    fprintf (stderr, "Erreur dans %s(); : widget ne doit pas être NULL.\n", __func__);
    return NULL;
  }

 return widget->name;
}

bool
widget_sdl_is_activate (t_widget_sdl *widget) {
  if (widget == NULL) {
    fprintf (stderr, "Erreur dans %s(); : widget ne doit pas être NULL.\n", __func__);
    return false;
  }

  return widget->activate;
}

void
widget_sdl_set_color (t_widget_sdl *widget, SDL_Color color, WIDGET_SDL_TYPE_OF_COLOR type_of_color) {
  if (widget == NULL) {
    fprintf (stderr, "Erreur dans %s(); : widget ne doit pas être NULL.\n", __func__);
    return;
  }

  switch (type_of_color) {
  case FOND :
    widget->couleur_fond = color;
    break;

  case CADRE :
    widget->couleur_cadre = color;
    break;

  case ACTIVE :
    widget->couleur_active = color;
    break;

  case INSENSIBLE :
    widget->couleur_insensible = color;
    break;

  default :
    fprintf (widget->file_error, "Erreur dans %s(); : type_of_color doit avoir comme valeur FOND, CADRE, ACTIVE ou INSENSIBLE.\n", __func__);
    break;
  }
}

SDL_Color*
widget_sdl_get_color (t_widget_sdl *widget, WIDGET_SDL_TYPE_OF_COLOR type_of_color) {
  if (widget == NULL) {
    fprintf (stderr, "Erreur dans %s(); : widget ne doit pas être NULL.\n", __func__);
    return NULL;
  }

  switch (type_of_color) {
  case FOND :
    return &widget->couleur_fond;

  case CADRE :
    return &widget->couleur_cadre;

  case ACTIVE :
    return &widget->couleur_active;

  case INSENSIBLE :
    return &widget->couleur_insensible;

  default :
    fprintf (widget->file_error, "Erreur dans %s(); : type_of_color doit avoir comme valeur FOND, CADRE, ACTIVE ou INSENSIBLE.\n", __func__);
    return NULL;
  }
}

SDL_Renderer*
widget_sdl_get_renderer (t_widget_sdl *widget) {
  if (widget == NULL) {
    fprintf (stderr, "Erreur dans %s(); : widget ne doit pas être NULL.\n", __func__);
    return NULL;
  }

  return widget->renderer;
}

void
widget_sdl_set_tooltip (t_widget_sdl *widget, const char *text) {
  if (widget == NULL) {
    fprintf (stderr, "Erreur dans %s(); : widget ne doit pas être NULL.\n", __func__);
    return;
  }

  /* Suppression de la bulle d'aide si elle existe */
  if (widget->tooltip) {
    tooltip_sdl_free (&widget->tooltip);
  }

  if (text == NULL)
    return;

  widget->tooltip = tooltip_sdl_new (text);
}

void
widget_sdl_set_user_data (t_widget_sdl *widget, const char *name, void *userdata) {
  if (widget == NULL) {
    fprintf (stderr, "Erreur dans %s(); : widget ne doit pas être NULL.\n", __func__);
    return;
  }

  if (name == NULL) {
    fprintf (widget->file_error, "Erreur dans %s(); : name ne doit pas être NULL.\n", __func__);
    return;
  }

  /* S'il y a déjà des données personnelles -> vérification que le nom donné n'existe pas déjà.
   * Dans ce cas l'opération échoue. */
  if (widget->userdata != NULL) {
    t_liste *list = widget->userdata;
    while (list) {
      t_userdata *data = (t_userdata*)list->donnee;
      if (strcmp (data->name, name)==0) {
				fprintf (widget->file_error, "Erreur dans %s(); : une donnée personnelle attachée avec le nom %s existe déjà.\n", __func__, name);
				return;
      }
      list = list->suivant;
    }
  }

  /* Si la donnée est NULL on sort */
  if (userdata == NULL) {
    fprintf (widget->file_error, "Attention ! Vous tentez d'ajouter une donnée personnelle de valeur NULL dans %s();\n", __func__);
    return;
  }

  /* Création de la nouvelle donnée et ajout à la liste */
  t_userdata *data = malloc (sizeof (t_userdata));

  if (data == NULL) {
    fprintf (widget->file_error, "Erreur dans %s(); : problème d'allocation mémoire pour la donnée personnelle.\n", __func__);
    return;
  }

  data->name = strdup (name);
  data->userdata = userdata;

  widget->userdata = liste_ajout_fin (widget->userdata, data);
}

void*
widget_sdl_get_user_data (t_widget_sdl *widget, const char *name) {
  if (widget == NULL) {
    fprintf (stderr, "Erreur dans %s(); : widget ne doit pas être NULL.\n", __func__);
    return NULL;
  }

  if (name == NULL) {
    fprintf (widget->file_error, "Erreur dans %s(); : name ne doit pas être NULL.\n", __func__);
    return NULL;
  }

  if (widget->userdata == NULL) {
    fprintf (widget->file_error, "Aucune donnée personnelle est attachée au widget.\n");
    return NULL;
  }

  t_liste *list = widget->userdata;
  while (list) {
    t_userdata *data = (t_userdata*)list->donnee;
    if (strcmp (data->name, name)==0) {
      return data->userdata;
    }
    list = list->suivant;
  }

  fprintf (widget->file_error, "La donnée \"%s\" n'a pas été trouvée.\n", name);

  return NULL;
}

bool
widget_sdl_set_cursor_from_file (t_widget_sdl *widget, const char *file)
{
  if (widget == NULL) {
    fprintf (stderr, "Erreur dans %s(); : widget ne doit pas être NULL.\n", __func__);
    return false;
  }

  if (file == NULL) {
    if (widget->cursor)
      SDL_FreeSurface (widget->cursor);

    widget->cursor = NULL;
    return true;
  }

  widget->cursor = IMG_Load(file);

  if( widget->cursor==NULL) {//si image non chargée
    fprintf(widget->file_error, "Erreur dans %s(); : problème lors du chargement de l'image  : %s. (%s)\n",__func__, file, SDL_GetError());
    return false;
  }

  return true;
}

void
widget_sdl_set_cursor_from_surface (t_widget_sdl *widget, SDL_Surface *cursor) {
  if (widget == NULL) {
    fprintf (stderr, "Erreur dans %s(); : widget ne doit pas être NULL.\n", __func__);
    return;
  }

  if (widget->cursor)
    SDL_FreeSurface (widget->cursor);

  widget->cursor = cursor;
}

/*----------------------------------*/

bool
widget_sdl_pt_is_in_rect (int x, int y, SDL_Rect *rect) {
  if (rect == NULL)
    return false;

   if (x >= rect->x && y >= rect->y &&
      x <= rect->x + rect->w && y <= rect->y + rect->h)
     return true;

   return false;
}

static void
widget_sdl_set_pointer_position (t_widget_sdl *widget, int x, int y) {
  if (widget == NULL)
    return;

  if (widget_sdl_pt_is_in_rect (x, y, &widget->original_size))
    widget->activate = true; // la souris est sur le bouton
  else
    widget->activate = false; // la souris est en dehors du bouton

}


