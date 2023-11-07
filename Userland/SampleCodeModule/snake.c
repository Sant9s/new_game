#include <utils.h>
#include <colors.h>
#include <UserSyscalls.h>

int headX, headY, fruitX, fruitY, headX2, headY2;
int width=30;
int height=30;
int gameOver=0;
int direction=2;
int direction2=4;
int size=0;
int size2=0;
int score=0;
int score2=0;
int speed=120;
int bodyX[100];
int bodyY[100];
int bodyX2[100];
int bodyY2[100];
int seed_count = 0;
int buffer_pos;
int prev_pos = 0;
int players;

void render(void);
void setupGame(void);
void inputCheck(void);
void moveSnake(void);
void gameOverCheck(void);
void placeFruit(void);
void fruitCheck(void);
int custom_rand();
void goodbye();
void render2(void);
void setupGame2(void);
void inputCheck2(void);
void moveSnake2(void);
void gameOverCheck2(void);
void placeFruit2(void);
void fruitCheck2(void);
void goodbye2();

void snake() {
	players = 1;
	setupGame();
	placeFruit();
	while(!gameOver) {
		clearScreen();
		render();
		inputCheck();
		moveSnake(); 
		gameOverCheck();
		fruitCheck();
		call_sleepms(speed);
  	}
	goodbye();
}

void goodbye(){
	call_sleepms(200);
	call_clear_screen();
	if(players == 1){
		putString("\n\n\n\n\t\tYour final Score: ", WHITE);
		putInt(score);
	}
	if(players == 2){
		putString("\n\n\n\n\t\tPlayer 1 final Score: ", WHITE);
		putInt(score);
		putString("\n\n\n\n\t\tPlayer 2 final Score: ", WHITE);
		putInt(score2);
		if (score>score2) putString("\n\n\t\tThe winner is Player 1!!\n\n\n\n", WHITE);
		if (score<score2) putString("\n\n\t\tThe winner is Player 2!!\n\n\n\n", WHITE);
		if (score==score2) putString("\n\n\t\tThere was a tie!!\n\n\n\n", WHITE);
		putNewLine();
		putNewLine();
		putNewLine();
	}
	score = 0;
	score2 = 0;
	putString("\n\n\t\tThank You for playing!!\n\n\n\n", WHITE);
	call_sleepms(700);
	call_clear_screen();
}



void snake_multiplayer() {
	players = 2;
	setupGame();
	placeFruit();
	while(!gameOver) {
		clearScreen();
		render();
		inputCheck();
		moveSnake(); 
		gameOverCheck();
		fruitCheck();
		call_sleepms(speed);
  	}
	goodbye();
}

void render(void) {
    call_clear_screen();
    int i, j, k, p;
    for (j = 0; j <= height; j++) {
        for (i = 0; i <= width; i++) {
            p = 1;
            if (i == 0 || j == 0 || i == width || j == height) {
                putString("*", WHITE);
            } else if (i == fruitX && j == fruitY && !(headX==fruitX && headY==fruitY)){
				if(players == 1){
					putString("0", RED);
				}
				else if((players == 2) && !(headX2==fruitX && headY2==fruitY)){
					putString("0", RED);
				}    
            } else if (i == headX && j == headY) {
                putString("o", WHITE);
			}else if((players == 2) && (i == headX2 && j == headY2)){
				putString("o", BLUE);
            } else {
                for (k = 0; k < size; k++) {
                    if ((i == bodyX[k] && j == bodyY[k])) {
                        putString("+", WHITE);
                        p = 0;
                        break;
                    }

                }
				if(players == 2){
					for (k = 0; k < size2; k++) {
                    if (i == bodyX2[k] && j == bodyY2[k]) {
                        putString("+", BLUE);
                        p = 0;
                        break;
                    }
                }
				}
				
                if (p) {
                    putString(" ", WHITE);
                }
            }
        }
        putNewLine();
    }
	if(players == 1){
		putString("Your Score: ", WHITE);
    	putIntColor(score, WHITE);
	}
	if(players == 2){
		putString("Player 1 score: ", WHITE);
    	putInt(score);
		putString("\nPlayer 2 score: ", WHITE);
    	putInt(score2);
	}
    
}

void placeFruit(void) {
    int maxTries = 100; // Límite de intentos para evitar bucles infinitos
    for (int try = 0; try < maxTries; try++) {
        fruitX = custom_rand() % (width - 2) + 1;
        fruitY = custom_rand() % (height - 2) + 1;

        // Asegúrate de que la fruta no se superponga con el cuerpo de la serpiente
        int overlap = 0;
        for (int i = 0; i < size; i++) {
			if((players == 2) && (fruitX == bodyX2[i] && fruitY == bodyY2[i])){			// no me gusta que este checkeando constantemente si hay 2 jugadores
				overlap = 1;
                break;
			}
            if (fruitX == bodyX[i] && fruitY == bodyY[i]) {
                overlap = 1;
                break;
            }
        }

        if (!overlap) {
            break; // Ubicación válida para la fruta
        }
    }
}



