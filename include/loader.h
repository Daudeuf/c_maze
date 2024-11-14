#ifndef MAZE_LOADER_H
#define MAZE_LOADER_H

typedef struct { // add player data (x,y,score,inv) & difficulty
	int   id;
	int   height;
	int   width;
	int** map;
} maze_map_t;

typedef struct { // add classement
	int  id;
	char name[33]; // taille max du nom de 32 caractères
} maze_data_t;

int          get_maze_count();
maze_data_t* get_maze_list();
maze_map_t   get_maze_map(int id);
int          get_free_id();
void         save_maze(int id, int height, int width, char* name, int** maze_map);
void         remove_maze(int id);

#endif