#include "game_config_sdl.h"

static SDL_Texture *game_config_sdl_create_texture (t_game_config_sdl *game_config, SDL_Renderer *renderer);
/* Affiche le bouton sur renderer transmis */
static void game_config_sdl_update (t_widget_sdl *widget, void *userdata);


t_game_config_sdl*
game_config_sdl_new (SDL_Rect size, t_logs *logs) {
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

  game_config->columns = 3;
  game_config->lines = 3;
fprintf (logs_descripteur_fichier(logs, LOG_STANDARD), "position du curseur vertical : %d, %d\n", size.x - 10, size.y + 35 + (14 * (game_config->lines-3)));

  /* Création du curseur vertical */
  game_config->vert_cursor = config_cursor_sdl_new((SDL_Rect){size.x - 10, size.y + 35 + (14 * (game_config->lines-3)), 14, 14}, logs);
  if (game_config->vert_cursor == NULL) {
		fprintf (logs_descripteur_fichier(logs, LOG_ERROR), "Erreur dans %s(); : erreur d'allocation mémoire de vert_cursor.\n", __func__);
    game_config_sdl_free ((void**)&game_config);
    return NULL;
  }

  config_cursor_set_type (game_config->vert_cursor, VERTICAL);
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

  config_cursor_set_type (game_config->horiz_cursor, HORIZONTAL);
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

  /* Calcul de la position actuelle des deux curseurs */
//  SDL_Rect column_position = (SDL_Rect){widget_size.x + 35 + (14 * (game_config->columns-3)), widget_size.y - 10, 14, 14};
//  SDL_Rect line_position = (SDL_Rect){widget_size.x - 10, widget_size.y + 35 + (14 * (game_config->lines-3)), 14, 14};

//  /* Création de la texture du curseur */
//  texture = SDL_CreateTextureFromSurface(widget_sdl_get_renderer (game_config->widget), game_config->surface);
//
//  /* Affichage des deux curseurs */
//  SDL_RenderCopy(widget_sdl_get_renderer (game_config->widget), texture, NULL, &column_position);
//  SDL_RenderCopy(widget_sdl_get_renderer (game_config->widget), texture, NULL, &line_position);

  SDL_DestroyTexture (texture);
}


