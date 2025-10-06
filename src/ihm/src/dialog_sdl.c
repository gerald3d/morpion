#include "dialog_sdl.h"

static void dialog_sdl_update (t_widget_sdl *widget, void *userdata);

t_dialog_sdl*
dialog_sdl_new (SDL_Rect size, t_logs *logs) {
  /* Tentative d'allocation d'un nouveau t_dialog_sdl */
  t_dialog_sdl *dialog = malloc (sizeof(t_dialog_sdl));

  if (dialog == NULL) {
    fprintf (stderr, "Erreur dans %s(); : erreur d'allocation mémoire de dialog.\n", __func__);
    return NULL;
  }

  dialog->widget = widget_sdl_new (logs);
  if (dialog->widget == NULL) {
    dialog_sdl_free ((void**)dialog);
    return NULL;
  }

  /* Affectation du pointeur de l'objet à l'objet parent */
  dialog->widget->widget_child = dialog;

  /* Affectation de la fonction de destruction du dialog au widget */
  dialog->widget->destroy_widget_child_fct = dialog_sdl_free;

  widget_sdl_set_size (dialog->widget, &size.x, &size.y, &size.w, &size.h);

  /* Rendre la fenêtre de dialogue modale */
   widget_sdl_set_modale (dialog->widget, true);

  /* Changement de la couleur de fond */
  widget_sdl_set_color (dialog->widget, (SDL_Color){231, 226, 226, 255}, FOND);

  dialog->title = NULL;
  dialog->surface = NULL;

  /* Ajout du bouton de fermeture en haut à droite */
  dialog->close_button = button_sdl_new (IMAGE, NULL, (SDL_Rect){size.x + size.w - 30, size.y, 30, 30}, logs);
  if (dialog->close_button == NULL) {
    dialog_sdl_free ((void**)dialog);
    return NULL;
  }

  /* Affectation d'une image pour le bouton de fermeture */
  button_sdl_set_image_from_file (dialog->close_button,	"images/croix_noire.png");

  /* Change le curseur lorsque la souris est sur le bouton de fermeture */
  widget_sdl_set_cursor_from_file (button_sdl_get_widget (dialog->close_button), cursor_sdl);

  /* Ajout d'une bulle d'aide au bouton de fermeture */
  widget_sdl_set_tooltip (button_sdl_get_widget (dialog->close_button), "Fermeture");

  widget_sdl_add_child_widget (dialog->widget, button_sdl_get_widget (dialog->close_button));

  /* Chargement de la police */
  dialog->font = TTF_OpenFont(font_sdl, 20);
  if (dialog->font == NULL) {
    fprintf (stderr, "Erreur dans %s (); : %s\n", __func__, SDL_GetError());
    dialog_sdl_free ((void**)dialog);
    return NULL;
  }

  /* Ajout du bouton d'annulation  */
  dialog->annul_button = button_sdl_new (TEXTE, dialog->font, (SDL_Rect){size.x + size.w - 170, size.y + size.h - 30, 80, 20}, logs);
  if (dialog->annul_button == NULL) {
    dialog_sdl_free ((void**)dialog);
    return NULL;
  }

  /* Texte du bouton */
  button_sdl_set_text (dialog->annul_button, "Annuler", TTF_STYLE_NORMAL);

  /* Change le curseur lorsque la souris est sur le bouton */
  widget_sdl_set_cursor_from_file (button_sdl_get_widget (dialog->annul_button), cursor_sdl);

  widget_sdl_add_child_widget (dialog->widget, button_sdl_get_widget (dialog->annul_button));

  /* Ajout du bouton Valider  */
  dialog->valid_button = button_sdl_new (TEXTE, dialog->font, (SDL_Rect){size.x + size.w - 85, size.y + size.h - 30, 80, 20}, logs);
  if (dialog->valid_button == NULL) {
    dialog_sdl_free ((void**)dialog);
    return NULL;
  }

  /* Texte du bouton */
  button_sdl_set_text (dialog->valid_button, "Valider", TTF_STYLE_NORMAL);

  /* Change le curseur lorsque la souris est sur le bouton */
  widget_sdl_set_cursor_from_file (button_sdl_get_widget (dialog->valid_button), "images/cursor.png");

  /* Ajout d'une bulle d'aide au bouton */
  widget_sdl_set_tooltip (button_sdl_get_widget (dialog->valid_button), "La partie en cours sera réinitialisée");

  widget_sdl_add_child_widget (dialog->widget, button_sdl_get_widget (dialog->valid_button));

  /* Affectation du callback du dessin de la texture au widget parent */
  widget_sdl_set_callback_create_texture (dialog->widget, dialog_sdl_update, dialog);

  return dialog;
}

