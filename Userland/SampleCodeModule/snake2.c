#include <utils.h>
#include <colors.h>
#include <UserSyscalls.h>
#include <snake2.h>

// map stuff (shouldn't get updated)
int squareSize = 10;
int squareSpace = 1;
int mapSize = 32;              
int snake_speed = 20;

// player stuff
int player1_size = 0;
int player2_size = 0;
int score_player_1 = 0;
int score_player_2 = 0;
int top_score = 0;

// game setup stuff
int amount_of_players;
int player1_alive;
int player2_alive;
int finish_game = 0;

// other stuff
int seed_count1 = 0;
int buffer_position;


struct position{
    int x;
    int y;
};

typedef struct position Position;

// snake positions
Position posHead1;
Position posHead2;
Position posBody1[100];                     // array with every position of the body
Position posBody2[100];                     // array with every position of the body
Position lastTail1;
Position lastTail2;

Position fruitPosition;


// directions    1: up    2: right    3: left     4: right
int player1_direction = 2;          // player 1 starts going right
int player2_direction = 1;          // player 2 starts going up

void start_snake2(){
    snake2_title_card();
}

void snake2_title_card(){
    int startGame=0;
    finish_game = 0;
    char c;
    clearScreen();

    while(startGame==0){
        setFontSize(10);
        own_printf("   SNAKE 2\n\n\n\n");
        setFontSize(4);
        putString(" Single Player:   Press 1\n Multiplayer:     Press 2\n", WHITE);
        setFontSize(1);
        putNewLine();
        
        c = getC();
        if (c == '1'){
            startGame = 1;
        }
        if (c == '2'){
            startGame = 2;
        }

    }
    
    clearScreen();
    create_map();

    if(startGame == 1){
        singleplayer();
    }
    if(startGame == 2){
        multiplayer();
    }
    
}

void create_map(){
    for(int i=0; i<mapSize+1; i++){
        fill_snake_square(i, 0, WHITE);
        fill_snake_square(i, mapSize, WHITE);
    }

    for(int i=0; i<mapSize+1; i++){
        fill_snake_square(0, i, WHITE);
        fill_snake_square(mapSize, i, WHITE);
    }
}



void spawn_player_1(){
    // create head and body
    player1_size = 1;
    score_player_1 = 0;
    posHead1.x = mapSize / 2;              
    posHead1.y = mapSize / 4;               
    posBody1[0].x = posHead1.x;
    posBody1[0].y = posHead1.y;
}


void singleplayer(){
    singleplayer_startup();
    while(finish_game==0){
        call_sleepms(snake_speed);              // sleep function
        check_input();                          // changes direction (or doesnt)
        move_snake();                           // moves array of snake (where everything is) (loses tail)
        if(check_fruit_position()==0){
            deleteTail();                        // checks if fruit is catched and updates snake
        }
        else{
            drawFruit();
        }
        check_player_crash();          // draw the next snake only if it didnt crash
        drawSnakes();                       // draw snakes after moving
        check_gameover_singleplayer();               
    }
    gameover_titlecard();
}

void check_gameover_singleplayer(){
    if(player1_alive == 0){
        finish_game = 1;
    }
}



void multiplayer(){
    spawn_player_1();
    spawn_player_2();
    amount_of_players = 2;
    placeRandomFruit();
    gamePaused();               // the game doesnt start until players press 'p'
}

void singleplayer_startup(){
    amount_of_players = 1;
    player1_alive = 1;
    spawn_player_1();
    placeRandomFruit();
    // gamePaused();               // por ahi lo termino sacandol
    drawSnakes();
    drawFruit();

}

void drawSnakes(){
    // draw player1
    if(player1_alive == 1){
        for(int i=0; i<player1_size; i++){
            fill_snake_square(posBody1[i].x, posBody1[i].y, BLUE);
        }
    }
    

    // draw player2
    if(amount_of_players == 2 && player2_alive == 1){
        for(int i=0; i<player2_size; i++){
            fill_snake_square(posBody2[i].x, posBody2[i].y, RED);
        }
    }
}



void deleteTail(){
    fill_snake_square(lastTail1.x, lastTail1.y, BLACK);
}

// not sure how this works but it returns a random position of the map for the fruit to be in
int custom_rand1() {
	int numeros[] = {56, 34, 72, 19, 88, 45, 27, 63, 11, 75,
                    90, 30, 12, 50, 42, 67, 22, 98, 14, 61,
                    25, 84, 37, 70, 17, 93, 76, 29, 58, 39,
                    66, 21, 83, 52, 15, 97, 41, 89, 33, 60}; 
	seed_count1 = (seed_count1 + 1) % (sizeof(numeros) / sizeof(numeros[0]));
    numeros[seed_count1] = (214013 * numeros[seed_count1] + 2531011);
    return (numeros[seed_count1] >> 16) & 0x7FFF;
}

