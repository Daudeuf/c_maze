#include <stdio.h>
#include <string.h>

#include "generation.h"
#include "function.h"
#include "affichage.h"
#include "loader.h"

// https://www.ascii-art-generator.org/
const char* title[] = {
	".·:'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''':·.",
	": :  _        _    ______   ______  ___ _   _ _____ _   _ _____  : :",
	": : | |      / \\  | __ ) \\ / /  _ \\|_ _| \\ | |_   _| | | | ____| : :",
	": : | |     / _ \\ |  _ \\\\ V /| |_) || ||  \\| | | | | |_| |  _|   : :",
	": : | |___ / ___ \\| |_) || | |  _ < | || |\\  | | | |  _  | |___  : :",
	": : |_____/_/   \\_\\____/ |_| |_| \\_\\___|_| \\_| |_| |_| |_|_____| : :",
	"'·:..............................................................:·'"
};

int menu(void) {
	int valeur = -1, result = -1;
	int terminal_width, terminal_height;

	get_terminal_dimension(&terminal_width, &terminal_height);

	for (int i = 0; i < 7; i++) {
		int n = (terminal_width - strlen(title[i])) / 2;
		char offset[n + 1];
		for (int j = 0; j < n; j++) offset[j] = ' ';
		offset[n] = '\0';
		printf("%s%s\n", offset, title[i]);
	}

	while (valeur != 0) {
		while (result != 1) {
			printf("\n 1) Créer un labyrinthe (11x25)\n 4) Créer un labyrinthe (max)\n 2) Charger un labyrinthe\n 3) Jouer\n 0) Quitter\n\tChoix : ");
			result = scanf("%d", &valeur);
			if (result != 1) while (getchar() != '\n');
		}

		printf("\n");
		result = -1;
		int** grid = NULL;

		switch (valeur) {
			case 1:
				grid = generate_grid(1024, 1024);
				afficher_lab( grid , 1024, 1024);
				save_new_maze(6, 1024, 1024, "maze de test", grid, 0, 0, NULL);
				free_grid(grid, 1024);
				grid = NULL;
				break;
			case 2:
				break;
			case 3:
				break;
			case 4:
				grid = generate_grid(terminal_height - 7, terminal_width/2);
				afficher_lab( grid , terminal_height - 7, terminal_width/2);
				save_new_maze(7, terminal_height - 7, terminal_width/2, "maze de test fullscreen", grid, 0, 0, NULL);
				free_grid(grid, terminal_height - 7);
				grid = NULL;
				break;
			default:
				break;
		}
	}

	return 0;
}

void afficher_lab(int** grid, int height, int width) {
	if (height%2==0) height--;
	if (width%2==0) width--;
	/*
	 * -1 : murs
	 * -2 : personnage
	 * -3 : sortie
	 * 
	 * autres nombre : chemin
	 */

	for (int y=0; y < height; y++) {
		for (int x=0; x < width; x++) {
			if (grid[y][x] == -1 ) printf("\e[0;33m■\e[0m");
			else if (grid[y][x] == -2 ) printf("O");
			else if (grid[y][x] == -3 ) printf("-");
			else printf("▫");
		}

		printf("\n");
	}
}

/*

test interface direct

*/
