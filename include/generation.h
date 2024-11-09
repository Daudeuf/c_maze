#ifndef MAZE_GENERATION_H
#define MAZE_GENERATION_H

typedef struct {
	int x;
	int y;
} vector_t;

vector_t* generate_shuffled_vectors(int width, int height);
void change_side_value(int** grid, int x, int y, int val);
int** generate_grid(int height, int width);

#endif