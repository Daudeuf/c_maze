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
SCREEN = "LABYRINTHE" ou lab chargé
Menu = menu classique (choix) [0]

- Création
SCREEN = labyrinthe créer (vue map)
Menu = menu classique (input) [1]

- Charger
SCREEN = labyrinthe créer (vue map lab sélectionné)
Menu = menu classique (liste des labyrinthes) [2]

- Jouer (classique)
SCREEN = labyrinthe en cour (vue map & perso)
Menu = Liste des touches & info partie [3]

- Jouer (pas classique)
SCREEN = labyrinthe en cour (vue map 2d amélioré)
Menu = Liste des touches & info partie & inventaire [4]

- Classement
SCREEN = labyrinthe chargé (vue map lab sélectionné)
Menu = Classement [5]

*/

int terminal_width, terminal_height;
int width, menu_height, screen_height;

int** c_screen; // screen actuel
char* c_menu;   // menu actuel

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

		interface_render();
	} else {
		intermediate_interface_render();
	}
}

void init_render() {
	get_terminal_dimension(&terminal_width, &terminal_height);

	interface_render();
}

void interface_render() {
	width         = terminal_width;
	menu_height   = terminal_height * 0.20;   if (menu_height < 1) menu_height = 1;
	screen_height = terminal_height - menu_height;

	reset_render(); // reset screen si taille change

	c_menu = menu_game(menu_height, width);

	printf("\033[%d;%dH", screen_height + 1, 1);
	printf("%s", c_menu);
	printf("\033[H");
	fflush(stdout);
}

void intermediate_interface_render() {
	char* menu = menu_game(menu_height, width);
	int has_change = 0;
	char* str = "";

	for (int i=0; i < menu_height*width; i++) {
		if (c_menu[i] != menu[i]) {
			if (!has_change) {
				asprintf(&str, "%s\033[%d;%dH%c", str, screen_height + 1 + i/width, 1 + i%width, menu[i]);
				has_change = 1;
			} else {
				asprintf(&str, "%s%c", str, menu[i]);
			}

			c_menu[i] = menu[i];
		} else has_change = 0;
	}

	free(menu);

	if (strlen(str) != 0) {
		printf("%s\033[H", str);
		fflush(stdout);
	}
}