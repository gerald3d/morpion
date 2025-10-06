#include "ihm_sdl.h"

t_ihm_sdl*
ihm_sdl_new (SDL_Renderer *renderer) {
  if (renderer == NULL) {
    fprintf (stderr, "Erreur dans %s(); : renderder ne doit pas être NULL.\n", __func__);
    return NULL;
  }

  t_ihm_sdl *ihm_sdl = malloc (sizeof (t_ihm_sdl));

  if (ihm_sdl == NULL) {
    fprintf (stderr, "Erreur dans %s(); : erreur d'allocation mémoire de l'ihm_sdl.\n", __func__);
    return NULL;
  }

  ihm_sdl->renderer = renderer;

  ihm_sdl->logs = logs_init ("ihm_standard_logs.txt", "ihm_error_logs.txt");
  if (ihm_sdl->logs == NULL) {
	  ihm_sdl_free (&ihm_sdl);
	  return NULL;
  }

  ihm_sdl->file_error = logs_descripteur_fichier (ihm_sdl->logs, LOG_ERROR);
  ihm_sdl->widget_list = NULL;
  ihm_sdl->insensible_widgets_list = NULL;

  return ihm_sdl;
}

void
ihm_sdl_free (t_ihm_sdl **ihm_sdl) {
 if (ihm_sdl == NULL) {
    fprintf (stderr, "Erreur dans %s(); : **ihm_sdl ne doit pas être NULL.\n", __func__);
    return;
  }

  if (*ihm_sdl == NULL) {
    fprintf (stderr, "Erreur dans %s(); : ihm_sdl ne doit pas être NULL.\n", __func__);
    return;
  }

  if ((*ihm_sdl)->widget_list) {
		t_liste *liste_tmp = (*ihm_sdl)->widget_list;
		t_widget_sdl *widget = NULL;
		while (liste_tmp) {
			widget = (t_widget_sdl*)liste_tmp->donnee;
			widget_sdl_free(&widget);

			liste_tmp = liste_tmp->suivant;
		}

    liste_liberation (&(*ihm_sdl)->widget_list);
  }

  if ((*ihm_sdl)->insensible_widgets_list)
    liste_liberation (&(*ihm_sdl)->insensible_widgets_list);

	if ((*ihm_sdl)->logs)
		logs_liberation ((*ihm_sdl)->logs);

  free (*ihm_sdl);

  *ihm_sdl = NULL;
}

t_logs*
ihm_sdl_get_logs(t_ihm_sdl *ihm_sdl) {
	if (ihm_sdl == NULL) {
    fprintf (stderr, "Erreur dans %s(); : ihm_sdl ne doit pas être NULL.\n", __func__);
    return NULL;
  }

 return ihm_sdl->logs;
}

SDL_Renderer*
ihm_sdl_get_renderer (t_ihm_sdl *ihm_sdl) {
 if (ihm_sdl == NULL) {
    fprintf (stderr, "Erreur dans %s(); : ihm_sdl ne doit pas être NULL.\n", __func__);
    return NULL;
  }

 return ihm_sdl->renderer;
}

void
ihm_sdl_widget_append (t_ihm_sdl *ihm_sdl, t_widget_sdl *widget) {
  if (ihm_sdl == NULL) {
    fprintf (stderr, "Erreur dans %s(); : ihm_sdl ne doit pas être NULL.\n", __func__);
    return;
  }

  if (widget == NULL) {
    fprintf (ihm_sdl->file_error, "Erreur dans %s(); : widget ne doit pas être NULL.\n", __func__);
    return;
  }

  /* Transmission du renderer au widget */
  widget_sdl_set_renderer (widget, ihm_sdl->renderer);

  /* Ajout du widget à la liste */
  ihm_sdl->widget_list = liste_ajout_fin (ihm_sdl->widget_list, widget);
}

t_liste*
ihm_get_widget_list (t_ihm_sdl *ihm_sdl) {
 if (ihm_sdl == NULL) {
    fprintf (stderr, "Erreur dans %s(); : ihm_sdl ne doit pas être NULL.\n", __func__);
    return NULL;
  }

 return ihm_sdl->widget_list;

}

