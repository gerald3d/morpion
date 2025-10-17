#include "game_config_sdl.h"

static SDL_Texture *game_config_sdl_create_texture (t_game_config_sdl *game_config, SDL_Renderer *renderer);
/* Affiche le bouton sur renderer transmis */
static void game_config_sdl_update (t_widget_sdl *widget, void *userdata);


t_game_config_sdl*
game_config_sdl_new (SDL_Rect size, t_logs *logs) {
	if (logs == NULL) {
		fprintf (stderr, "Erreur dans %s(); : logs doit être non NULL.\n", __func__);
		return NULL;
	}

  /* Tentative d'allocation d'un nouveau t_game_config_sdl */
  t_game_config_sdl *game_config = malloc (sizeof(t_game_config_sdl));

  if (game_config == NULL) {
    fprintf (logs_descripteur_fichier(logs, LOG_ERROR), "Erreur dans %s(); : erreur d'allocation mémoire de *game_config.\n", __func__);
    return NULL;
  }

  game_config->widget = widget_sdl_new (logs);
  if (game_config->widget == NULL) {
    game_config_sdl_free ((void**)&game_config);
    return NULL;
  }

  /* Affectation du pointeur de l'objet à l'objet parent */
  game_config->widget->widget_child = game_config;

  /* Affectation de la fonction de destruction du configurateur au widget */
  game_config->widget->destroy_widget_child_fct = game_config_sdl_free;

  widget_sdl_set_size (game_config->widget, &size.x, &size.y, &size.w, &size.h);

  /* Couleur transparente pour le cadre */
  widget_sdl_set_color (game_config->widget, (SDL_Color){0, 0, 0, 0}, CADRE);

  game_config->image = IMG_Load("ihm/images/image-CurseurAxe.png");
  if(game_config->image==NULL) {
    fprintf (logs_descripteur_fichier(logs, LOG_ERROR), "Erreur dans %s(); : %s\n", __func__, SDL_GetError());
    game_config_sdl_free ((void**)&game_config);
    return NULL;
  }
  game_config->corres_position_cases = malloc(15*sizeof(int));
  game_config->corres_position_cases [0] = 0;
  game_config->corres_position_cases [1] = 0;
  game_config->corres_position_cases [2] = 0;
  game_config->corres_position_cases [3] = 3;
  game_config->corres_position_cases [4] = 4;
  game_config->corres_position_cases [5] = 5;
  game_config->corres_position_cases [6] = 6;
  game_config->corres_position_cases [7] = 8;
  game_config->corres_position_cases [8] = 10;
  game_config->corres_position_cases [9] = 12;
  game_config->corres_position_cases [10] = 15;
  game_config->corres_position_cases [11] = 20;
  game_config->corres_position_cases [12] = 22;
  game_config->corres_position_cases [13] = 25;
  game_config->corres_position_cases [14] = 30;
  game_config->corres_cases_position = malloc(31*sizeof(int));
  game_config->corres_cases_position [3] = 3;
  game_config->corres_cases_position [4] = 4;
  game_config->corres_cases_position [5] = 5;
  game_config->corres_cases_position [6] = 6;
  game_config->corres_cases_position [8] = 7;
  game_config->corres_cases_position [10] = 8;
  game_config->corres_cases_position [12] = 9;
  game_config->corres_cases_position [15] = 10;
  game_config->corres_cases_position [20] = 11;
  game_config->corres_cases_position [22] = 12;
  game_config->corres_cases_position [25] = 13;
  game_config->corres_cases_position [30] = 14;

  game_config->columns = 3;
  game_config->lines = 3;

  /***************POUR DEBUG*************/
  game_config->vert_cursor = NULL;
  game_config->horiz_cursor = NULL;

  /* Création du curseur vertical */
  game_config->vert_cursor = config_cursor_sdl_new((SDL_Rect){size.x - 10, size.y + 35 + (14 * (game_config->lines-3)), 14, 14}, logs);
  if (game_config->vert_cursor == NULL) {
		fprintf (logs_descripteur_fichier(logs, LOG_ERROR), "Erreur dans %s(); : erreur d'allocation mémoire de vert_cursor.\n", __func__);
    game_config_sdl_free ((void**)&game_config);
    return NULL;
  }

  config_cursor_sdl_set_type (game_config->vert_cursor, VERTICAL);
  config_cursor_sdl_set_image_from_file(game_config->vert_cursor, "ihm/images/image-Curseur.png");

  /* Ajout du curseur vertical au game_config */
  widget_sdl_add_child_widget (game_config->widget, config_cursor_sdl_get_widget(game_config->vert_cursor));

  /* Création du curseur horizontal */
  game_config->horiz_cursor = config_cursor_sdl_new((SDL_Rect){size.x + 35 + (14 * (game_config->columns-3)), size.y - 10, 14, 14}, logs);
  if (game_config->horiz_cursor == NULL) {
		fprintf (logs_descripteur_fichier(logs, LOG_ERROR), "Erreur dans %s(); : erreur d'allocation mémoire de horiz_cursor.\n", __func__);
    game_config_sdl_free ((void**)&game_config);
    return NULL;
  }

  config_cursor_sdl_set_type (game_config->horiz_cursor, HORIZONTAL);
  config_cursor_sdl_set_image_from_file(game_config->horiz_cursor, "ihm/images/image-Curseur.png");

	/* Ajout du curseur horizontal au game_config */
  widget_sdl_add_child_widget (game_config->widget, config_cursor_sdl_get_widget(game_config->horiz_cursor));

  /* Affectation du callback du dessin de la texture au widget parent */
  widget_sdl_set_callback_create_texture (game_config->widget, game_config_sdl_update, game_config);

  return game_config;
}

