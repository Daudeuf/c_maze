#include <stdio.h>
#include "module.h"
#include "module_sec.h"
#include "generation.h"
#include "affichage.h"
#include "loader.h"
#include "player.h"
#include "tick.h"

int main(void) {
	/*printf("free id : %d\n", get_free_id());
	maze_data_t* mazes = get_maze_list();
	for (int i = 0; i < get_maze_count(); i++) printf("-> %d %s\n", mazes[i].id, mazes[i].name);
	maze_map_t maze_map = get_maze_map(7);
	afficher_lab( maze_map.map, maze_map.height, maze_map.width);*/

	//tick_start();

	menu();

	/*maze_map_t maze_map = get_maze_map(7);

	start(maze_map.height, maze_map.width, maze_map.map);*/

	return 0;
}