#include <utils.h>
#include <colors.h>
#include <UserSyscalls.h>
#include <game.h>
#include <buffer.h>

// sizes
#define SMALL_SQUARE 10             // amount of pixeles in vertice of a square
#define DIALOGUE_SQUARE 150         // size of character during dialogue

// map limits
#define MAX_X_PIXEL 1010
#define MAX_Y_PIXEL 550
#define MAX_Y_SCREEN_PIXEL 700
#define MAX_X_SMALL_SQUARE MAX_X_PIXEL / SMALL_SQUARE
#define MAX_Y_SMALL_SQUARE MAX_Y_PIXEL / SMALL_SQUARE
#define LVL_AMOUNT 3

// player settings
#define PLAYER_COLOR GREEN
#define GAME_SPEED 1
#define MAX_HP 12
#define INITIAL_SPAWNPOINT_X 58
#define INITIAL_SPANWPOINT_Y 38

// map data
MapInfo map_positions[MAX_X_SMALL_SQUARE + 1][MAX_Y_SMALL_SQUARE + 1];          // necesito el +1 porque el cuadrado map_positions[101][75] existe
int current_lvl;
void (*lvl_pointers[LVL_AMOUNT])() = {create_lvl_0_map, create_lvl_1_map, create_lvl_2_map};                        // points at level creation
int first_spawn;

// player variables
Blocktype last_block_checked = empty;
Direction player_direction = STILL;
Direction last_direction_moved = STILL;
Color hp_color = GREEN;         
Position player_position;
int player_hp;

// game checks
int game_over;

// extra stuff
int my_buffer_position;

// en el eje X entran 102 small_squares (10) y un poquito -> hay 1020 pixeles mas o menos
// en el eje Y entran 76 small_squares (10) y un poquito -> hay 760 pixeles mas o menos

// entonces el tablero esta dividido en X de 0 a 101 y en Y de 0 a 75
// pero el playable tablero va de 0 a 101 y de 0 a 55

