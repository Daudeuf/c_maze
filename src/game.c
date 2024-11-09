#include <stdio.h>
#include <stdlib.h>

#include "game.h"

// State
int mode; // 0: initial_menu, 1:create, 2:load, 3:classic, 4:advanced

// Menu
int menu_index;



void init_game() {
	mode = 0;
	menu_index = 0;
}

void handle_key_game(char key) {
	if (key == 'u') menu_index++;
}

void tick_game() {

}

char* menu_game(int h, int w) {

}