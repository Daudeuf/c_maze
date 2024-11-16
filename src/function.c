#include <sys/ioctl.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "loader.h"

void get_terminal_dimension(int* width, int* height) {
	struct winsize w;

	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0) {
		*width = w.ws_col;
		*height = w.ws_row;
	} else {
		*width = 80; // default value
		*height = 25;
	}
}

void free_grid(int** grid, int height) {
	if (height%2==0) height--;

	for (int i = 0; i < height; i++) {
		free(grid[i]);
	}

	free(grid);
}

void free_maze_map(maze_map_t maze) {
	int height = maze.height;
	if (height%2==0) height--;

	for (int i = 0; i < height; i++) {
		free(maze.map[i]);
	}

	for (int i = 0; i < maze.player_inventory_count; i++) {
		free(maze.player_inventory[i]);
	}

	free(maze.map);
	free(maze.player_inventory);
	free(maze.monsters);
}

int copy_string_pos(const char* src, char* dest, int start_pos) {
	if (strlen(dest) <= start_pos) return start_pos;

	int j;

	for (j=0; j < strlen(src); j++) {
		if (dest[start_pos + j] == '\0') break;

		dest[start_pos + j] = src[j];
	}

	return j + start_pos;
}

int copy_int_to_string_pos(const int src, char* dest, int start_pos) {
	if (strlen(dest) <= start_pos) return start_pos;

	char* src_str;
	int j;

	asprintf(&src_str, "%d", src);

	for (j=0; j < strlen(src_str); j++) {
		if (dest[start_pos + j] == '\0') break;

		dest[start_pos + j] = src_str[j];
	}

	free(src_str);

	return j + start_pos;
}

int distance(int x1, int y1, int x2, int y2) {
	return (int) ( sqrt( pow(x2 - x1, 2) + pow(y2 - y1, 2) ) );
}
