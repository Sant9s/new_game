#include <utils.h>
#include <colors.h>
#include <UserSyscalls.h>
#include <game.h>
#include <buffer.h>
#define SMALL_SQUARE 10     // amount of pixeles in vertice of a square
#define DIALOGUE_SQUARE 150
#define MAX_X_PIXEL 1010
#define MAX_Y_PIXEL 550
#define MAX_Y_SCREEN_PIXEL 700
#define MAX_X_SMALL_SQUARE MAX_X_PIXEL / SMALL_SQUARE
#define MAX_Y_SMALL_SQUARE MAX_Y_PIXEL / SMALL_SQUARE
#define PLAYER_COLOR GREEN
#define GAME_SPEED 1
#define MAX_HP 12

MapInfo map_positions[MAX_X_SMALL_SQUARE + 1][MAX_Y_SMALL_SQUARE + 1];          // necesito el +1 porque el cuadrado map_positions[101][75] existe
int my_buffer_position;
Direction player_direction = STILL;
Position player_position;
Direction last_direction_moved = STILL;
int player_hp;
Color hp_color;
int game_over;
Blocktype last_block_checked = empty;

// en el eje X entran 102 small_squares (10) y un poquito -> hay 1020 pixeles mas o menos
// en el eje Y entran 76 small_squares (10) y un poquito -> hay 760 pixeles mas o menos

// entonces el tablero esta dividido en X de 0 a 101 y en Y de 0 a 75

void start_new_game(){
    game_over = 0;
    for(int i=0; i<=MAX_X_SMALL_SQUARE + 1; i++){
        for(int j=0; j<=MAX_Y_SMALL_SQUARE + 1; j++){
            map_positions[i][j].color = BLACK;
            map_positions[i][j].blocktype = empty;
        }
    }
    set_map_positions_at_start();
    new_game_title_card();
    create_character();
    create_new_game_map();
    draw_npc();
    draw_enemy();
    draw_healer();
    game_running();
}

void new_game_title_card(){
    int start_game = 0;
    char c;
    call_clear_screen();

    while(start_game == 0){
        setFontSize(8);
        own_printf("  SANTOS GAME\n\n\n\n\n");
        setFontSize(4);
        putString(" PRESS X TO START NEW GAME", WHITE);
        setFontSize(1);
        putNewLine();
        
        c = getC();
        if (c == 'X' || c == 'x'){
            start_game = 1;
        }
    }

    clearScreen();
    


}

void create_new_game_map(){
    // create walls around map
    for(int i=0; i<MAX_X_SMALL_SQUARE + 1; i++){
        change_square_color_by_square_position(i, 0, BLUE);
        map_positions[i][0].blocktype = map_barrier;
    }
    for(int i=0; i<MAX_X_SMALL_SQUARE + 1; i++){
        change_square_color_by_square_position(i, MAX_Y_SMALL_SQUARE, BLUE);
        map_positions[i][MAX_Y_SMALL_SQUARE].blocktype = map_barrier;
    }
    for(int i=0; i<MAX_Y_SMALL_SQUARE + 1; i++){
        change_square_color_by_square_position(0, i, BLUE);
        map_positions[0][i].blocktype = map_barrier;
    }
    for(int i=0; i<MAX_Y_SMALL_SQUARE + 1; i++){
        change_square_color_by_square_position(MAX_X_SMALL_SQUARE, i, BLUE);
        map_positions[MAX_X_SMALL_SQUARE][i].blocktype = map_barrier;
    }

    // create some pillars and stuff
    for(int i=0; i<40; i++){
        change_square_color_by_square_position(20, i, BLUE);
        change_square_color_by_square_position(40, i, BLUE);
        map_positions[20][i].blocktype = map_barrier;
        map_positions[40][i].blocktype = map_barrier;
    }

    draw_hp();

}