void placeRandomFruit(void){
    int maxTries = 100;
    for(int try = 0; try < maxTries; try++){
        fruitPosition.x = custom_rand1() % mapSize - 1;
        fruitPosition.y = custom_rand1() % mapSize - 1;

        int overlap = 0;

        for(int i = 0; i<player1_size; i++){
            if((posBody1[i].x == fruitPosition.x && posBody1[i].y == fruitPosition.y) || (fruitPosition.x == 0 || fruitPosition.y == 0)){
                overlap = 1;
            }
        }
        

        if(amount_of_players == 2){
            for(int i=0; i<player2_size; i++){
                if(posBody2[i].x == fruitPosition.x && posBody2[i].y == fruitPosition.y){
                    overlap = 1;
                }
            }
        }

        if(overlap == 0){
            break;
        }

    }
}

void check_player_crash(){
    if(amount_of_players == 1){
        if(posHead1.x == 0 || posHead1.x == mapSize || posHead1.y == 0 || posHead1.y == mapSize){       // check p1 out of bounds
            kill_player_1();
            singleplayer_gameover();
        }
        for(int i = 1; i<player1_size; i++){
            if(posHead1.x == posBody1[i].x && posHead1.y == posBody1[i].y){         // check if p1 crashed against itself
                kill_player_1();
                singleplayer_gameover();
                
            }
        }
    }

    if(amount_of_players == 2){
        if(posHead1.x == 0 || posHead1.x == mapSize || posHead1.y == 0 || posHead1.y == mapSize){       // check if p1 crashed against edge
            if(player1_alive == 1){
                kill_player_1();
            }
        }
        if(posHead2.x == 0 || posHead2.x == mapSize || posHead2.y == 0 || posHead2.y == mapSize){       // check if p2 crashed against edge
            if(player2_alive == 1){
                kill_player_2();
            }
        }
        for(int i=0; i<player2_size; i++){                                          
            if(posHead1.x == posBody2[i].x && posHead1.y == posBody2[i].y){         // check if p1 crashed against p2
                if(player1_alive == 1){
                    kill_player_1();
                    player1_alive = 0;
                }
                
            }
            
            if(i!=0 && posHead2.x == posBody2[i].x && posHead2.y == posBody2[i].y ){         // check if p2 crashed against itself
                if(player2_alive == 1){
                    kill_player_2();
                    player2_alive = 0;
                }
                
            }
        }
        for(int i=0; i<player1_size; i++){                                         
            if(posHead2.x == posBody1[i].x && posHead2.y == posBody1[i].y){         // check if p2 crashed against p1
                if(player2_alive == 1){
                    kill_player_2();
                    player2_alive = 0;
                }
            }
            if(posHead1.x == posBody1[i].x && posHead1.y == posBody1[i].y){         // check if p1 crashed against itself
                if(player1_alive == 1){
                    kill_player_1();
                    player1_alive = 0;
                }
            }
        }

        if(player1_alive == 0 && player2_alive == 0){
            multiplayer_gameover();
        }
    }
}

int check_fruit_position(){
    if(posHead1.x == fruitPosition.x && posHead1.y == fruitPosition.y){
        player1_size++;
        posBody1[player1_size-1] = lastTail1;
        score_player_1 += 10;
        placeRandomFruit();
        return 1;
    }

    if(amount_of_players == 2 && posHead2.x == fruitPosition.x && posHead2.y == fruitPosition.y){
        player2_size++;
        posBody2[player2_size-1] = posBody2[player2_size-2];
        score_player_2 += 10;
        placeRandomFruit();
        return 1;
    }

    return 0; // no fruit was found
    

}



void kill_player_1(){
    for(int i=0; i<player1_size; i++){
        fill_snake_square(posBody1[i].x, posBody1[i].y, BLACK);             // deletes tail
        posBody1[i].x = '\0';                                               // delete its info
        posBody1[i].y = '\0';
    }
    player1_size = 0;
    player1_alive = 0;
    lastTail1.x = '\0';
    lastTail1.y = '\0';
}

void kill_player_2(){
    for(int i=0; i<player2_size; i++){
        fill_snake_square(posBody2[i].x, posBody2[i].y, BLACK); 
        posBody2[i].x = '\0';
        posBody2[i].y = '\0';
    }
    player2_size = 0;
    player2_alive = 0;
}




void drawFruit(){
    fill_snake_square(fruitPosition.x, fruitPosition.y, RED);
}

