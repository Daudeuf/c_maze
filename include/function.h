#ifndef MAZE_FUNCTION_H
#define MAZE_FUNCTION_H

#include "loader.h"

int get_terminal_dimension(int* width, int* height);
void free_grid(int** grid, int height);
void free_maze_map(maze_map_t map);
int copy_string_pos(const char* src, char* dest, int start_pos);
int copy_int_to_string_pos(const int src, char* dest, int start_pos);

#endif