// this funcion recieves an x (0, 101) and a y (0, 75) and paints it
void change_square_color_by_square_position(int x_square, int y_square, Color color){
    map_positions[x_square][y_square].color = color;
    putSquare(x_square * SMALL_SQUARE, y_square * SMALL_SQUARE, SMALL_SQUARE, color);

}

void set_map_positions_at_start(){
    for(int x = 0; x <= MAX_X_SMALL_SQUARE; x++){
        for(int y = 0; y <= MAX_Y_SMALL_SQUARE; y++){
            map_positions[x][y].color = BLACK;
        }
    }
}

void create_character(){
    change_square_color_by_square_position(58, 38, PLAYER_COLOR);
    map_positions[58][38].blocktype = npc;
    player_position.x = 58;
    player_position.y = 38;
    player_direction = STILL;
    player_hp = MAX_HP;
    hp_color = GREEN;
}

void draw_bigger_square(int x_square, int y_square, Color color){
    for(int i=x_square-1; i<=x_square+1; i++){
        for(int j=y_square-1; j<=y_square+1; j++){
            map_positions[i][j].color = color;
            putSquare(i * SMALL_SQUARE, j * SMALL_SQUARE, SMALL_SQUARE, color);
        }
    }
}

void check_player_input(void) {
	char c;
	call_getBuffPosition(&my_buffer_position);         // que pingo hace esto.                 
	call_getLastKey(&c, my_buffer_position);       // TESTEAR MOVIMIENTO
	switch (c) {
		case 'w':
			player_direction=NORTH;
            last_direction_moved=NORTH;
            call_fake_sys_read(&c);
			break;
		case 'd':
			player_direction=EAST;
            last_direction_moved=EAST;
            call_fake_sys_read(&c);
			break;
		case 's':
			player_direction=SOUTH;
            last_direction_moved=SOUTH;
            call_fake_sys_read(&c);
			break;
		case 'a':
			player_direction=WEST;
            last_direction_moved=WEST;
            call_fake_sys_read(&c);
			break;
        case 'e':
            last_block_checked = check_block_type();
            if(last_block_checked != empty){
                pick_conversation(last_block_checked);
                redraw_everything();
                call_fake_sys_read(&c);
            }
            break;
        default:
            break;
	}
    
        
}

void move_player(){
    if(!((map_positions[player_position.x-1][player_position.y].color != BLACK && player_direction == WEST) || (map_positions[player_position.x+1][player_position.y].color != BLACK && player_direction == EAST) || (map_positions[player_position.x][player_position.y-1].color != BLACK && player_direction == NORTH) || (map_positions[player_position.x][player_position.y+1].color != BLACK && player_direction == SOUTH))){
        map_positions[player_position.x][player_position.y].color = BLACK; // player moves from their spot
        change_square_color_by_square_position(player_position.x, player_position.y, BLACK);
        switch (player_direction) {
			case NORTH:             // move south
                map_positions[player_position.x][player_position.y].color = BLACK;
                map_positions[player_position.x][player_position.y].blocktype = empty;
                player_position.y--;
                map_positions[player_position.x][player_position.y].color = PLAYER_COLOR;
                map_positions[player_position.x][player_position.y].blocktype = npc;
				break;
			case EAST:             // move east
                map_positions[player_position.x][player_position.y].color = BLACK;
                map_positions[player_position.x][player_position.y].blocktype = empty;
                player_position.x++;
                map_positions[player_position.x][player_position.y].color = PLAYER_COLOR;
                map_positions[player_position.x][player_position.y].blocktype = npc;
				break;
			case SOUTH:             // move north
                map_positions[player_position.x][player_position.y].color = BLACK;
                map_positions[player_position.x][player_position.y].blocktype = empty;
                player_position.y++;
                map_positions[player_position.x][player_position.y].color = PLAYER_COLOR;
                map_positions[player_position.x][player_position.y].blocktype = npc;
				break;
			case WEST:             // move west
                map_positions[player_position.x][player_position.y].color = BLACK;
                map_positions[player_position.x][player_position.y].blocktype = empty;
                player_position.x--;
                map_positions[player_position.x][player_position.y].color = PLAYER_COLOR;
                map_positions[player_position.x][player_position.y].blocktype = npc;
                break;
            default:
                break;    
        }
        change_square_color_by_square_position(player_position.x, player_position.y, PLAYER_COLOR);
        player_direction = STILL;
    }
    else if(((map_positions[player_position.x-1][player_position.y].color == RED && player_direction == WEST) || (map_positions[player_position.x+1][player_position.y].color == RED && player_direction == EAST) || (map_positions[player_position.x][player_position.y-1].color == RED && player_direction == NORTH) || (map_positions[player_position.x][player_position.y+1].color == RED && player_direction == SOUTH))){
        damage_player();
    }
}