void start_new_game(){
    first_spawn = 1;
    current_lvl = 0;
    new_game_title_card(); 
    lvl_pointers[current_lvl]();                            // creates lvl 0
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

void create_lvl_0_map(){
    call_clear_screen();        // en vez de clear screen por ahi es mejor pintar solo el nivel (asi no se pinta devuelta el hp)
    create_walls();             
    
    // blocks west door
    for(int i=29; i<=31; i++){                                          // magic number de las puertas, solucionar
        change_square_color_by_square_position(0, i, BLUE);
        map_positions[0][i].blocktype = map_barrier;
    }

    // create some pillars and stuff
    for(int i=0; i<40; i++){
        change_square_color_by_square_position(20, i, BLUE);
        change_square_color_by_square_position(40, i, BLUE);
        map_positions[20][i].blocktype = map_barrier;
        map_positions[40][i].blocktype = map_barrier;
    }

    if(first_spawn == 1){                                                          
        create_character(INITIAL_SPAWNPOINT_X, INITIAL_SPANWPOINT_Y);               // initial spawnpoint
        first_spawn = 0;
    }
    else if(last_block_checked == east_door){
        draw_character(1, 30);
    }
    else if(last_block_checked == west_door){
        draw_character(MAX_X_SMALL_SQUARE - 1, 30);
    }
    
    create_npc(10, 10);
    create_enemy(25, 10);
    create_healer(80, 10);
    draw_hp();
}

void create_lvl_1_map(){
    call_clear_screen();
    create_walls();
    if(last_block_checked == east_door){
        draw_character(1, 30);
    }
    else if(last_block_checked == west_door){
        draw_character(MAX_X_SMALL_SQUARE - 1, 30);
    }
    draw_bigger_square(50, 30, RED, enemy);
    draw_hp();
}

void create_lvl_2_map(){
    call_clear_screen();
    create_walls();
    if(last_block_checked == east_door){
        draw_character(1, 30);
    }
    else if(last_block_checked == west_door){
        draw_character(MAX_X_SMALL_SQUARE - 1, 30);
    }
    draw_hp();
}

void create_walls(){
    set_map_positions_at_start();                               // sets all squares at empty and black
    for(int i=0; i<MAX_X_SMALL_SQUARE + 1; i++){
        change_square_color_by_square_position(i, 0, BLUE);
        change_square_color_by_square_position(i, MAX_Y_SMALL_SQUARE, BLUE);
        map_positions[i][0].blocktype = map_barrier;
        map_positions[i][MAX_Y_SMALL_SQUARE].blocktype = map_barrier;
    }
    for(int i=0; i<MAX_Y_SMALL_SQUARE + 1; i++){
        if(i < 29 || i > 31){
            change_square_color_by_square_position(0, i, BLUE);
            change_square_color_by_square_position(MAX_X_SMALL_SQUARE, i, BLUE);
            map_positions[0][i].blocktype = map_barrier;
            map_positions[MAX_X_SMALL_SQUARE][i].blocktype = map_barrier;
        }
        else {
            map_positions[0][i].blocktype = west_door;
            map_positions[MAX_X_SMALL_SQUARE][i].blocktype = east_door;
        }
        
    }
}

// this funcion recieves an x (0, 101) and a y (0, 75) and paints it
// tengo que actualizar esta funcion para que reciba un blocktype
void change_square_color_by_square_position(int x_square, int y_square, Color color){
    map_positions[x_square][y_square].color = color;
    putSquare(x_square * SMALL_SQUARE, y_square * SMALL_SQUARE, SMALL_SQUARE, color);

}

void set_map_positions_at_start(){
    for(int x = 0; x <= MAX_X_SMALL_SQUARE; x++){
        for(int y = 0; y <= MAX_Y_SMALL_SQUARE; y++){
            map_positions[x][y].color = BLACK;
            map_positions[x][y].blocktype = empty;
        }
    }
}

void create_character(int x, int y){
    draw_character(x, y);
    player_hp = MAX_HP;
    hp_color = GREEN;
}

void create_npc(int x,  int y){
    change_square_color_by_square_position(x, y, WHITE);
    map_positions[x][y].blocktype = npc;
}

void create_enemy(int x, int y){
    change_square_color_by_square_position(x, y, RED);
    map_positions[x][y].blocktype = enemy;
}

void create_healer(int x, int y){
    change_square_color_by_square_position(x, y, BLUE);
    map_positions[x][y].blocktype = healer;
}

void draw_character(int x, int y){
    last_block_checked = npc;
    change_square_color_by_square_position(x, y, PLAYER_COLOR);
    map_positions[x][y].blocktype = empty;
    player_position.x = x;
    player_position.y = y;
    player_direction = STILL;   
}

void draw_bigger_square(int x_square, int y_square, Color color, Blocktype blocktype){
    for(int i=x_square-1; i<=x_square+1; i++){
        for(int j=y_square-1; j<=y_square+1; j++){
            map_positions[i][j].blocktype = blocktype;
            map_positions[i][j].color = color;
            putSquare(i * SMALL_SQUARE, j * SMALL_SQUARE, SMALL_SQUARE, color);
        }
    }
}

void check_player_input(void) {
	char c;
	call_getBuffPosition(&my_buffer_position);                     
	call_getLastKey(&c, my_buffer_position);       // tengo la teoria que puedo mejorar todo esto usando getC()
	switch (c) {
		case 'w':
			player_direction=NORTH;
            last_direction_moved=NORTH;
            call_fake_sys_read(&c);                 // adds a ' ' to the buffer to prevent contiunous movement
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
    
    MapInfo north_block = map_positions[player_position.x][player_position.y-1];
    MapInfo east_block = map_positions[player_position.x+1][player_position.y];
    MapInfo south_block = map_positions[player_position.x][player_position.y+1];
    MapInfo west_block = map_positions[player_position.x-1][player_position.y];
    
    // check if the player can move
    if(!((west_block.blocktype != empty && player_direction == WEST) || (north_block.blocktype != empty && player_direction == NORTH) || (south_block.blocktype != empty && player_direction == SOUTH) || (east_block.blocktype != empty && player_direction == EAST))){
        change_square_color_by_square_position(player_position.x, player_position.y, BLACK);
        map_positions[player_position.x][player_position.y].blocktype = empty;
        switch (player_direction) {
			case NORTH:            
                player_position.y--;
				break;
			case EAST:           
                player_position.x++;
				break;
			case SOUTH:             
                player_position.y++;
				break;
			case WEST:            
                player_position.x--;
                break;
            default:
                break;    
        }
        map_positions[player_position.x][player_position.y].color = PLAYER_COLOR;
        map_positions[player_position.x][player_position.y].blocktype = npc;
        change_square_color_by_square_position(player_position.x, player_position.y, PLAYER_COLOR);
        player_direction = STILL;
    }
    else if(((west_block.color == RED && player_direction == WEST) || (east_block.color == RED && player_direction == EAST) || (north_block.color == RED && player_direction == NORTH) || (south_block.color == RED && player_direction == SOUTH))){
        damage_player();
    }
    else if(west_block.blocktype == west_door && player_direction == WEST){
        last_block_checked = west_door;
        advance_west_door();                                               
    }
    else if(east_block.blocktype == east_door && player_direction == EAST){
        last_block_checked = east_door;
        advance_east_door();       
    }
}



void game_running(){
    game_over = 0;
    while(!game_over){
        call_sleepms(GAME_SPEED);                   // game's "refresh rate"
        check_player_input();
        move_player();
    }
    game_over_screen();
}

void draw_hp(){

    // drawing the letters HP
    for(int i=63; i<72; i++){
        putSquare(50*SMALL_SQUARE, i*SMALL_SQUARE, SMALL_SQUARE, hp_color);
        putSquare(54*SMALL_SQUARE, i*SMALL_SQUARE, SMALL_SQUARE, hp_color);
        putSquare(57*SMALL_SQUARE, i*SMALL_SQUARE, SMALL_SQUARE, hp_color);
    }

    for(int i=51; i<54; i++){
        putSquare(i*SMALL_SQUARE, 67*SMALL_SQUARE, SMALL_SQUARE, hp_color);
    }

    putSquare(58*SMALL_SQUARE, 63*SMALL_SQUARE, SMALL_SQUARE, hp_color);
    putSquare(59*SMALL_SQUARE, 63*SMALL_SQUARE, SMALL_SQUARE, hp_color);
    putSquare(58*SMALL_SQUARE, 66*SMALL_SQUARE, SMALL_SQUARE, hp_color);
    putSquare(59*SMALL_SQUARE, 66*SMALL_SQUARE, SMALL_SQUARE, hp_color);
    putSquare(60*SMALL_SQUARE, 64*SMALL_SQUARE, SMALL_SQUARE, hp_color);
    putSquare(60*SMALL_SQUARE, 65*SMALL_SQUARE, SMALL_SQUARE, hp_color);


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
    if(player_hp <= 0){
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
    call_sleepms(200);
    putString("i", RED);
    call_sleepms(200);
    putString("l", RED);
    call_sleepms(200);
    putString("l", RED);
    setFontSize(2);
    call_sleepms(300);
    putString("  you\n", WHITE);
    call_sleepms(400);
    putString("\nI will take your soul\n", WHITE);
    call_sleepms(600);
    clearScreen();
}

void draw_wall_conversation(){
    clearScreen();
    setFontSize(4);
    call_sleepms(100);
    putString("A Wall, interesting...", WHITE);
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
    putString("30 hp restored\n", GREEN);
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

void advance_east_door(){
    if(current_lvl < LVL_AMOUNT - 1){                   // this shouldn't happen anyways
        current_lvl++;
        lvl_pointers[current_lvl]();    
    }    
}

void advance_west_door(){
    if(current_lvl > 0){                   // this shouldn't happen anyways
        current_lvl--;
        lvl_pointers[current_lvl]();    
    }   
}

