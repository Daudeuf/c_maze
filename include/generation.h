#ifndef MAZE_GENERATION_H
#define MAZE_GENERATION_H

#include "loader.h"

typedef struct {
	int x;
	int y;
} vector_t;

vector_t*       generate_shuffled_vectors(int width, int height);
vector_t* generate_empty_shuffled_vectors(int** grid, int width, int height);
void            change_side_value(int** grid, int x, int y, int val);
int**           generate_grid(int height, int width);
maze_monster_t* generate_items_and_monsters(int** grid, int height, int width, int difficulty, int* monster_count);
void            break_walls(int** grid, int height, int width);


#endif