void
dialog_sdl_free (void **dialog) {
  if (dialog == NULL) {
    fprintf (stderr, "Erreur dans %s(); : **dialog ne doit pas être NULL.\n", __func__);
    return;
  }

  if (*dialog == NULL) {
    fprintf (stderr, "Erreur dans %s(); : dialog ne doit pas être NULL.\n", __func__);
    return;
  }

  t_dialog_sdl *dial = *dialog;

  if (dial->surface)
    SDL_FreeSurface (dial->surface);

  if (dial->close_button) {
		t_widget_sdl *widget = button_sdl_get_widget(dial->close_button);
    widget_sdl_free(&widget);
  }

  if (dial->valid_button) {
    t_widget_sdl *widget = button_sdl_get_widget(dial->valid_button);
    widget_sdl_free(&widget);
  }

  if (dial->annul_button) {
  	t_widget_sdl *widget = button_sdl_get_widget(dial->annul_button);
    widget_sdl_free(&widget);
  }

  if (dial->font)
    TTF_CloseFont(dial->font);

  if (dial->title)
    free (dial->title);

  free (dial);

  dial = NULL;
}

t_widget_sdl*
dialog_sdl_get_widget (t_dialog_sdl *dialog) {
  if (dialog == NULL) {
    fprintf (stderr, "Erreur dans %s(); : dialog ne doit pas être NULL.\n", __func__);
    return NULL;
  }

 return dialog->widget;
}

void
dialog_sdl_set_title (t_dialog_sdl *dialog, const char *title) {
  if (dialog == NULL) {
    fprintf (stderr, "Erreur dans %s(); : dialog ne doit pas être NULL.\n", __func__);
    return;
  }

  if (dialog->title) {
    free (dialog->title);
    dialog->title = NULL;
  }

  if (title == NULL)
    return;

  dialog->title = strdup (title);
}

t_button_sdl*
dialog_sdl_get_close_button  (t_dialog_sdl *dialog) {
  if (dialog == NULL) {
    fprintf (stderr, "Erreur dans %s(); : dialog ne doit pas être NULL.\n", __func__);
    return NULL;
  }

  return dialog->close_button;
}

t_button_sdl*
dialog_sdl_get_annul_button  (t_dialog_sdl *dialog) {
  if (dialog == NULL) {
    fprintf (stderr, "Erreur dans %s(); : dialog ne doit pas être NULL.\n", __func__);
    return NULL;
  }

  return dialog->annul_button;
}

t_button_sdl*
dialog_sdl_get_valid_button  (t_dialog_sdl *dialog) {
  if (dialog == NULL) {
    fprintf (stderr, "Erreur dans %s(); : dialog ne doit pas être NULL.\n", __func__);
    return NULL;
  }

  return dialog->valid_button;
}

/*------------------------------------------------------------------*/

static void
dialog_sdl_surface_new (t_dialog_sdl *dialog) {
  if (dialog==NULL) {
    fprintf (stderr, "Erreur dans %s(); : dialog ne doit pas être NULL.\n", __func__);
    return;
  }

  if (dialog->surface)
    SDL_FreeSurface(dialog->surface);

  TTF_SetFontStyle(dialog->font, TTF_STYLE_NORMAL);

  /* Récupération de la taille de la fenêtre de dialogue */
  SDL_Rect size;
  widget_sdl_get_size (dialog->widget, &size);

  dialog->surface = TTF_RenderUTF8_Blended_Wrapped(dialog->font, dialog->title, (SDL_Color){0, 0, 0, 255}, size.w - 10);
}

static SDL_Texture*
dialog_sdl_create_texture (t_dialog_sdl *dialog) {
  if (dialog==NULL) {
    fprintf (stderr, "Erreur dans %s(); : dialog ne doit pas être NULL.\n", __func__);
    return NULL;
  }

  return SDL_CreateTextureFromSurface(widget_sdl_get_renderer (dialog->widget), dialog->surface);
}

static void
dialog_sdl_update (t_widget_sdl *widget, void *userdata) {
  if (widget == NULL) {
    fprintf (stderr, "Erreur dans %s(); : widget ne doit pas être NULL.\n", __func__);
    return;
  }

  if (userdata == NULL) {
    fprintf (stderr, "Erreur dans %s(); : userdata ne doit pas être NULL.\n", __func__);
    return;
  }

  t_dialog_sdl *dialog = (t_dialog_sdl*)userdata;

  SDL_Color color = dialog->widget->couleur_fond;;

  /* Remplissage du fond de la boite de dialogue */
  SDL_SetRenderDrawColor(dialog->widget->renderer, color.r, color.g, color.b, color.a);
  SDL_RenderFillRect(widget_sdl_get_renderer (dialog->widget), &dialog->widget->actual_size);

  /* Mise à jour de la surface "title" */
  dialog_sdl_surface_new (dialog);

  /* Récupération de la texture du texte et en même temps de sa taille */
  SDL_Texture *texture = dialog_sdl_create_texture (dialog);
  if (texture == false)
    return;

  /* Récupération de la taille de la fenêtre de dialogue */
  SDL_Rect diag_size;
  widget_sdl_get_size (dialog->widget, &diag_size);

  /* Récupération de la taille de la texture */
  SDL_Rect texture_size;
  SDL_QueryTexture (texture, NULL, NULL, &texture_size.w, &texture_size.h);

  /* Positionnement du titre */
  texture_size.x = diag_size.x + diag_size.w / 2 - texture_size.w / 2;
  texture_size.y = diag_size.y + 30;

  SDL_RenderCopy(widget_sdl_get_renderer (dialog->widget), texture, NULL, &texture_size);

  SDL_DestroyTexture (texture);
}
