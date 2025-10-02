#include "button_sdl.h"

static void button_sdl_text_surface_new (t_button_sdl *button);
static SDL_Texture *button_sdl_create_texture (t_button_sdl *button, SDL_Renderer *renderer);
/* Affiche le bouton sur renderer transmis */
static void button_sdl_update (t_widget_sdl *widget, void *userdata);


t_button_sdl*
button_sdl_new (BUTTON_SDL_TYPE_OF_BUTTON type_de_bouton, TTF_Font *font, SDL_Rect size) {
  /* Tentative d'allocation d'un nouveau t_button_sdl */
  t_button_sdl *button = malloc (sizeof(t_button_sdl));

  if (button == NULL) {
    fprintf (stderr, "Erreur dans %s(); : erreur d'allocation mémoire de *button.\n", __func__);
    return NULL;
  }

  button->widget = widget_sdl_new ();
  if (button->widget == NULL) {
    button_sdl_free ((void**)button);
    return NULL;
  }

  /* Affectation du pointeur de l'objet à l'objet parent */
  button->widget->widget_child = button;

  /* Affectation de la fonction de destruction du bouton au widget */
  button->widget->destroy_widget_child_fct = button_sdl_free;

  widget_sdl_set_size (button->widget, &size.x, &size.y, &size.w, &size.h);

  button->font = font;
  button->style = TTF_STYLE_NORMAL;

  if (type_de_bouton == IMAGE || type_de_bouton == TEXTE)
    button->button_type = type_de_bouton;
  else {
    fprintf (stderr, "Erreur dans %s(); : type__de_bouton doit avoir la valeur TEXTE ou IMAGE.\n", __func__);
    button_sdl_free ((void**)button);
    return NULL;
  }

  if (type_de_bouton == TEXTE && font == NULL) {
    fprintf (stderr, "Erreur dans %s(); : font ne doit pas être NULL.\n", __func__);
    button_sdl_free ((void**)button);
    return NULL;
  }

  button->text = NULL;
  button->image = NULL;
  button->surface = NULL;
  button->on_activate = NULL;
  button->userdata = NULL;

  button->couleur_text = (SDL_Color){0, 0, 0, 255};

  /* Affectation du callback du dessin de la texture au widget parent */
  widget_sdl_set_callback_create_texture (button->widget, button_sdl_update, button);

  /* Affectation du callback lorsque la souris est sur le bouton. Permet de changer le style de la font */
  /* widget_sdl_set_mouse_on_callback (button->widget, button_sdl_mouse_in, NULL); */

  return button;
}

void
button_sdl_free (void **button) {
  if (button == NULL) {
    fprintf (stderr, "Erreur dans %s(); : **button ne doit pas être NULL.\n", __func__);
    return;
  }

  if (*button == NULL) {
    fprintf (stderr, "Erreur dans %s(); : button ne doit pas être NULL.\n", __func__);
    return;
  }

  t_button_sdl *intern_button = *button;
//  if (intern_button->widget)
//    widget_sdl_free (&intern_button->widget);

  if (intern_button->text)
    free (intern_button->text);

  if (intern_button->surface)
    SDL_FreeSurface (intern_button->surface);

  if (intern_button->image)
    SDL_FreeSurface (intern_button->image);

  free (intern_button);

  intern_button = NULL;
}

t_widget_sdl*
button_sdl_get_widget (t_button_sdl *button) {
  if (button == NULL) {
    fprintf (stderr, "Erreur dans %s(); : button ne doit pas être NULL.\n", __func__);
    return NULL;
  }

 return button->widget;
}

bool
button_sdl_set_image_from_file (t_button_sdl *button, const char *file) {
  if (button == NULL) {
    fprintf (stderr, "Erreur dans %s(); : button ne doit pas être NULL.\n", __func__);
    return false;
  }

  if (file == NULL) {
    fprintf (stderr, "Erreur dans %s(); : file ne doit pas être NULL.\n", __func__);
    return false;
  }

  if (button->image)
    SDL_FreeSurface (button->image);

  button->image = IMG_Load(file);
  if(button->image==NULL) {
    fprintf (stderr, "Erreur dans %s(); : %s\n", __func__, SDL_GetError());
    return false;
  }

  return true;
}

bool
button_sdl_set_image_from_surface (t_button_sdl *button, SDL_Surface *image) {
  if (button == NULL) {
    fprintf (stderr, "Erreur dans %s(); : button ne doit pas être NULL.\n", __func__);
    return false;
  }

  if (image == NULL) {
    fprintf (stderr, "Erreur dans %s(); : image ne doit pas être NULL.\n", __func__);
    return false;
  }

  if (button->image)
    SDL_FreeSurface (button->image);

  button->image = image;
  if(button->image==NULL) {
    fprintf (stderr, "Erreur dans %s(); : %s\n", __func__, SDL_GetError());
    return false;
  }

  return true;
}

