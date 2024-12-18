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
|                        Menu (& choice)                       |
|                                                              |
+--------------------------------------------------------------+

Parts :
- Initial :
SCREEN = loaded maze
Menu = default menu (choice) [0]

- Creation
SCREEN = created maze (map view)
Menu = creation menu [1]

- Loaded
SCREEN = created maze (map view)
Menu = all mazes [2]

- Play (classic)
SCREEN = current maze (map view playable)
Menu = datas about game [3]

- Play (advanced)
SCREEN = current maze (upgraded map view playable)
Menu = datas about game [4]

- Ranking
SCREEN = loaded maze (map view)
Menu = Ranking [5]

*/

int terminal_width, terminal_height;
int width, menu_height, screen_height;

char* c_screen; // current screen
char* c_menu;   // current menu

void reset_render() {
	// ANSI sequence to put cursor at position 1;1 and clear screen
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

	reset_render(); // reset screen if size change

	c_menu   = menu_game(menu_height, width);
	c_screen = map_game(screen_height, width);

	printf("\033[H");
	printf("%s", c_screen);
	printf("\033[%d;%dH", screen_height + 1, 1);
	printf("%s", c_menu);
	printf("\033[H");
	fflush(stdout);
}

void intermediate_interface_render() {
	char* screen = map_game(screen_height, width);
	char* menu   = menu_game(menu_height, width);
	int has_change = 0;
	char* str = "";

	for (int i=0; i < screen_height*width; i++) {
		if (c_screen[i] != screen[i]) {
			if (!has_change) {
				asprintf(&str, "%s\033[%d;%dH%c", str, 1 + i/width, 1 + i%width, screen[i]);
				has_change = 1;
			} else {
				asprintf(&str, "%s%c", str, screen[i]);
			}

			c_screen[i] = screen[i];
		} else has_change = 0;
	}

	has_change = 0;

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

	free(screen);
	free(menu);

	if (strlen(str) != 0) {
		printf("%s\033[H", str);
		fflush(stdout);
	}
}