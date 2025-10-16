AUTOMAKE_OPTIONS = subdir-objects

bin_PROGRAMS = morpion

morpion_SOURCES = ihm/src/chain_list.c\
ihm/src/logs.c\
ihm/src/ihm_sdl.c\
ihm/src/widget_sdl.c\
ihm/src/tooltip_sdl.c\
ihm/src/button_sdl.c\
ihm/src/state_button_sdl.c\
ihm/src/toggle_button_sdl.c\
ihm/src/dialog_sdl.c\
ihm/src/game_config_sdl.c\
ihm/src/config_cursor_sdl.c\
ihm/src/case_sdl.c\
morp_sdl.c\
Affichage.c\
ia.c\
Instruction.c\
Jeu.c\
texture.c\
Tournoi.c\
main.c

ACLOCAL_AMFLAGS = -I m4

AM_CFLAGS = -Wunused -Wall -Wextra -g $(SLD2_CFLAGS) $(ICU_CFLAGS)
LDADD = -lm $(SDL2_LIBS) $(ICU_LIBS) -lSDL2_ttf -lSDL2_mixer -lSDL2_image
