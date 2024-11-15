#ifndef MAZE_LOADER_H
#define MAZE_LOADER_H

typedef struct {
	int type;
	int def_x;
	int def_y;
	int x;
	int y;
} maze_monster_t;

typedef struct {
	int    id;
	int    height;
	int    width;
	int**  map;

	int    difficulty;

	int    player_x;
	int    player_y;
	int    player_score;
	char** player_inventory;
	int    player_inventory_count;

	int    monster_count;
	maze_monster_t* monsters;
} maze_map_t;

typedef struct {
	int  id;
	char name[33];
	int  ranking[10];
} maze_data_t;

int          get_maze_count();
maze_data_t* get_maze_list();
maze_data_t  get_maze_map_data(int id);
maze_map_t   get_maze_map(int id);
int          get_free_id();
void         save_new_maze(int id, int height, int width, char* name, int** maze_map, int difficulty, int monster_count, maze_monster_t* monsters);
void         save_maze(int id, int height, int width, char* name, int** maze_map, int difficulty, int monster_count, maze_monster_t* monsters, int player_x, int player_y, int player_score, int player_inventory_count, char** player_inventory, int* ranking);
//void         remove_maze(int id);
int          player_has_item(maze_map_t* map, char* item);
void         player_add_item(maze_map_t* map, char* item);
int          player_remove_item(maze_map_t* map, char* item);
void         player_clear_item(maze_map_t* map);

#endif