#include "config_cursor_sdl.h"

/* Affiche le bouton sur le renderer transmis */
static void config_cursor_sdl_update (t_widget_sdl *widget, void *userdata);


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
    config_cursor_sdl_free ((void**)config_cursor);
    return NULL;
  }

  /* Affectation du pointeur de l'objet à l'objet parent */
  config_cursor->widget->widget_child = config_cursor;

  /* Affectation de la fonction de destruction du bouton au widget */
  config_cursor->widget->destroy_widget_child_fct = config_cursor_sdl_free;

  widget_sdl_set_size (config_cursor->widget, &size.x, &size.y, &size.w, &size.h);

  config_cursor->image = NULL;
//  config_cursor->on_activate = NULL;
//  config_cursor->userdata = NULL;

  /* Affectation du callback du dessin de la texture au widget parent */
  widget_sdl_set_callback_create_texture (config_cursor->widget, config_cursor_sdl_update, config_cursor);

  config_cursor->cursor_type = HORIZONTAL;
  config_cursor->offset = 14;
  config_cursor->position = 0;

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
    fprintf (stderr, "Erreur dans %s(); : alignment doit être VERTICAL ou HORIZONTAL.\n", __func__);
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
	size.x = widget_size.x + widget_size.w/2 - size.w/2;
	size.y = widget_size.y + widget_size.h/2 - size.h/2;

  /* Application de la texture dans le rendu */
  SDL_RenderCopy(widget_sdl_get_renderer (widget), texture, NULL, &size);


  SDL_DestroyTexture (texture);
}