void
game_config_sdl_free (void **game_config) {
  if (game_config == NULL) {
    fprintf (stderr, "Erreur dans %s(); : **game_config ne doit pas être NULL.\n", __func__);
    return;
  }

  if (*game_config == NULL) {
    fprintf (stderr, "Erreur dans %s(); : game_config ne doit pas être NULL.\n", __func__);
    return;
  }

  t_game_config_sdl *config = *game_config;

  if (config->widget)
    widget_sdl_free (&config->widget);

  if (config->image)
    SDL_FreeSurface (config->image);

	if (config->horiz_cursor) {
		t_widget_sdl *widget = config_cursor_sdl_get_widget(config->horiz_cursor);
		widget_sdl_free(&widget);
	}

	if (config->vert_cursor) {
		t_widget_sdl *widget = config_cursor_sdl_get_widget(config->vert_cursor);
		widget_sdl_free(&widget);
	}
	free (config->corres_cases_position);	free (config->corres_position_cases);
  free (config);

  config = NULL;
}

t_widget_sdl*
game_config_sdl_get_widget (t_game_config_sdl *game_config) {
  if (game_config == NULL) {
    fprintf (stderr, "Erreur dans %s(); : game_config ne doit pas être NULL.\n", __func__);
    return NULL;
  }

 return game_config->widget;
}

void
game_config_sdl_get_tab_size (t_game_config_sdl *game_config, int *horizontal, int *vertical) {
	if (game_config == NULL) {
    fprintf (stderr, "Erreur dans %s(); : game_config ne doit pas être NULL.\n", __func__);
    return;
  }

  if (horizontal != NULL)
		*horizontal = game_config->corres_position_cases [config_cursor_sdl_get_position (game_config->horiz_cursor)];

  if (vertical != NULL)
		*vertical = game_config->corres_position_cases [config_cursor_sdl_get_position (game_config->vert_cursor)];
}

void
game_config_sdl_set_tab_size (t_game_config_sdl *game_config, int *horizontal, int *vertical) {
	if (game_config == NULL) {
    fprintf (stderr, "Erreur dans %s(); : game_config ne doit pas être NULL.\n", __func__);
    return;
  }  if (horizontal != NULL && *horizontal>=3 && *horizontal<31)
		config_cursor_sdl_set_position (game_config->horiz_cursor, game_config->corres_cases_position [*horizontal]);

  if (vertical != NULL && *vertical>=3 && *vertical<31)
		config_cursor_sdl_set_position (game_config->vert_cursor, game_config->corres_cases_position [*vertical]);
}

/*------------------------------------------------------------------*/

static SDL_Texture*
game_config_sdl_create_texture (t_game_config_sdl *game_config, SDL_Renderer *renderer) {
  if (game_config == NULL) {
    fprintf (stderr, "Erreur dans %s(); : game_config ne doit pas être NULL.\n", __func__);
    return NULL;
  }

  if (renderer == NULL) {
    fprintf (stderr, "Erreur dans %s(); : renderer ne doit pas être NULL.\n", __func__);
    return NULL;
  }

  SDL_Texture *texture;

  /* Mise à jour de l'image */
  texture = SDL_CreateTextureFromSurface(widget_sdl_get_renderer (game_config->widget), game_config->image);

  return texture;
}

static void
game_config_sdl_update (t_widget_sdl *widget, void *userdata) {
  if (widget == NULL) {
    fprintf (stderr, "Erreur dans %s(); : widget ne doit pas être NULL.\n", __func__);
    return;
  }

  if (userdata == NULL) {
    fprintf (stderr, "Erreur dans %s(); : userdata ne doit pas être NULL.\n", __func__);
    return;
  }

  t_game_config_sdl *game_config = (t_game_config_sdl*)userdata;

  SDL_Texture *texture = game_config_sdl_create_texture (game_config, widget_sdl_get_renderer (widget));

  /* Récupération de la texture du texte ou de l'image */
  if (texture == NULL)
    return;

  /* Récupération de la taille du widget */
  SDL_Rect widget_size;
  widget_sdl_get_size (game_config->widget, &widget_size);

  SDL_RenderCopy(widget_sdl_get_renderer (game_config->widget), texture, NULL, &widget_size);

  SDL_DestroyTexture (texture);
}


