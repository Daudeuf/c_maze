#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "tick.h"
#include "function.h"
#include "generation.h"
#include "loader.h"

// Elements of main menu
const char* MENU_ITEMS[] = { "Creer", "Charger", "Jouer", "Classement", "Quitter" };

// State
int mode; // 0: initial_menu, 1:create, 2:load, 3:classic, 4:advanced, 5:classement

// Menu
int menu_index; // 0:créer, 1:charger, 2:jouer, 3:classement, 4:quitter
int menu_index_count;

char* input_name;
char* input_width;
char* input_height;
int   input_difficulty;

maze_data_t* mazes;

// Current game
int map_loaded;
maze_map_t maze_map;


void init_game() {
	map_loaded = 0;

	mode = 0;
	menu_index = 0;
	menu_index_count = 5;
}

void handle_key_game(char key) {
	if (mode != 1 && key == 'c') tick_quit();

	if (mode == 0 || mode == 2) {
		if (key == 'z' || key == 'q') {
			if (menu_index > 0) menu_index--;
		}
		if (key == 's' || key == 'd') {
			if (menu_index < (menu_index_count - 1)) menu_index++;
		}
		if (key == '\n' || key == ' ') {
			if (mode == 0) {
				if (menu_index == 0) { // créer
					mode = 1;

					// menu_index -> 0:width, 1:height, 2:difficulty, 3:name

					input_name   = (char*) malloc(sizeof(char));
					input_width  = (char*) malloc(sizeof(char));
					input_height = (char*) malloc(sizeof(char));

					input_name[0]   = '\0';
					input_width[0]  = '\0';
					input_height[0] = '\0';

					input_difficulty = -1; // 0:facile, 1:difficile

				} else if (menu_index == 1) { // charger
					menu_index_count = get_maze_count();

					if (menu_index_count > 0) {
						mode = 2;
						menu_index = 0;
						mazes = get_maze_list();
					} else {
						menu_index_count = 5;
					}
				} else if (menu_index == 2) { // jouer
					if (map_loaded) {
						mode = 3;
					}
				} else if (menu_index == 3) { // classement

				} else if (menu_index == 4) tick_quit(); // quitter
			} else if (mode == 2) {
				if (map_loaded) free_maze_map(maze_map);

				maze_map = get_maze_map(mazes[menu_index].id);
				map_loaded = 1;

				free(mazes);

				mode = 0;
				menu_index = 0;
				menu_index_count = 5;
			}
		}
	} else if (mode == 1) {
		if (key == 0x7F) { // del: 0x7F, suppr:0x1B
			if (menu_index == 0 && strlen(input_width) > 0) input_width[strlen(input_width) - 1] = '\0';
			if (menu_index == 1 && strlen(input_height) > 0) input_height[strlen(input_height) - 1] = '\0';
			if (menu_index == 3 && strlen(input_name) > 0) input_name[strlen(input_name) - 1] = '\0';
		} else if(key == '\n') {
			if (menu_index == 0 && strlen(input_width) > 0) menu_index++;
			if (menu_index == 1 && strlen(input_height) > 0) menu_index++;
			if (menu_index == 2 && input_difficulty != -1) menu_index++;
			if (menu_index == 3 && strlen(input_name) > 0) {
				// CREATION OF THE GAME
				int width_final = atoi(input_width);
				int height_final = atoi(input_height);

				if (width_final%2  == 0) width_final--;
				if (height_final%2 == 0) height_final--;

				int** grid = generate_grid(height_final, width_final);
				int id = get_free_id();
				save_new_maze(id, height_final, width_final, input_name, grid, input_difficulty, 0, NULL);
				free_grid(grid, height_final);
				grid = NULL;

				if (map_loaded) free_maze_map(maze_map);

				maze_map = get_maze_map(id);
				map_loaded = 1;

				free(input_name);
				free(input_width);
				free(input_height);

				mode = 0;
				menu_index = 0;
				menu_index_count = 5;
			}
		} else {
			if ( menu_index == 3 && ((key <= 'Z' && key >= 'A') || (key <= 'z' && key >= 'a') || key == ' ' || key == '_' || key == '-') && strlen(input_name) <= 32 ) {
				asprintf(&input_name, "%s%c", input_name, key);
			}

			if (key <= '9' && key >= '0') {
				if (menu_index == 0) asprintf(&input_width, "%s%c", input_width, key);
				if (menu_index == 1) asprintf(&input_height, "%s%c", input_height, key);
			}

			if (menu_index == 2 && key == 'd') input_difficulty = 1;
			if (menu_index == 2 && key == 'f') input_difficulty = 0;
		}
	} else if (mode == 3) {
		int changed = 0;

		if (key == 'z' && maze_map.player_y - 1 >= 0)
			if (maze_map.map[maze_map.player_y-1][maze_map.player_x] != 1) {
				maze_map.player_y--;
				changed = 1;
			}

		if (key == 'q' && maze_map.player_x - 1 >= 0)
			if (maze_map.map[maze_map.player_y][maze_map.player_x-1] != 1) {
				maze_map.player_x--;
				changed = 1;
			}

		if (key == 's' && maze_map.player_y + 1 < maze_map.height)
			if (maze_map.map[maze_map.player_y+1][maze_map.player_x] != 1) {
				maze_map.player_y++;
				changed = 1;
			}

		if (key == 'd' && maze_map.player_x + 1 < maze_map.width)
			if (maze_map.map[maze_map.player_y][maze_map.player_x+1] != 1) {
				maze_map.player_x++;
				changed = 1;
			}

		if (changed) {
			maze_data_t d = get_maze_map_data(maze_map.id);
			save_maze(maze_map.id, maze_map.height, maze_map.width, d.name, maze_map.map, maze_map.difficulty, maze_map.monster_count, maze_map.monsters, maze_map.player_x, maze_map.player_y, maze_map.player_score, maze_map.player_inventory_count, maze_map.player_inventory, d.ranking);
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

			copy_string_pos(MENU_ITEMS[i], menu, a_pos);

			if (menu_index == i) menu[a_pos - 2] = '>';
		}
	}

	if (mode == 1) {
		int a_pos = copy_string_pos("Dimensions :", menu, w*(gap) + 4) + 1;
		a_pos = copy_string_pos(input_width, menu, a_pos);
		if (menu_index == 0) a_pos = copy_string_pos("_", menu, a_pos);
		menu[a_pos++] = 'x';
		a_pos = copy_string_pos(input_height, menu, a_pos) + 4;
		if (menu_index == 1) a_pos = copy_string_pos("_", menu, a_pos-4) + 4;

		a_pos = copy_string_pos("Difficulte (f/d) :", menu, a_pos) + 1;
		a_pos = copy_string_pos(input_difficulty == 0 ? "Facile   " : (input_difficulty == 1 ? "Difficile" : "_________"), menu, a_pos) + 4;

		if (max_element_line > 1) a_pos = w*(gap + 1) + 4;

		a_pos = copy_string_pos("Nom :", menu, a_pos) + 1;
		a_pos = copy_string_pos(input_name, menu, a_pos);
		if (menu_index == 3) copy_string_pos("_", menu, a_pos);
	}

	if (mode == 2) {
		int a_pos = copy_int_to_string_pos(menu_index + 1, menu, w*(gap) + 4);
		a_pos = copy_string_pos("/", menu, a_pos);
		a_pos = copy_int_to_string_pos(menu_index_count, menu, a_pos) + 4;

		int show = max_element_line > menu_index_count ? menu_index_count : max_element_line;
		int f_show = (menu_index > menu_index_count - show) ? menu_index_count - show : menu_index;

		for (int i=0; i < show; i++) {
			if (f_show + i == menu_index) copy_string_pos(">", menu, a_pos + i*w - 2);

			char* str_b;
			asprintf(&str_b, "[id:%d] %s", mazes[f_show + i].id, mazes[f_show + i].name);
			copy_string_pos(str_b, menu, a_pos + i*w);
		}
	}

	if (mode == 3) {

	}

	return menu;
}

