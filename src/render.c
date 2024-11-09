#include <stdio.h>
#include <stdlib.h>

#include "render.h"
#include "function.h"
#include "game.h"

/*

+--------------------------------------------------------------+
|                                                              |
|                                                              |
|                                                              |
|                                                              |
|                            SCREEN                            |
|                                                              |
|                                                              |
|                                                              |
|                                                              |
+--------------------------------------------------------------+
|                        Menu (& choix)                        |
|                                                              |
+--------------------------------------------------------------+

Phases :
- Initial :
SCREEN = "LABYRINTHE"
Menu = menu classique

- Création
SCREEN = labyrinthe créer (vue map)
Menu = menu classique

- Charger
SCREEN = labyrinthe créer (vue map lab sélectionné)
Menu = menu classique (liste des labyrinthes)

- Jouer (classique)
SCREEN = labyrinthe en cour (vue map & perso)
Menu = Liste des touches & info partie

- Jouer (pas classique)
SCREEN = labyrinthe en cour (vue map 2d amélioré)
Menu = Liste des touches & info partie & inventaire

*/

int terminal_width, terminal_height;

void reset_render() {
	// sequence ansi pour se mettre à la position 1;1 et clear le termianl après
	printf("\033[H\033[J");
	fflush(stdout);
}

void tick_render() {
	int terminal_width_n, terminal_height_n;
	get_terminal_dimension(&terminal_width_n, &terminal_height_n);

	if (terminal_width_n != terminal_width || terminal_height_n != terminal_height) {
		terminal_width = terminal_width_n;
		terminal_height = terminal_height_n;

		reset_render(); // reset screen si taille change

		/*printf("\033[%d;%dH", terminal_height/2 + 1, (terminal_width - 10)/2 + 1);
		printf("Labyrinthe");
		printf("\033[H");
		fflush(stdout);*/
	}
}

void init_render() {
	get_terminal_dimension(&terminal_width, &terminal_height);

	reset_render();

	char* menu = menu_game(terminal_height * 0.20, terminal_width);

	/*printf("\033[%d;%dH", terminal_height/2 + 1, (terminal_width - 10)/2 + 1);
	printf("Labyrinthe");
	printf("\033[H");
	fflush(stdout);*/
}
