#include "state_button_sdl.h"

static SDL_Texture *state_button_sdl_create_texture (t_state_button_sdl *state_button, SDL_Renderer *renderer);
static void state_button_changed_cb (t_state_button_sdl *state_button, void *userdata);

t_state_button_sdl*
state_button_sdl_new (SDL_Rect size, t_logs *logs) {
  t_state_button_sdl *state_button = malloc (sizeof(t_state_button_sdl));

  if (state_button == NULL) {
		FILE *error = logs_descripteur_fichier (logs, LOG_ERROR);
    fprintf (error, "Erreur dans %s(); : erreur d'allocation mémoire de *state_button.\n", __func__);
    return NULL;
  }

  state_button->widget = widget_sdl_new (logs);
  if (state_button->widget == NULL) {
    state_button_sdl_free ((void**)state_button);
    return NULL;
  }

  /* Affectation du pointeur de l'objet à l'objet parent */
  state_button->widget->widget_child = state_button;

  /* Affectation de la fonction de destruction du state button au widget */
  state_button->widget->destroy_widget_child_fct = state_button_sdl_free;

  widget_sdl_set_size (state_button->widget, &size.x, &size.y, &size.w, &size.h);

	state_button->state = false;
  state_button->image = NULL;

  /* Affectation du callback du dessin de la texture au widget parent */
  widget_sdl_set_callback_create_texture (state_button->widget, state_button_sdl_update, state_button);

  /* Affectation du callback lors du clic de la souris */
  widget_sdl_set_mouse_clic_callback (state_button->widget, state_button_changed_cb, NULL);

  return state_button;
}

void
state_button_sdl_free (void **state_button) {
  if (state_button == NULL) {
    fprintf (stderr, "Erreur dans %s(); : **state_button ne doit pas être NULL.\n", __func__);
    return;
  }

  if (*state_button == NULL) {
    fprintf (stderr, "Erreur dans %s(); : state_button ne doit pas être NULL.\n", __func__);
    return;
  }

	t_state_button_sdl *intern_button= *state_button;
  if (intern_button->image)
    SDL_FreeSurface (intern_button->image);

  free (intern_button);

  intern_button = NULL;
}

t_widget_sdl*
state_button_sdl_get_widget (t_state_button_sdl *state_button) {
  if (state_button == NULL) {
    fprintf (stderr, "Erreur dans %s(); : state_button ne doit pas être NULL.\n", __func__);
    return NULL;
  }

  return state_button->widget;
}

bool
state_button_sdl_set_image (t_state_button_sdl *state_button, const char *file) {
  if (state_button == NULL) {
    fprintf (stderr, "Erreur dans %s(); : state_button ne doit pas être NULL.\n", __func__);
    return false;
  }

  state_button->image = IMG_Load(file);

  if(state_button->image == NULL) {
    fprintf (state_button->widget->file_error, "Erreur dans %s(); : %s\n", __func__, SDL_GetError());
    return false;
  }

  return true;
}

bool
state_button_sdl_is_active (t_state_button_sdl *state_button) {
  if (state_button == NULL) {
    fprintf (stderr, "Erreur dans %s(); : state_button ne doit pas être NULL.\n", __func__);
    return false;
  }

 return state_button->state;
}

void
state_button_sdl_set_active (t_state_button_sdl *state_button, bool state) {
  if (state_button == NULL) {
    fprintf (stderr, "Erreur dans %s(); : state_button ne doit pas être NULL.\n", __func__);
    return;
  }

  state_button->state = state;
}


/*------------------------------------------------------------------*/

static void
state_button_changed_cb (t_state_button_sdl *state_button, void *userdata) {
  /* Inversion de l'état du booléen state */
  state_button->state = !state_button->state;
}

static SDL_Texture*
state_button_sdl_create_texture (t_state_button_sdl *state_button, SDL_Renderer *renderer) {
  if (state_button == NULL) {
    fprintf (stderr, "Erreur dans %s(); : button ne doit pas être NULL.\n", __func__);
    return NULL;
  }

  if (renderer == NULL) {
    fprintf (state_button->widget->file_error, "Erreur dans %s(); : renderer ne doit pas être NULL.\n", __func__);
    return NULL;
  }

  SDL_Texture *texture;

  texture = SDL_CreateTextureFromSurface(widget_sdl_get_renderer (state_button->widget), state_button->image);

  return texture;
}

void
state_button_sdl_update (t_widget_sdl *widget, void *userdata) {
  if (widget == NULL) {
    fprintf (stderr, "Erreur dans %s(); : widget ne doit pas être NULL.\n", __func__);
    return;
  }

  if (userdata == NULL) {
    fprintf (stderr, "Erreur dans %s(); : userdata ne doit pas être NULL.\n", __func__);
    return;
  }

  t_state_button_sdl *state_button = (t_state_button_sdl*)userdata;

  /* Si le bouton est enfoncé changement de la couleur de fond du bouton en blanc */
  SDL_Color color;
  /* printf ("etat : %d\n", state_button->state); */
  if (state_button_sdl_is_active (state_button))
    color = state_button->widget->couleur_active;
  else
    color = state_button->widget->couleur_fond;

  /* Remplissage du fond du bouton */
  SDL_SetRenderDrawColor(state_button->widget->renderer, color.r, color.g, color.b, color.a);
  SDL_RenderFillRect(widget_sdl_get_renderer (state_button->widget), &state_button->widget->actual_size);

  /* Création de la texture */
  SDL_Texture *texture = state_button_sdl_create_texture (state_button, widget_sdl_get_renderer (widget));

  /* Récupération de la texture du texte ou de l'image */
  if (texture == NULL) {
    fprintf (state_button->widget->file_error, "Erreur dans %s(); : %s\n", __func__, SDL_GetError());
    return;
  }

  /* Remplissage du bouton */
  SDL_Rect widget_size;
  widget_sdl_get_size (widget, &widget_size);

  // Affichage de l'image
  SDL_RenderCopy(widget_sdl_get_renderer (widget), texture, NULL, &widget_size);

  SDL_DestroyTexture (texture);
}


