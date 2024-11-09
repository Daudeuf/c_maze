#include <sys/ioctl.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>

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
