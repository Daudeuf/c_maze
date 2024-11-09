#include <stdio.h>
#include <stdlib.h>

#include "render.h"
#include "function.h"

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

void clear_render() {
	char* str = (char*) malloc( (terminal_height * (terminal_width + 1)) * sizeof(char));

	str[terminal_height * (terminal_width + 1) - 1] = '\0';

	for (int i=0; i < terminal_height; i++) {
		for (int j=0; j < terminal_width; j++) str[i * (terminal_width + 1) + j] = '_';
		if (i < terminal_height - 1) str[i * (terminal_width + 1) + terminal_width] = '\n';
	}

	printf("%s", str);
	fflush(stdout);

	//printf("\033[%d;%dH", 1, 1);

	/*for (int i=0; i < terminal_height - 1; i++) {
		for (int j=0; j < terminal_width/2; j++) printf("_");
		printf("\n");
	}

	for (int j=0; j < terminal_width/2 -5; j++) printf("_");

	fflush(stdout);*/

	printf("\033[%d;%dH", terminal_height-1, terminal_width/2);
	printf("Hello !");
	printf("\033[%d;%dH", terminal_height, terminal_width);
	fflush(stdout);
}

void tick_render() {
	int terminal_width_n, terminal_height_n;
	get_terminal_dimension(&terminal_width_n, &terminal_height_n);

	if (terminal_width_n != terminal_width || terminal_height_n != terminal_height) {
		terminal_width = terminal_width_n;
		terminal_height = terminal_height_n;


		//printf("%d x %d\n", terminal_width, terminal_height);
		clear_render(); // clear screen si taille change

		/*printf("\033[%d;%dH", terminal_height/2 + 1, terminal_width/2 + 1);
		printf("Labyrinthe");
		printf("\033[1;1H");*/
	}

	//printf("\033[1;1H");
	//printf("Bonjour, en haut à gauche du terminal !");
}

void init_render() {
	get_terminal_dimension(&terminal_width, &terminal_height);

	clear_render();

	/*printf("\033[%d;%dH", terminal_height/2 + 1, terminal_width/2 + 1);
	printf("Labyrinthe");
	printf("\033[1;1H");*/
}