char* map_game(int h, int w) {
	char* map = (char*) malloc(h*w * sizeof(char));

	int gap = (h > 10 && w > 10) ? 2 : ((h > 8 && w > 8) ? 1 : 0);
	int act_h = h - 2*gap, act_w = w - 2*gap;

	for (int i=0; i < h*w; i++) map[i] = ' ';

	if (map_loaded) {
		int y_offset = maze_map.player_y - (act_h/2);
		int x_offset = maze_map.player_x - (act_w/2);

		for (int y=0; y < act_h; y++) {
			for (int x=0; x < act_w; x++) {
				if (y+y_offset < maze_map.height && x+x_offset < maze_map.width && y+y_offset >= 0 && x+x_offset >= 0) {
					char c = ' ';

					switch (maze_map.map[y+y_offset][x+x_offset]) {
						case 1:
							c = '#';
							break;
						case 2:
							c = '-';
							break;
						case 3:
							c = 'o';
							break;
						case 4:
							c = 'P';
							break;
						case 5:
							c = 'T';
							break;
						default:
							break;
					}

					if (mode == 3 && maze_map.player_y == y+y_offset && maze_map.player_x == x+x_offset) c = 'P';

					map[(gap+y)*w+gap+x] = c;
				}
			}
		}
	}

	return map;
}