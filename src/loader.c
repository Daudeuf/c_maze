#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>

#include "loader.h"

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

			fgets(line, sizeof(line), file);
			sscanf(line, "%d", &lst[i].id);
			fgets(lst[i].name, sizeof(lst[i].name), file);
			lst[i].name[strlen(lst[i].name)-1] = '\0';
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

		fgets(line, sizeof(line), file);
		sscanf(line, "%d", &maze_map.id);
		fgets(line, sizeof(line), file);
		fgets(line, sizeof(line), file);

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

void save_maze(int id, int height, int width, char* name, int** maze_map) {
	if (height%2==0) height--;
	if (width%2==0) width--;

	char filename[32];
	int char_count = 0;

	sprintf(filename, "./mazes/maze-map-%d.cfg", id);
	FILE* file = fopen(filename, "w");

	if (file != NULL) {
		fprintf(file, "%d\n%s\n\n", id, name);

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

void remove_maze(int id) {
	printf("not yet implemented :)\n");
}
