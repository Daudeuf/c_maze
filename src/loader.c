#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>

#include "loader.h"

/* local function */

int end_with(char* str, char* end) {
	if (!str || !end) return 0;

	size_t lenstr = strlen(str);
	size_t lenend = strlen(end);

	if (lenend > lenstr) return 0;

	return strncmp(str + lenstr - lenend, end, lenend) == 0;
}

int filter(const struct dirent *name) {
	if (strncmp(name->d_name, "maze-map-", 9) != 0) return 0;

	return end_with(name->d_name, ".cfg");
}

int is_int(const char* str) {
	char* endptr;
	strtol(str, &endptr, 10);
	return *endptr == '\0';
}

/* end local function */

int get_maze_count() {
	struct dirent **namelist;
	return scandir("./mazes", &namelist, filter, alphasort);
}

maze_data_t* get_maze_list() {
	char filename[32];
	struct dirent **namelist;
	int n = scandir("./mazes", &namelist, filter, alphasort);

	maze_data_t* lst = (maze_data_t*) malloc( n * sizeof(maze_data_t) );

	for (int i = 0; i < n; i++) {
		sprintf(filename, "./mazes/%s", namelist[i]->d_name);
		FILE* file = fopen(filename, "r");

		if (file != NULL) {
			char line[256];
			int x, y, s;
			int inv_count, monster_count;
			int ranking[10];

			fgets(line, sizeof(line), file);
			sscanf(line, "%d", &lst[i].id);
			fgets(lst[i].name, sizeof(lst[i].name), file);
			lst[i].name[strlen(lst[i].name) - 1] = '\0';
			fgets(line, sizeof(line), file);
			sscanf(line, "%d %d %d %d %d", &x, &y, &s, &inv_count, &monster_count);
			for (int i=0; i < (inv_count + monster_count + 1); i++) fgets(line, sizeof(line), file);
			sscanf(line, "%d %d %d %d %d %d %d %d %d %d", &ranking[0], &ranking[1], &ranking[2], &ranking[3], &ranking[4], &ranking[5], &ranking[6], &ranking[7], &ranking[8], &ranking[9]);
			fclose(file);
		}
	}

	return lst;
}

maze_map_t get_maze_map(int id) {
	maze_map_t maze_map = {};
	char filename[32];

	sprintf(filename, "./mazes/maze-map-%d.cfg", id);
	FILE* file = fopen(filename, "r");

	if (file != NULL) {
		char line[256];

		char** inv;
		maze_monster_t* monsters;

		fgets(line, sizeof(line), file);
		sscanf(line, "%d %d", &maze_map.id, &maze_map.difficulty);
		fgets(line, sizeof(line), file);// skip nom

		fgets(line, sizeof(line), file);// ligne data int
		sscanf(line, "%d %d %d %d %d", &maze_map.player_x, &maze_map.player_y, &maze_map.player_score, &maze_map.player_inventory_count, &maze_map.monster_count);

		maze_map.player_inventory = (char**) malloc(maze_map.player_inventory_count * sizeof(char*));
		maze_map.monsters = (maze_monster_t*) malloc(maze_map.monster_count * sizeof(maze_monster_t));

		for (int i=0; i < maze_map.player_inventory_count; i++) {
			maze_map.player_inventory[i] = (char*) malloc(17 * sizeof(char));
			fgets(maze_map.player_inventory[i], sizeof(maze_map.player_inventory[i]), file);
			maze_map.player_inventory[i][strlen(maze_map.player_inventory[i]) - 1] = '\0';
		}

		for (int i=0; i < maze_map.monster_count; i++) {
			maze_monster_t monster = {};

			fgets(line, sizeof(line), file);
			sscanf(line, "%d %d %d %d %d", &monster.type, &monster.def_x, &monster.def_y, &monster.x, &monster.y);

			maze_map.monsters[i] = monster;
		}

		fgets(line, sizeof(line), file);// skip classement
		fgets(line, sizeof(line), file);// skip ligne vide avant grille

		long maze_map_pos = ftell(file);
		int temp_width = 0;

		while (fgets(line, sizeof(line), file) != NULL) {
			size_t len = strlen(line);

			if (len <= 1) break;

			temp_width += len / 3;

			if (end_with(line, "E\n")) {
				maze_map.height++;
				if (maze_map.width < temp_width) maze_map.width = temp_width;
				temp_width = 0;
			}
		}

		fseek(file, maze_map_pos, SEEK_SET);

		maze_map.map = (int**) malloc( maze_map.height * sizeof(int*) );

		for (int y = 0; y < maze_map.height; y++) {
			maze_map.map[y] = (int*) malloc( maze_map.width * sizeof(int) );
		}

		int width = 0, height = 0;

		while (fgets(line, sizeof(line), file) != NULL) {
			char * strToken = strtok (line, " ");

			while ( strToken != NULL ) {

				if (strcmp(strToken, "E\n") == 0) {
					width = 0;
					height ++;
				} else if (strcmp(strToken, "\n") != 0) {
					maze_map.map[height][width++] = atoi(strToken);
				}

				strToken = strtok (NULL, " ");
			}
		}

		fclose(file);
	}

	return maze_map;
}

int get_free_id() {
	int cpt = 1;
	char filename[32];

	sprintf(filename, "./mazes/maze-map-%d.cfg", cpt);

	while (access(filename, F_OK) == 0)
		sprintf(filename, "./mazes/maze-map-%d.cfg", ++cpt);

	return cpt;
}

void save_maze(int id, int height, int width, char* name, int** maze_map, int difficulty, int monster_count, maze_monster_t* monsters, int player_x, int player_y, int player_score, int player_inventory_count, char** player_inventory, int* ranking) {
	if (height%2==0) height--;
	if (width%2==0) width--;

	char filename[32];
	int char_count = 0;

	sprintf(filename, "./mazes/maze-map-%d.cfg", id);
	FILE* file = fopen(filename, "w");

	if (file != NULL) {
		fprintf(file, "%d %d\n%s\n%d %d %d %d %d\n"/* "\n%s\n%s\n%s\n\n" */, id, difficulty, name, player_x, player_y, player_score, player_inventory_count, monster_count);

		for (int i=0; i < player_inventory_count; i++) {
			fprintf(file, "%s\n", player_inventory[i]);
		}

		for (int i=0; i < monster_count; i++) {
			fprintf(file, "%d %d %d %d %d\n", monsters[i].type, monsters[i].def_x, monsters[i].def_y, monsters[i].x, monsters[i].y);
		}

		for (int i=0; i < 10; i++) {
			fprintf(file, "%d%s", ranking[i], i == 9 ? "\n\n" : " ");
		}

		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				if (char_count + 3 > 254) { // 256 - 3 ('E' '\n' '\0')
					fprintf(file, "\n%02d ", maze_map[y][x]);
					char_count = 3;
				} else {
					fprintf(file, "%02d ", maze_map[y][x]);
					char_count += 3;
				}
			}

			fprintf(file, "E\n");
			char_count = 0;
		}

		fclose(file);
	}
}

void save_new_maze(int id, int height, int width, char* name, int** maze_map, int difficulty, int monster_count, maze_monster_t* monsters) {
	char*  default_ranking = (int*)   calloc(10, sizeof(int));

	save_maze(id, height, width, name, maze_map, difficulty, monster_count, monsters, width - 2, height - 1, 0, 0, NULL, default_ranking);
}

/*void remove_maze(int id) {
	printf("not yet implemented :)\n");
}*/