void
button_sdl_set_text (t_button_sdl *button, const char *text, int style) {
  if (button == NULL) {
    fprintf (stderr, "Erreur dans %s(); : button ne doit pas être NULL.\n", __func__);
    return;
  }

  if (button->button_type == IMAGE) {
    fprintf (stderr, "Erreur dans %s(); : button est du type IMAGE.\n", __func__);
    return;
  }

  if (button->text)
    free (button->text);

  if (text == NULL)
    button->text = strdup (" ");
  else
    button->text = strdup (text);

  button->style = style;

  button_sdl_text_surface_new (button);
}

const char*
button_sdl_get_text (t_button_sdl *button) {
  if (button == NULL) {
    fprintf (stderr, "Erreur dans %s(); : button ne doit pas être NULL.\n", __func__);
    return NULL;
  }

  if (button->button_type == IMAGE) {
    fprintf (stderr, "Erreur dans %s(); : button est du type IMAGE.\n", __func__);
    return NULL;
  }

  return button->text;
}

/*------------------------------------------------------------------*/

static SDL_Texture*
button_sdl_create_texture (t_button_sdl *button, SDL_Renderer *renderer) {
  if (button == NULL) {
    fprintf (stderr, "Erreur dans %s(); : button ne doit pas être NULL.\n", __func__);
    return NULL;
  }

  if (renderer == NULL) {
    fprintf (stderr, "Erreur dans %s(); : renderer ne doit pas être NULL.\n", __func__);
    return NULL;
  }

  SDL_Texture *texture;

  if (button->button_type == TEXTE) {
    /* Mise à jour du texte dans le bouton */
    button_sdl_text_surface_new (button);
    texture = SDL_CreateTextureFromSurface(widget_sdl_get_renderer (button->widget), button->surface);
  } else // Mise à jour de l'image
    texture = SDL_CreateTextureFromSurface(widget_sdl_get_renderer (button->widget), button->image);

  return texture;
}

static void
button_sdl_text_surface_new (t_button_sdl *button) {
  if (button == NULL) {
    fprintf (stderr, "Erreur dans %s(); : button ne doit pas être NULL.\n", __func__);
    return;
  }

  if (button->surface)
    SDL_FreeSurface(button->surface);

  TTF_SetFontStyle(button->font, button->style);
  if (widget_sdl_is_sensitive (button->widget) == true)
    button->surface = TTF_RenderUTF8_Blended(button->font, button->text, button->couleur_text);
  else
    button->surface = TTF_RenderUTF8_Blended(button->font, button->text, button->widget->couleur_insensible);
}

static void
button_sdl_update (t_widget_sdl *widget, void *userdata) {
  if (widget == NULL) {
    fprintf (stderr, "Erreur dans %s(); : widget ne doit pas être NULL.\n", __func__);
    return;
  }

  if (userdata == NULL) {
    fprintf (stderr, "Erreur dans %s(); : userdata ne doit pas être NULL.\n", __func__);
    return;
  }

  t_button_sdl *button = (t_button_sdl*)userdata;

  SDL_Color color;
  if (widget_sdl_mouse_on (button->widget) == true) {
    button->style = TTF_STYLE_ITALIC;
    color = button->widget->couleur_active;
  }
  else {
    button->style = TTF_STYLE_NORMAL;
    color = button->widget->couleur_fond;
  }

  /* Remplissage du fond du bouton */
  SDL_SetRenderDrawColor(button->widget->renderer, color.r, color.g, color.b, color.a);
  SDL_RenderFillRect(widget_sdl_get_renderer (button->widget), &button->widget->actual_size);

  /* Mise à jour de la surface "texte" avec le nouveau style */
  button_sdl_text_surface_new (button);

  SDL_Texture *texture = button_sdl_create_texture (button, widget_sdl_get_renderer (widget));

  /* Récupération de la texture du texte ou de l'image */
  if (texture == NULL)
    return;

  /* Remplissage du bouton */
  SDL_Rect widget_size;
  widget_sdl_get_size (widget, &widget_size);

  if (button->button_type == TEXTE) {
    SDL_Rect size;
    SDL_QueryTexture(texture, NULL, NULL, &size.w, &size.h);

    /* Calcul de la position du texte (centré) dans le cadre */
    size.x = widget_size.x + widget_size.w/2 - size.w/2;
    size.y = widget_size.y + widget_size.h/2 - size.h/2;

    /* Application de la texture dans le rendu */
    SDL_RenderCopy(widget_sdl_get_renderer (widget), texture, NULL, &size);
  } else { // Affichage de l'image
    SDL_RenderCopy(widget_sdl_get_renderer (widget), texture, NULL, &widget_size);
  }

  SDL_DestroyTexture (texture);
}