void check_input(void) {
	char c;
	call_getBuffPosition(&buffer_position);         // que pingo hace esto.
	// if (prev_pos < buffer_pos){
		// prev_pos = buffer_pos;                   // esto va a romper el codigo pero no se que hace
		call_getLastKey(&c, buffer_position);       // TESTEAR MOVIMIENTO
		switch (c) {
			case 'w':
				if(player1_direction!=3)
					player1_direction=1;
				break;
			case 'd':
				if(player1_direction!=4)
					player1_direction=2;
				break;
			case 's':
				if(player1_direction!=1)
					player1_direction=3;
				break;
			case 'a':
				if(player1_direction!=2)
					player1_direction=4;
					break;
			case 'i':
				if(player2_direction!=3)
					player2_direction=1;
				break;
			case 'l':
				if(player2_direction!=4)
					player2_direction=2;
				break;
			case 'k':
				if(player2_direction!=1)
					player2_direction=3;
				break;
			case 'j':
				if(player2_direction!=2)
					player2_direction=4;
					break;
		}
	// }
}

void move_snake(void) {
    // every part of the body becomes the next one in the array except for the head
    lastTail1 = posBody1[player1_size-1];   // save the last position of the tail incase player catches a fruit
    for(int i=player1_size-1; i>0; i--){
        posBody1[i] = posBody1[i-1];
    }

    // for(int i=0; i<player1_size - 1; i++){
    //     posBody1[player1_size - i - 1] = posBody1[player1_size - i - 2];
    // }

    // how the head moves

    switch (player1_direction) {
			case 1:
                posHead1.y--;           // no creo pero OJO que por ahi esto tiene que ser un puntero
				break;
			case 2:
                posHead1.x++;
				break;
			case 3:
                posHead1.y++;
				break;
			case 4:
                posHead1.x--;
                break;
            default:
                break;          // this should never happen
	}

    posBody1[0] = posHead1;     // update head position
	
	
    if(amount_of_players == 2 && player2_alive == 1){
        
        // every part of the body becomes the previous one except for the head
        lastTail2 = posBody2[player2_size-1];                   // save the last position of the tail incase player catches a fruit
        for(int i=player1_size-1; i>0; i--){
            posBody1[i] = posBody1[i-1];
        }

        // how the head moves
        switch (player2_direction) {
			case 1:
                posHead2.y--;
				break;
			case 2:
                posHead2.x++;
				break;
			case 3:
                posHead2.y++;
				break;
			case 4:
                posHead2.x--;
            default:                                // this should never happen
                break;          
	    }

        posBody2[0] = posHead2;     // update head position

    }

	
}

void singleplayer_gameover(){
    
    finish_game = 1;
}

void multiplayer_gameover(){

}

// le pasas un x e y de snake y te lo rellena para el jueguito
void fill_snake_square(int x, int y, int color){
    putSquare(x*squareSize + x*squareSpace, y*squareSize + y*squareSpace, squareSize, color);
}

void spawn_player_2(){ 
    posHead2.x = mapSize * squareSize / 4;
    posHead2.y = mapSize * squareSize / 2;
    posBody2[0].x = posHead2.x;
    posBody2[0].y = posHead2.y;
}

void gamePaused(){
    int gamePaused = 1;
    char c;
    while(gamePaused == 1){
        c = getC();
        if(c == 'p' || c == 'P'){
            gamePaused = 0;
        }
    }

}

void gameover_titlecard(){
    call_sleepms(200);
	call_clear_screen();
    setFontSize(3);
	if(amount_of_players == 1){
		putString("Your final Score: ", WHITE);
		putInt(score_player_1);
		if(score_player_1 > top_score){
			top_score = score_player_1;
		}
		putString("\n\nHighscore: ", WHITE);
		putInt(top_score);
        
	}
	if(amount_of_players == 2){
		putString("Player 1 final Score: ", WHITE);
		putInt(score_player_1);
        putNewLine();
		putString("Player 2 final Score: ", WHITE);
		putInt(score_player_2);
		if (score_player_1>score_player_2) putString("\n\n\n\nThe winner is Player 1!!\n\n\n\n", WHITE);
		if (score_player_1<score_player_2) putString("\n\n\n\nThe winner is Player 2!!\n\n\n\n", WHITE);
		if (score_player_1==score_player_2) putString("\n\n\n\nThere was a tie!!\n\n\n\n", WHITE);
	}
    putNewLine();
	putNewLine();
	putNewLine();
	score_player_1 = 0;
	score_player_2 = 0;
	putString("\n\nThank You for playing!!\n\n\n\n", WHITE);
    setFontSize(1);
	call_sleepms(1000);
	call_clear_screen();
}