void
ihm_sdl_set_events (t_ihm_sdl *ihm_sdl, SDL_Event *event) {
  if (ihm_sdl == NULL) {
    fprintf (stderr, "Erreur dans %s(); : ihm_sdl ne doit pas être NULL.\n", __func__);
    return;
  }

  t_liste *liste_tmp = ihm_sdl->widget_list;
  t_widget_sdl *widget;
  while (liste_tmp) {
    widget = (t_widget_sdl*)liste_tmp->donnee;

    if (widget_sdl_is_visible (widget) == true)
      widget_sdl_set_events (widget, event);

    liste_tmp = liste_tmp->suivant;
  }
}

void
ihm_sdl_renderer_update (t_ihm_sdl *ihm_sdl) {
  if (ihm_sdl == NULL) {
    fprintf (stderr, "Erreur dans %s(); : ihm_sdl ne doit pas être NULL.\n", __func__);
    return;
  }

  t_liste *liste_tmp = NULL;
  t_widget_sdl *widget = NULL;
  t_widget_sdl *widget_with_tooltip = NULL;
  t_widget_sdl *modale_widget = NULL;
  t_widget_sdl *widget_cursor_change = NULL;

  /* Recherche si au moins un widget est modale et visible et initialisation du
   * pointeur d'un widget si un curseur particulier doit être affiché */
  liste_tmp = ihm_sdl->widget_list;
  while (liste_tmp) {
    widget = (t_widget_sdl*)liste_tmp->donnee;

    if (widget_sdl_is_visible (widget) == true) {
      if (widget->activate == true && widget->sensitive == true)
				widget_cursor_change = widget;

      if (widget_sdl_is_modale (widget))
				modale_widget = widget;

      /* Si des widgets enfants sont incorporés */
      t_liste *list = NULL;
      if ((list = widget_sdl_get_child_widgets_list (widget))) {
				while (list) {
	  			t_widget_sdl *child = (t_widget_sdl*)list->donnee;

	  			if (child->activate == true) {
	    			widget_cursor_change = child;
	  			}

	  			if (widget_sdl_is_modale (child) && widget_sdl_is_visible (child))
	   				 modale_widget = child;

	 					 list = list->suivant;
				}
      }
    }

    liste_tmp = liste_tmp->suivant;
  }

  /* Si au moins un widget est modale alors désactivation de tous les autres et
     sauvegarde de ces derniers dans une liste chaînée.
     * Cette sauvegarde permet lorsqu'aucun widget est modale de remettre les autres
     * dans leur état d'origine.
     * Si lors de cette vérification la liste des widgets désactivés est déjà
     * initialisée alors il est inuile de les désactiver à nouveau. */
  if (modale_widget) {
    if (ihm_sdl->insensible_widgets_list == NULL) { // C'est la première fois qu'un widget est modale
      liste_tmp = ihm_sdl->widget_list;
      while (liste_tmp) {
				widget = (t_widget_sdl*)liste_tmp->donnee;
				if (widget != modale_widget) {
					/* Désactivation du widget */
					widget_sdl_set_sensitive (widget, false);
					/* Sauvegarde du widget dans la liste */
					ihm_sdl->insensible_widgets_list = liste_ajout_fin (ihm_sdl->insensible_widgets_list, widget);
				}

				liste_tmp = liste_tmp->suivant;
      }
    }
  } else { // Réactivation si ce n'est déjà fait des autres widgets */
    if (ihm_sdl->insensible_widgets_list) {
      t_liste *list = ihm_sdl->insensible_widgets_list;
      t_widget_sdl *widget = NULL;
      while (list) {
				widget = (t_widget_sdl*)list->donnee;
				widget_sdl_set_sensitive (widget, true);
				list = list->suivant;
      }

      /* Libération de la liste */
      liste_liberation (&ihm_sdl->insensible_widgets_list);
    }
  }

  /* Affichage de tous les widgets avec en dernier le widget modale s'il existe */
  liste_tmp = ihm_sdl->widget_list;
  while (liste_tmp) {
    widget = (t_widget_sdl*)liste_tmp->donnee;

    if (widget_sdl_is_visible (widget) == true) {
      /* Si le widget à afficher doit aussi afficher son tooltip le widget sera affiché en dernier */
      if (widget->tooltip && widget->activate)
				widget_with_tooltip = widget;
      else if (widget != modale_widget || modale_widget == NULL) {
				/* Affichage du widget principal */
				widget_sdl_renderer_update (widget);
				/* Si des widgets enfants sont incorporés -> affichage de ces derniers */
				t_liste *list = NULL;
				if ((list = widget_sdl_get_child_widgets_list (widget))) {
					while (list) {
						t_widget_sdl *child = (t_widget_sdl*)list->donnee;

						/* Transmission du renderer au widget */
						widget_sdl_set_renderer (child, ihm_sdl->renderer);
						/* Affichage des widgets enfants */
						widget_sdl_renderer_update (child);
						list = list->suivant;
					}
				}
      }
    }

    liste_tmp = liste_tmp->suivant;
  }

  /* Affichage du widget modale s'il existe */
  if (modale_widget) {
    widget_sdl_renderer_update (modale_widget);

    /* Si des widgets enfants sont incorporés -> affichage de ces derniers */
    t_liste *list = NULL;
    if ((list = widget_sdl_get_child_widgets_list (modale_widget))) {
      while (list) {
				t_widget_sdl *child = (t_widget_sdl*)list->donnee;

				/* Transmission du renderer au widget */
				widget_sdl_set_renderer (child, ihm_sdl->renderer);
				/* Affichage des widgets enfants */
				widget_sdl_renderer_update (child);
				list = list->suivant;
			}
    }
  }

  /* Affichage du widget avec son tooltip au cas où */
  if (widget_with_tooltip && widget_sdl_is_sensitive (widget_with_tooltip)) {
    /* Affichage du widget principal */
    widget_sdl_renderer_update (widget_with_tooltip);

    /* Si des widgets enfants sont incorporés -> affichage de ces derniers */
    t_liste *list = NULL;
    if ((list = widget_sdl_get_child_widgets_list (widget_with_tooltip))) {
      while (list) {
				t_widget_sdl *child = (t_widget_sdl*)list->donnee;
				/* Affichage du child */
				/* Transmission du renderer au widget */
				widget_sdl_set_renderer (child, ihm_sdl->renderer);
				widget_sdl_renderer_update (child);

				list = list->suivant;
      }
    }
  }

  /* Changement de la forme du curseur si la souris est sur un widget */
  if (widget_cursor_change && widget_cursor_change->cursor) {
    int x, y;
    widget_sdl_get_mouse_position (widget_cursor_change, &x, &y);

    /* Désactivation du curseur système */
    SDL_ShowCursor(SDL_DISABLE);

    /* Création de la texture du curseur */
    SDL_Texture *cursor = SDL_CreateTextureFromSurface(ihm_sdl->renderer, widget_cursor_change->cursor);
    if(cursor == NULL) {
      fprintf(ihm_sdl->file_error, "Erreur dans %s(); : problème pour transformer la Surface en Texture : %s\n", __func__, SDL_GetError());
      return;
    }

    /* Récupération de la taille de l'image du curseur */
    SDL_Rect pointer_size = {0, 0, 0, 0};
    SDL_QueryTexture(cursor, NULL, NULL, &pointer_size.w, &pointer_size.h);

    /* Initialisation de la position de départ pour afficher l'image ainsi
       que la taille d'affichage pour la destination */
    SDL_Rect position = {x, y, pointer_size.w, pointer_size.h};

    /* Affichage de l'image en lieu et place du curseur système */
    SDL_RenderCopy(ihm_sdl->renderer, cursor, NULL, &position);

    SDL_DestroyTexture (cursor);
  }
  else { // Affichage du curseur système
    SDL_ShowCursor(SDL_ENABLE);
  }
}
