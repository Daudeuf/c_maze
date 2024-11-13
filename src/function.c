#include <sys/ioctl.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <string.h>

void get_terminal_dimension(int* width, int* height) {
	struct winsize w;

	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0) {
		*width = w.ws_col;
		*height = w.ws_row;
	} else {
		*width = 80; // si echec alors valeurs par défaut
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