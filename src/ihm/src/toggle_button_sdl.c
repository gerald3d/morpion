#include "toggle_button_sdl.h"

static void toggle_button_sdl_update (t_widget_sdl *widget, void *userdata);
static void toggle_button_sdl_clic_callback (t_state_button_sdl *button, void *userdata);

t_toggle_button_sdl*
toggle_button_sdl_new (SDL_Rect size, t_logs *logs) {
  t_toggle_button_sdl *toggle_button = malloc (sizeof (t_toggle_button_sdl));
  if (toggle_button == NULL)
    return NULL;

  toggle_button->widget = widget_sdl_new (logs);
  if (toggle_button->widget == NULL) {
    toggle_button_sdl_free ((void**)toggle_button);
    return NULL;
  }

  /* Affectation du pointeur de l'objet à l'objet parent */
  toggle_button->widget->widget_child = toggle_button;

  /* Affectation de la fonction de destruction du toggle button au widget */
  toggle_button->widget->destroy_widget_child_fct = toggle_button_sdl_free;

  widget_sdl_set_size (toggle_button->widget, &size.x, &size.y, &size.w, &size.h);

  /* Mise en transparence du widget */
  widget_sdl_set_color (toggle_button->widget, (SDL_Color){255, 255, 255, 0}, FOND);
  widget_sdl_set_color (toggle_button->widget, (SDL_Color){0, 0, 0, 0}, ACTIVE);

  SDL_Rect button1_size = (SDL_Rect){size.x, size.y, size.w/2, size.h};
  toggle_button->button1 = state_button_sdl_new (button1_size, logs);
  if (toggle_button->button1 == NULL) {
  	t_widget_sdl *widget = state_button_sdl_get_widget (toggle_button->button1);
    widget_sdl_free(&widget);

    return NULL;
  }

  /* Activation par défaut du bouton1 */
  state_button_sdl_set_active (toggle_button->button1, true);

  SDL_Rect button2_size = (SDL_Rect){size.x+size.w/2, size.y, size.w/2, size.h};
  toggle_button->button2 = state_button_sdl_new (button2_size, logs);
  if (toggle_button->button2 == NULL) {
		t_widget_sdl *widget = state_button_sdl_get_widget(toggle_button->button1);
    widget_sdl_free(&widget);

    toggle_button_sdl_free ((void**)toggle_button);
    return NULL;
  }

  /* Attachement d'une donnée personnelle à chaque bouton pour permettre de les reconnaître
   * dans la fonction callback */
  toggle_button->num1 = BUTTON1;
  toggle_button->num2 = BUTTON2;
  widget_sdl_set_user_data (state_button_sdl_get_widget (toggle_button->button1), "numero", &toggle_button->num1);
  widget_sdl_set_user_data (state_button_sdl_get_widget (toggle_button->button2), "numero", &toggle_button->num2);

  /* Ajout des deux boutons dans la liste des widgets enfants */
  widget_sdl_add_child_widget (toggle_button->widget, state_button_sdl_get_widget (toggle_button->button1));
  widget_sdl_add_child_widget (toggle_button->widget, state_button_sdl_get_widget (toggle_button->button2));

  /* Affectation du callback pour les boutons. Il gère la bascule entre les deux */
  widget_sdl_set_mouse_clic_callback (state_button_sdl_get_widget (toggle_button->button1),
				      toggle_button_sdl_clic_callback, toggle_button);
  widget_sdl_set_mouse_clic_callback (state_button_sdl_get_widget (toggle_button->button2),
				      toggle_button_sdl_clic_callback, toggle_button);

  /* Affectation du callback du dessin de la texture au widget parent */
  widget_sdl_set_callback_create_texture (toggle_button->widget, toggle_button_sdl_update, toggle_button);

  toggle_button->button1_is_activated = true;

  return toggle_button;
}

