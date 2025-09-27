#include "tooltip_sdl.h"

static void tooltip_sdl_surface_new (t_tooltip_sdl *tooltip);
static bool tooltip_sdl_create_texture (t_tooltip_sdl *tooltip);

t_tooltip_sdl*
tooltip_sdl_new (const char* text) {
  t_tooltip_sdl *tooltip = malloc (sizeof (t_tooltip_sdl));
  
  if (tooltip==NULL) {
    return NULL;
  }

  tooltip->renderer = NULL;
  tooltip->texture = NULL;
  tooltip->surface = NULL;
  tooltip->text = NULL;
  tooltip->font = TTF_OpenFont(font_sdl, 20);

  if (tooltip->font == NULL) {
    fprintf (stderr, "Erreur dans %s(); : erreur d'allocation de la police : %s\n", __func__, SDL_GetError());
    tooltip_sdl_free (&tooltip);
    return NULL;
  }

  tooltip_sdl_set_text (tooltip, text);
  
  return tooltip;
}

void
tooltip_sdl_free (t_tooltip_sdl **tooltip) {
  if (tooltip==NULL) {
    fprintf (stderr, "Erreur dans %s(); : **tooltip ne doit pas être NULL.\n", __func__);
    return;
  }

  if (*tooltip==NULL) {
    fprintf (stderr, "Erreur dans %s(); : tooltip ne doit pas être NULL.\n", __func__);
    return;
  }

  if ((*tooltip)->font)
    TTF_CloseFont((*tooltip)->font);
  
  if ((*tooltip)->text)
    free ((*tooltip)->text);

  if ((*tooltip)->surface)
    SDL_FreeSurface((*tooltip)->surface);

  if ((*tooltip)->texture)
    SDL_DestroyTexture ((*tooltip)->texture);

  free (*tooltip);

  *tooltip = NULL;
}

void
tooltip_sdl_set_text (t_tooltip_sdl *tooltip, const char *text) {
  if (tooltip == NULL) {
    fprintf (stderr, "Erreur dans %s(); : tooltip ne doit pas être NULL.\n", __func__);
    return;
  }

  if (tooltip->text)
    free (tooltip->text);

  if (text == NULL)
    tooltip->text = strdup (" ");
  else
    tooltip->text = strdup (text);
  
  tooltip_sdl_surface_new (tooltip);
}

void
tooltip_sdl_update (t_tooltip_sdl *tooltip, SDL_Rect *size_button) {
  if (tooltip == NULL) {
    fprintf (stderr, "Erreur dans %s(); : tooltip ne doit pas être NULL.\n", __func__);
    return;
  }

  /* Récupération de la texture du texte et en même temps de sa taille */
  if (tooltip_sdl_create_texture (tooltip) == false)
    return;

  /* Récupération de la taille de la texture */
  SDL_Rect size;
  SDL_QueryTexture (tooltip->texture, NULL, NULL, &size.w, &size.h);

  /* Agrandissement de la taille de chaque côté pour englober correctement le texte */
  size.w += 10;
  size.h += 10;
  /* Positionnement de la bulle d'aide */
  size.x = size_button->x - size.w / 2;
  size.y = size_button->y + size_button->h;

  /* Récupération de la taille de la fenêtre */
  SDL_Window *window = SDL_RenderGetWindow(tooltip->renderer);
  int winx, winy;
  SDL_GetWindowSize(window, &winx, &winy);

  /* Vérififcation et modification de la position de la bulle d'aide si elle sort de l'écran */
  if (size.x+size.w > winx)
    size.x = winx - size.w -5; // -5 pour ne pas coller la bulle d'aide au bord de l'écran
  else if (size.x < 0)
    size.x = 5;

  if (size.y+size.h > winy)
    size.y = winy - size_button->h - size.h; 
  else if (size.y < 0)
    size.y = 5;  
  
  SDL_SetRenderDrawColor (tooltip->renderer, 0, 0, 0, 255);
  SDL_RenderFillRect (tooltip->renderer, &size);

  /* Dessin du cadre */
  SDL_SetRenderDrawColor(tooltip->renderer, 180, 180, 180, 255);
  SDL_RenderDrawRect(tooltip->renderer, &size);

  /* Application de la texture dans le rendu */
  /* Déplacement du texte de +5 vers la droite */
  size.x += 5;
  size.y += 5;
  size.w -= 10;
  size.h -= 10;
  SDL_RenderCopy(tooltip->renderer, tooltip->texture, NULL, &size);
}

/*-----------------------------------*/

static void
tooltip_sdl_surface_new (t_tooltip_sdl *tooltip) {
  if (tooltip==NULL) {
    fprintf (stderr, "Erreur dans %s(); : tooltip ne doit pas être NULL.\n", __func__);
    return;
  }

  if (tooltip->surface)
    SDL_FreeSurface(tooltip->surface);

  int h;
  TTF_SizeUTF8 (tooltip->font, tooltip->text, NULL, &h);
  
  tooltip->surface = TTF_RenderUTF8_Blended_Wrapped(tooltip->font, tooltip->text, (SDL_Color){255, 255, 255, 255}, 300);
}

static bool
tooltip_sdl_create_texture (t_tooltip_sdl *tooltip) {
  if (tooltip==NULL) {
    fprintf (stderr, "Erreur dans %s(); : tooltip ne doit pas être NULL.\n", __func__);
    return false;
  }

  if (tooltip->texture)
    SDL_DestroyTexture(tooltip->texture);
  
  tooltip->texture = SDL_CreateTextureFromSurface(tooltip->renderer, tooltip->surface);

  return true;
}
