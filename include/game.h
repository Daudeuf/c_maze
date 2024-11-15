#ifndef MAZE_GAME_H
#define MAZE_GAME_H

void init_game();
void handle_key_game(char key);
void tick_game();
char* menu_game(int h, int w);
char* map_game(int h, int w);

#endif