void draw_npc(){
    change_square_color_by_square_position(10, 10, WHITE);
    map_positions[10][10].blocktype = npc;
}

void draw_enemy(){
    change_square_color_by_square_position(25, 10, RED);
    map_positions[25][10].blocktype = enemy;
}

void draw_healer(){
    change_square_color_by_square_position(80, 10, BLUE);
    map_positions[80][10].blocktype = healer;
}

void game_running(){
    game_over = 0;
    while(!game_over){
        call_sleepms(GAME_SPEED);
        check_player_input();
        move_player();
    }
    game_over_screen();
}

void draw_hp(){

    // drawing the letters HP
    for(int i=63; i<72; i++){
        putSquare(50*SMALL_SQUARE, i*SMALL_SQUARE, SMALL_SQUARE, GREEN);
        putSquare(54*SMALL_SQUARE, i*SMALL_SQUARE, SMALL_SQUARE, GREEN);
        putSquare(57*SMALL_SQUARE, i*SMALL_SQUARE, SMALL_SQUARE, GREEN);
    }

    for(int i=51; i<54; i++){
        putSquare(i*SMALL_SQUARE, 67*SMALL_SQUARE, SMALL_SQUARE, GREEN);
    }

    putSquare(58*SMALL_SQUARE, 63*SMALL_SQUARE, SMALL_SQUARE, GREEN);
    putSquare(59*SMALL_SQUARE, 63*SMALL_SQUARE, SMALL_SQUARE, GREEN);
    putSquare(58*SMALL_SQUARE, 66*SMALL_SQUARE, SMALL_SQUARE, GREEN);
    putSquare(59*SMALL_SQUARE, 66*SMALL_SQUARE, SMALL_SQUARE, GREEN);
    putSquare(60*SMALL_SQUARE, 64*SMALL_SQUARE, SMALL_SQUARE, GREEN);
    putSquare(60*SMALL_SQUARE, 65*SMALL_SQUARE, SMALL_SQUARE, GREEN);


    // drawing the bar
    for(int i=65; i<90; i++){
        putSquare(i*SMALL_SQUARE, 63*SMALL_SQUARE, SMALL_SQUARE, WHITE);
        putSquare(i*SMALL_SQUARE, 72*SMALL_SQUARE, SMALL_SQUARE, WHITE);
    }

    for(int i=63; i<72; i++){
        putSquare(65*SMALL_SQUARE, i*SMALL_SQUARE, SMALL_SQUARE, WHITE);
        putSquare(89*SMALL_SQUARE, i*SMALL_SQUARE, SMALL_SQUARE, WHITE);
    }

    // fill bar with black
    for(int i=66; i<89; i++){
        for(int j=64; j<72; j++){
            putSquare(i*SMALL_SQUARE, j*SMALL_SQUARE, SMALL_SQUARE, BLACK);
        }
    }

    

    // fill bar with hp
    for(int i=66; i<89; i++){
        if(player_hp >= ((i-65) / 2)){
            for(int j=64; j<72; j++){
                putSquare(i*SMALL_SQUARE, j*SMALL_SQUARE, SMALL_SQUARE, hp_color);
            }
        }
            
    }

}

