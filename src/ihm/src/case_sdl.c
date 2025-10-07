#include "case_sdl.h"

/* static void case_sdl_text_surface_new (t_case_sdl *case_sdl); */
static SDL_Texture *case_sdl_create_texture (t_case_sdl *case_sdl, SDL_Renderer *renderer);
/* Affiche le bouton sur renderer transmis */
static void case_sdl_update (t_widget_sdl *widget, void *userdata);

t_case_sdl*
case_sdl_new (SDL_Rect size, t_logs *logs) {
  t_case_sdl *case_sdl = malloc (sizeof(t_case_sdl));

  if (case_sdl == NULL) {
		FILE *error = logs_descripteur_fichier (logs, LOG_ERROR);
    fprintf (error, "Erreur dans %s(); : erreur d'allocation mémoire de *case.\n", __func__);
    return NULL;
  }

  case_sdl->widget = widget_sdl_new (logs);
  if (case_sdl->widget == NULL) {
    case_sdl_free ((void**)&case_sdl);
    return NULL;
  }

  /* Affectation du pointeur de l'objet à l'objet parent */
  case_sdl->widget->widget_child = case_sdl;

  /* Affectation de la fonction de destruction de la case au widget */
  case_sdl->widget->destroy_widget_child_fct = case_sdl_free;

  case_sdl->image = NULL;

  widget_sdl_set_size (case_sdl->widget, &size.x, &size.y, &size.w, &size.h);

  /* Affectation du callback du dessin de la texture au widget parent */
  widget_sdl_set_callback_create_texture (case_sdl->widget, case_sdl_update, case_sdl);

  return case_sdl;
}

void
case_sdl_free (void **case_sdl) {
  if (case_sdl == NULL) {
    fprintf (stderr, "Erreur dans %s(); : **case ne doit pas être NULL.\n", __func__);
    return;
  }

  if (*case_sdl == NULL) {
    fprintf (stderr, "Erreur dans %s(); : case ne doit pas être NULL.\n", __func__);
    return;
  }

	t_case_sdl *intern_case= *case_sdl;
  if (intern_case->image)
    SDL_FreeSurface (intern_case->image);

  free (intern_case);

  intern_case = NULL;
}

t_widget_sdl*
case_sdl_get_widget (t_case_sdl *case_sdl) {
  if (case_sdl == NULL) {
    fprintf (stderr, "Erreur dans %s(); : case ne doit pas être NULL.\n", __func__);
    return NULL;
  }

 return case_sdl->widget;
}

bool
case_sdl_set_image (t_case_sdl *case_sdl, SDL_Surface *image) {
  if (case_sdl == NULL) {
    fprintf (stderr, "Erreur dans %s(); : case ne doit pas être NULL.\n", __func__);
    return false;
  }

  case_sdl->image = image;

  return true;
}

/*------------------------------------------------------------------*/

static SDL_Texture*
case_sdl_create_texture (t_case_sdl *case_sdl, SDL_Renderer *renderer) {
  if (case_sdl == NULL) {
    fprintf (stderr, "Erreur dans %s(); : case ne doit pas être NULL.\n", __func__);
    return NULL;
  }

  if (renderer == NULL) {
    fprintf (case_sdl->widget->file_error, "Erreur dans %s(); : renderer ne doit pas être NULL.\n", __func__);
    return NULL;
  }

  if (case_sdl == NULL)
    return NULL;

  SDL_Texture *texture;
  texture = SDL_CreateTextureFromSurface(widget_sdl_get_renderer (case_sdl->widget), case_sdl->image);

  return texture;
}

static void
case_sdl_update (t_widget_sdl *widget, void *userdata) {
  if (widget == NULL) {
    fprintf (stderr, "Erreur dans %s(); : widget ne doit pas être NULL.\n", __func__);
    return;
  }

  if (userdata == NULL) {
    fprintf (widget->file_error, "Erreur dans %s(); : userdata ne doit pas être NULL.\n", __func__);
    return;
  }

  t_case_sdl *case_sdl = (t_case_sdl*)userdata;

    SDL_Color color;
  if (widget_sdl_mouse_on (case_sdl->widget) == true)
    color = case_sdl->widget->couleur_active;
  else
    color = case_sdl->widget->couleur_fond;

  /* Remplissage du fond du bouton */
  SDL_SetRenderDrawColor(case_sdl->widget->renderer, color.r, color.g, color.b, color.a);
  SDL_RenderFillRect(widget_sdl_get_renderer (case_sdl->widget), &case_sdl->widget->actual_size);

  SDL_Texture *texture = case_sdl_create_texture (case_sdl, widget_sdl_get_renderer (widget));

  /* Récupération de la texture du texte ou de l'image */
  if (texture == NULL)
    return;

  /* Remplissage du bouton */
  SDL_Rect widget_size;
  widget_sdl_get_size (widget, &widget_size);

  // Affichage de l'image
  SDL_RenderCopy(widget_sdl_get_renderer (widget), texture, NULL, &widget_size);

  SDL_DestroyTexture (texture);
}


