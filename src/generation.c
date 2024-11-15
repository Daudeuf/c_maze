#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "generation.h"

vector_t* generate_shuffled_vectors(int width, int height) {
	int nb = ((height - 2) * (width - 2) / 2);

	vector_t * lst = NULL;

	srand ( time(NULL) );
	
	lst = (vector_t *) malloc(nb*sizeof(vector_t));

	for (int i = 0; i < nb; i++) {
		vector_t v = {
			1 + (1 + i * 2) % (width - 2),
			1 + (1 + i * 2) / (width - 2)
		};

		lst[i] = v;
	}

	for (int i = nb-1; i > 0; i--)
	{
		int j = rand() % (i+1);

		vector_t temp = lst[i];
		lst[i] = lst[j];
		lst[j] = temp;
	}

	return lst;
}

vector_t* generate_empty_shuffled_vectors(int** grid, int width, int height) {
	int nb = 0;

	srand ( time(NULL) );

	for (int y=0; y < height; y++) {
		for (int x=0; x < width; x++) {
			if (grid[y][x] == 0) {
				nb++;
			}
		}
	}

	vector_t * lst = (vector_t *) malloc(nb*sizeof(vector_t));

	int i = 0;
	for (int y=0; y < height; y++) {
		for (int x=0; x < width; x++) {
			if (grid[y][x] == 0) {
				vector_t v = { x, y };

				lst[i++] = v;
			}
		}
	}

	for (int i = nb-1; i > 0; i--)
	{
		int j = rand() % (i+1);

		vector_t temp = lst[i];
		lst[i] = lst[j];
		lst[j] = temp;
	}

	return lst;
}

void change_side_value(int** grid, int x, int y, int val) {
	// 166s (1024x1024)
	/*for (int i = 0; i < 4; i++) {
		int a = y + (i%2 == 0 ? (i == 0 ? 1 : -1) : 0), b = x + (i%2 != 0 ? (i == 1 ? 1 : -1) : 0);

		if (grid[a][b] == val) {
			grid[a][b] = grid[y][x];

			change_side_value(grid, b, a, val);
		}
	}*/

	// 88s
	if (grid[y + 1][x] == val) { grid[y + 1][x] = grid[y][x]; change_side_value(grid, x, y + 1, val); }
	if (grid[y - 1][x] == val) { grid[y - 1][x] = grid[y][x]; change_side_value(grid, x, y - 1, val); }
	if (grid[y][x + 1] == val) { grid[y][x + 1] = grid[y][x]; change_side_value(grid, x + 1, y, val); }
	if (grid[y][x - 1] == val) { grid[y][x - 1] = grid[y][x]; change_side_value(grid, x - 1, y, val); }
}

/*
0: chemin
1: mur
2: entrée
3: sortie
4: piège
5: trésor
6: clé
7: piège activé
8: trésor découvert
*/

int** generate_grid(int height, int width) {
	/*
	 * 512x512 : 10s
	 * 1024x1024 : 2m 20s
	 */
	clock_t start = clock();

	if (height%2==0) height--;
	if (width%2==0) width--;

	int    cpt = 2;
	int** grid = (int**) malloc(height * sizeof(int*));

	for (int y=0; y < height; y++) {
		grid[y] = (int*) malloc(width * sizeof(int));

		for (int x=0; x < width; x++) {
			grid[y][x] = (y == 0 || y == height - 1 || x == 0 || x == width - 1 || (x % 2 == 0 || y % 2 == 0)) ? 1 : cpt++;
		}
	}

	//delta(start, 1);

	vector_t * lst = generate_shuffled_vectors(width, height);

	//delta(start, 2);

	//int cpt_it = 0;

	for (int i = ((height - 2) * (width - 2) / 2) - 1; i >= 0; i--) {
		int x = lst[i].x, y = lst[i].y;
		int val_a = 1, val_b = 1;

		if (grid[y][x - 1] != 1 && grid[y][x + 1] != 1) {
			val_a = grid[y][x - 1]; val_b = grid[y][x + 1];
		} else if (grid[y - 1][x] != 1 && grid[y + 1][x] != 1) {
			val_a = grid[y - 1][x]; val_b = grid[y + 1][x];
		}

		if (val_a != val_b && val_a != 1 && val_b != 1) {
			grid[y][x] = val_a;

			change_side_value(grid, x, y, val_b);
			//cpt_it ++;

		} else grid[y][x] = 1;
	}

	//printf("changement : %d murs : %d\n", cpt_it, (height - 2) * (width - 2) / 2);

	//delta(start, 3);

	free(lst); // libération de la mémoire prise par la liste de vecteur

	// Mettre le chemin à 0
	int val = grid[1][1];
	grid[1][1] = 0;
	change_side_value(grid, 1, 1, val);

	grid[0][1] = 3; // sortie
	grid[height - 1][width - 2] = 2; // entrée


	//delta(start, 4);

	return grid;
}

maze_monster_t* generate_items_and_monsters(int** grid, int height, int width, int difficulty, int* monster_count) {
	int item_count = (height + width) + 1;
	*monster_count = (item_count - 1) / 4 * (difficulty == 0 ? 0 : 1);

	int ghost = (*monster_count) == 0 ? 0 : (rand()%(*monster_count));
	int ogre = (*monster_count) - ghost;

	maze_monster_t* monsters = (maze_monster_t*) malloc(*monster_count * sizeof(maze_monster_t));
	vector_t * lst = generate_empty_shuffled_vectors(grid, width, height);

	for (int i=0; i < item_count; i++) {
		grid[lst[i].y][lst[i].x] = (i == 0 ? 6 : (i <= (item_count - 1) / 2 ? 4 : 5));

		if (grid[lst[i].y][lst[i].x] == 5 && ogre > 0) {
			maze_monster_t monster = {.type=0,.x=lst[i].x,.def_x=lst[i].x,.y=lst[i].y,.def_y=lst[i].y};

			monsters[(*monster_count) - ghost - ogre--] = monster;
		}
	}
	
	free(lst);
	
	for (int i=0; i < ghost; i++) {
		int x = 1 + rand()%(width - 2);
		int y = 1 + rand()%(height - 2);

		maze_monster_t monster = {.type=1,.x=x,.def_x=x,.y=y,.def_y=y};

		monsters[(*monster_count) - ghost + i] = monster;
	}

	return monsters;
}

void break_walls(int** grid, int height, int width) {
	vector_t * lst = generate_shuffled_vectors(width, height);
	
	for (int i = (height - 2) * (width - 2) / 8; i >= 0; i--) {
		grid[lst[i].y][lst[i].x] = 0;
	}
	
	free(lst);
}
