#ifndef __DEFINE_SDL__
#define __DEFINE_SDL__

#define couleur_fond_sdl (SDL_Color){187, 122, 87, 255}        // Couleur de fond par défaut du widget
#define couleur_active_sdl (SDL_Color){195, 195, 195, 255}     // Couleur de fond du widget lorsque la souris est dessus
#define couleur_cadre_sdl (SDL_Color){0, 0, 0, 255}            // Couleur du cadre
#define couleur_insensible_sdl (SDL_Color){200, 200, 200, 255} // Couleur du fond du widget lorsqu'il est insensible aux évènements

#define font_sdl "times.ttf"    // Police utilisée pour tous les textes de l'ihm
#define cursor_sdl "ihm/images/cursor.png" // Image du curseur lorsque le pointeur de la souris est sur un widget
#endif
