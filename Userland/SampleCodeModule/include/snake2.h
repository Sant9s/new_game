#ifndef __SNAKE2_H__
#define __SNAKE2_H__

// shows the snake titlecard
void start_snake2();
void snake2_title_card();

// creates the map, should only be called once
void create_map();

// executes the singleplayer game
void singleplayer();

// excecutes the multiplayer game
void multiplayer();

// player spawn (adds the position of the spawn to the body of the snake)
void spawn_player_1();
void spawn_player_2();

// places a random fruit on the map (only updates fruitPosition)
void placeRandomFruit();

// calls fill_snake_square() to draw the fruit
void drawFruit();

// calls fill_snake_square() to draw each position of the body of each snake
void drawSnakes();



// moves the snake in the updated direction. Stores the tail in global variable in case a fruit
// is catched
void move_snake();

// checks if the head is in the same position of the fruit, if it is it does the following:
// it updates the size of the player, then it duplicates the tail so that next time the snake
// moves, it still has the same tail. Returns 1 if it found a fruit or 0 if it didnt
int check_fruit_position();

// recieves a position x,y and draws the square in the correct position of the map
void fill_snake_square(int x, int y, int color);

void check_player_crash();

void check_positions();
void kill_player_1();
void kill_player_2();
void singleplayer_startup();
void singleplayer_gameover();
void multiplayer_gameover();
void gameover_titlecard();

void check_input();

#endif /*__SNAKE2_H__*/