void damage_player(){
    player_hp--;
    if(player_hp < 6){
        hp_color = RED;
    }
    draw_hp();
    if(player_hp == 0){
        game_over = 1;
    }
    
}

void game_over_screen(){
    call_sleepms(200);
	call_clear_screen();
    setFontSize(10);
    putString("             GAME OVER\n", RED);
    setFontSize(3);
    putNewLine();
	putNewLine();
	putNewLine();
	putString("\n\nThank You for playing!!\n\n\n\n", WHITE);
    setFontSize(1);
	call_sleepms(1000);
	call_clear_screen();

}

// returns the type of block the player is facing
Blocktype check_block_type(){
   if(last_direction_moved==NORTH){
    return map_positions[player_position.x][player_position.y-1].blocktype;
   }
   if(last_direction_moved==SOUTH){
    return map_positions[player_position.x][player_position.y+1].blocktype;
   }
   if(last_direction_moved==EAST){
    return map_positions[player_position.x+1][player_position.y].blocktype;
   }
   if(last_direction_moved==WEST){
    return map_positions[player_position.x-1][player_position.y].blocktype;
   }
   return empty;

}

void pick_conversation(Blocktype block){
    if(block == npc){
        draw_npc_conversation();
    }
    if(block == enemy){
        draw_enemy_conversation();
    }
    if(block == map_barrier){
        draw_wall_conversation();
    }
    if(block == healer){
        draw_healer_conversation();
    }
}


void draw_npc_conversation(){
    clearScreen();
    setFontSize(2);
    putString("Hello, I am the NPC\n", WHITE);
    call_sleepms(200);
    putString("I am here to tell you that\n", WHITE);
    call_sleepms(200);
    putString("you are the chosen one\n", WHITE);
    call_sleepms(200);
    putString("and you must save the world\n", WHITE);
    call_sleepms(200);
    putString("from the evil enemy\n", WHITE);
    call_sleepms(700);
}

void draw_enemy_conversation(){
    clearScreen();
    draw_square_for_dialog(RED);
    setFontSize(2);
    putString("Hello, I am the Enemy\n", WHITE);
    call_sleepms(200);
    putString("I will ", WHITE);
    call_sleepms(500);
    setFontSize(4);
    putString("k", RED);
    call_sleepms(300);
    putString("i", RED);
    call_sleepms(300);
    putString("l", RED);
    call_sleepms(300);
    putString("l", RED);
    setFontSize(2);
    call_sleepms(300);
    putString("  you\n", WHITE);
    call_sleepms(400);
    putString("\nThen I will take your soul\n", WHITE);
    call_sleepms(600);
    clearScreen();
}

void draw_wall_conversation(){
    clearScreen();
    setFontSize(4);
    call_sleepms(100);
    putString("Dude, this is just a wall", WHITE);
    call_sleepms(400);
}

void draw_healer_conversation(){
    clearScreen();
    setFontSize(4);
    call_sleepms(200);
    putString("Hello, I am the Healer\n", BLUE);
    call_sleepms(200);
    if(player_hp == MAX_HP){
        putString("You are already at full health\n", WHITE);
        call_sleepms(400);
        return;
    }
    putString("I will heal you, my son\n", WHITE);
    call_sleepms(400);
    player_hp += 3;
    if(player_hp > MAX_HP){
        player_hp = MAX_HP;
    }
    if(player_hp >= 6){
        hp_color = GREEN;
    }
}

void redraw_everything(){
    for(int i=0; i<MAX_X_SMALL_SQUARE + 1; i++){
        for(int j=0; j<MAX_Y_SMALL_SQUARE + 1; j++){
            change_square_color_by_square_position(i, j, map_positions[i][j].color);
        }
    }
    draw_hp();
}


void draw_square_for_dialog(Color color){
    putSquare(800, 600, DIALOGUE_SQUARE, color);
}
