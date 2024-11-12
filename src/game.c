#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "tick.h"

// Elements of main menu
const char* MENU_ITEMS[] = { "Creer", "Charger", "Jouer", "Classement", "Quitter" };

// State
int mode; // 0: initial_menu, 1:create, 2:load, 3:classic, 4:advanced

// Menu
int menu_index; // 0:créer, 1:charger, 2:jouer, 3:classement, 4:quitter
int menu_index_count;



void init_game() {
	mode = 0;
	menu_index = 0;
	menu_index_count = 5;
}

void handle_key_game(char key) {
	if (mode == 0 || mode == 2) {
		if (key == 'z' || key == 'q') {
			if (menu_index > 0) menu_index--;
		}
		if (key == 's' || key == 'd') {
			if (menu_index < (menu_index_count - 1)) menu_index++;
		}
		if (key == '\n' || key == ' ') {
			if (menu_index == 0); // créer
			if (menu_index == 1); // charger
			if (menu_index == 2); // jouer
			if (menu_index == 3); // classement
			if (menu_index == 4) tick_quit(); // quitter
		}
	}
}

void tick_game() {

}

char* menu_game(int h, int w) {
	char* menu = (char*) malloc(h*w * sizeof(char));

	int gap = h > 4 ? 2 : (h > 2 ? 1 : 0);
	int max_element_line = h - 2*gap;

	for (int i=0; i < h*w; i++) menu[i] = ' ';

	if (mode == 0) {
		int max_w = 0;

		for (int i=0; i < menu_index_count; i++) {
			int len = strlen(MENU_ITEMS[i]);
			max_w = (max_w < len) ? len : max_w;
		}

		for (int i=0; i < menu_index_count; i++) {
			int a_pos = w*(gap + i%max_element_line) + 4 + (6 + max_w)*(i/max_element_line);

			for (int j=0; j < strlen(MENU_ITEMS[i]); j++) {
				menu[a_pos + j] = MENU_ITEMS[i][j];
			}

			if (menu_index == i) menu[a_pos - 2] = '>';
		}
	}

	return menu;
}