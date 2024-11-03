#include <termios.h>

int get_terminal_dimension(int* width, int* height);
void free_grid(int** grid, int height);