void
toggle_button_sdl_free (void **toggle_button) {
  if (toggle_button == NULL) {
    fprintf (stderr, "Erreur dans %s(); : **toggle_button ne doit pas être NULL.\n", __func__);
    return;
  }

  if (*toggle_button == NULL) {
    fprintf (stderr, "Erreur dans %s(); : toggle_button ne doit pas être NULL.\n", __func__);
    return;
  }

	t_toggle_button_sdl *intern_toggle= *toggle_button;
  if (intern_toggle->button1) {
  	t_widget_sdl *widget = state_button_sdl_get_widget(intern_toggle->button1);
    widget_sdl_free(&widget);
  }

  if (intern_toggle->button2) {
		t_widget_sdl *widget = state_button_sdl_get_widget(intern_toggle->button2);
    widget_sdl_free(&widget);
  }

  free (intern_toggle);
  intern_toggle = NULL;
}

t_state_button_sdl*
toggle_button_sdl_get_state_button (t_toggle_button_sdl *toggle_button, TOGGLE_BUTTON_NUMBER button_number) {
  if (toggle_button == NULL) {
    fprintf (stderr, "Erreur dans %s(); : toggle_button ne doit pas être NULL.\n", __func__);
    return NULL;
  }

  if (button_number == BUTTON1)
    return toggle_button->button1;

  if (button_number == BUTTON2)
    return toggle_button->button2;

  return NULL;
}

t_widget_sdl*
toggle_button_sdl_get_widget (t_toggle_button_sdl *toggle_button) {
  if (toggle_button == NULL) {
    fprintf (stderr, "Erreur dans %s(); : toggle_button ne doit pas être NULL.\n", __func__);
    return NULL;
  }

 return toggle_button->widget;
}

static void
toggle_button_sdl_clic_callback (t_state_button_sdl *button, void *userdata) {
 if (button == NULL) {
    fprintf (stderr, "Erreur dans %s(); : button ne doit pas être NULL.\n", __func__);
    return;
  }

  if (userdata == NULL) {
    fprintf (stderr, "Erreur dans %s(); : userdata ne doit pas être NULL.\n", __func__);
    return;
  }

  /* Récupération de la donnée personnelle du bouton pour savoir son numéro */
  TOGGLE_BUTTON_NUMBER *num = (TOGGLE_BUTTON_NUMBER*)widget_sdl_get_user_data (state_button_sdl_get_widget (button), "numero");

  /* Récupération du toggle_button */
  t_toggle_button_sdl *toggle_button = (t_toggle_button_sdl*)userdata;

  if (*num == BUTTON1) {
    /* Si le bouton1 est déjà actif on ne change rien */
    if (toggle_button->button1_is_activated == false)
      toggle_button->button1_is_activated = true;

    /* Mise à jour de l'état des deux boutons */
    state_button_sdl_set_active (toggle_button->button1, true);
    state_button_sdl_set_active (toggle_button->button2, false);
    return;
  }

  if (*num == BUTTON2) {
    /* Si le bouton2 est déjà actif on ne change rien */
    if (toggle_button->button1_is_activated == true)
      toggle_button->button1_is_activated = false;

    /* Mise à jour de l'état des deux boutons */
    state_button_sdl_set_active (toggle_button->button1, false);
    state_button_sdl_set_active (toggle_button->button2, true);
  }
}

bool
toggle_button_sdl_button1_is_active (t_toggle_button_sdl *toggle_button) {
  if (toggle_button == NULL) {
    fprintf (stderr, "Erreur dans %s(); : toggle_button ne doit pas être NULL.\n", __func__);
    return false;
  }

  return toggle_button->button1_is_activated;
}

/**************************/

static void
toggle_button_sdl_update (t_widget_sdl *widget, void *userdata) {
  if (widget == NULL) {
    fprintf (stderr, "Erreur dans %s(); : widget ne doit pas être NULL.\n", __func__);
    return;
  }

  if (userdata == NULL) {
    fprintf (stderr, "Erreur dans %s(); : userdata ne doit pas être NULL.\n", __func__);
    return;
  }

  /* Affichage des widgets internes */

}