// Random number in the range [0, RAND_MAX]
int custom_rand() {
	int numeros[] = {56, 34, 72, 19, 88, 45, 27, 63, 11, 75,
                    90, 30, 12, 50, 42, 67, 22, 98, 14, 61,
                    25, 84, 37, 70, 17, 93, 76, 29, 58, 39,
                    66, 21, 83, 52, 15, 97, 41, 89, 33, 60}; 
	seed_count = (seed_count + 1) % (sizeof(numeros) / sizeof(numeros[0]));
    numeros[seed_count] = (214013 * numeros[seed_count] + 2531011);
    return (numeros[seed_count] >> 16) & 0x7FFF;
}

void setupGame(void) {
	if(players == 2){
		headX=height/3;
		headY=width/3;
		headX2=2*height/3;
		headY2=2*width/3;
		size2 = 0;
	}
	if(players == 1){
		headX=height/2;
		headY=width/2;
	}
	
	call_getBuffPosition(&buffer_pos);
	width=30;
	height=30;
	gameOver=0;
	direction=2;
	size=0;
	score=0;
	speed=50;
	seed_count = 0;
	prev_pos = 0;
}



void fruitCheck(void) {
	if((players == 2) && (headX2 == fruitX && headY2 == fruitY)){
		score2 += 10;
		size2++;
		placeFruit();

	}
	else if(headX==fruitX && headY==fruitY) {
		score+=10;
		size++;
		placeFruit();
	}
}

void inputCheck(void) {
	char c;
	call_getBuffPosition(&buffer_pos);
	if (prev_pos < buffer_pos){
		prev_pos = buffer_pos;
		call_getLastKey(&c, buffer_pos);
		switch (c) {
			case 'w':
				if(direction!=3)
					direction=1;
				break;
			case 'd':
				if(direction!=4)
					direction=2;
				break;
			case 's':
				if(direction!=1)
					direction=3;
				break;
			case 'a':
				if(direction!=2)
					direction=4;
					break;
			case 'i':
				if(direction2!=3)
					direction2=1;
				break;
			case 'l':
				if(direction2!=4)
					direction2=2;
				break;
			case 'k':
				if(direction2!=1)
					direction2=3;
				break;
			case 'j':
				if(direction2!=2)
					direction2=4;
					break;
		}
	}
}

void moveSnake(void) {
	int x1,x2,y1,y2,i;
	if(size==1){
		bodyX[0]=headX;
		bodyY[0]=headY;
	} else {
		x1=headX;
		y1=headY;
		for(i=0;i<size;i++) {
			x2=bodyX[i];
			y2=bodyY[i];
			bodyX[i]=x1;
			bodyY[i]=y1;
			x1=x2;
			y1=y2;
		}
	}
	switch (direction) {
		case 1:
			headY--;
			break;
		case 2:
			headX++;
			break;
		case 3:
			headY++;
			break;
		case 4:
			headX--;
	}
	if(players == 2){
		if(size2==1){
		bodyX2[0]=headX2;
		bodyY2[0]=headY2;
	} else {
		x1=headX2;
		y1=headY2;
		for(i=0;i<size2;i++) {
			x2=bodyX2[i];
			y2=bodyY2[i];
			bodyX2[i]=x1;
			bodyY2[i]=y1;
			x1=x2;
			y1=y2;
		}
	}
	switch (direction2) {
		case 1:
			headY2--;
			break;
		case 2:
			headX2++;
			break;
		case 3:
			headY2++;
			break;
		case 4:
			headX2--;
	}
	}
}


void gameOverCheck(void) {
	// check if player1 is out of bounds
	if(headX==width||headX==0||headY==height||headY==0){
		gameOver = 1;
		return;
	}

	// check if player1 crashed againts itself
	if(players == 1){
		for(int i=0;i<size-1;i++) {
		if(headX==bodyX[i] && headY==bodyY[i])
			gameOver=1;
	}
	}
	

	if(players == 2){
		// check if player 2 is out of bounds
		if(headX2==width||headX2==0||headY2==height||headY2==0){
			gameOver=1;
			return;
		}

		for(int i=0; i < size-1; i++){
				if((headX2==bodyX[i] && headY2==bodyY[i]) || (headX==bodyX[i] && headY==bodyY[i])){
				gameOver=1;		
				return;
			}	
		}
		
		for(int i=0;i<size2-1;i++) {
			if((headX==bodyX2[i] && headY==bodyY2[i]) || (headX2==bodyX2[i] && headY2==bodyY2[i]) ){		
				gameOver = 1;
				return; 
			}
		}
		
	}
}