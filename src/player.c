#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>

#include "affichage.h"

void deplacer_curseur(int x, int y) {
	printf("\033[%d;%dH", y + 1, x + 1);
}

void start(int height, int width, int** grid) {
	int player_x = width / 2;
	int player_y = height / 2;
	char key;
	struct termios old_termios, new_termios;

	tcgetattr(STDIN_FILENO, &old_termios);
	new_termios = old_termios;
	new_termios.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);


	system("clear");

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			if (player_x == x && player_y == y) printf("\e[0;31m P\e[0m");
			else if (grid[y][x] == -1 ) printf("\e[0;33m██\e[0m");
			else if (grid[y][x] == -2 ) printf(" O");
			else if (grid[y][x] == -3 ) printf("🔑");
			else printf("██");
		}

		printf("\n");
	}

	while (1) {
		key = getchar();

		if (key == 'z' || key == 'q' || key == 's' || key == 'd') {
			int new_x = player_x + (key == 'q' ? -1 : (key == 'd' ? 1 : 0));
			int new_y = player_y + (key == 'z' ? -1 : (key == 's' ? 1 : 0));

			if (new_x < 0 || new_y < 0 || new_x >= width || new_y >= height || grid[new_y][new_x] == -1) {
				new_x = player_x;
				new_y = player_y;
			}

			if (new_x != player_x || new_y != player_y) {
				deplacer_curseur(player_x * 2, player_y);

				if (grid[player_y][player_x] == -1 ) printf("\e[0;33m██\e[0m");
				else if (grid[player_y][player_x] == -2 ) printf(" O");
				else if (grid[player_y][player_x] == -3 ) printf("🔑");
				else printf("██");

				player_x = new_x;
				player_y = new_y;

				deplacer_curseur(player_x * 2, player_y);
				printf("\e[0;31m P\e[0m");
			}
		} else if (key == 'x') {
			break;
		}
	}

	tcsetattr(STDIN_FILENO, TCSANOW, &old_termios);

	return 0;
}