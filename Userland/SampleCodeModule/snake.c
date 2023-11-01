#include <utils.h>
#include <colors.h>

int headX,headY,fruitX,fruitY,width=30,height=30,gameOver=0,direction=2,speeds=0.8i,size=0,score=0,speed=100000;
int bodyX[30],bodyY[30];

void render(void);
void setupGame(void);
void inputCheck(void);
void moveSnake(void);
void gameOverCheck(void);
void placeFruit(void);
void fruitCheck(void);

void snake() {
	setupGame();
	placeFruit();
	while(!gameOver) {
		render();
		inputCheck();
		moveSnake();
		fruitCheck();
		gameOverCheck();
		//usleep(speed);
  	}
	//usleep(speed);
	//system("clear");
	putString("\n\n\n\n\t\t\tYour final Score: ", WHITE);
	printF("%d", score);
	putString("\n\n\t\t\tThank You for playing!! (press any key to exit)\n\n\n\n", WHITE);
	// char c = getC();
	// while(c != 0);
	//system("clear");
}

void render(void) {
	//system("clear");
	int i,j,k,p;
	for(j=0;j<=height;j++) {
		for(i=0;i<=width;i++) {
			p=1;
			if(i==0||j==0||j==height||i==width)
				putString("*", WHITE);
			else if(i==fruitX&&j==fruitY)
				putString("0", WHITE);
			else if(i==headX&&j==headY)
				putString("o", WHITE);
			else {
				for(k=0;k<size;k++) {
					if(i==bodyX[k] && j==bodyY[k]) {
						putString("+", WHITE);
						p=0;
					}
				}
				if(p)
					putString(" ", WHITE);
			}
		}
		putNewLine();
	}
	putString("Scored: ", WHITE);
	printF("%d", score);
}

void placeFruit(void) {
	resetfruitX:fruitX=18;
	//resetfruitX:fruitX=rand()%20;
	if(fruitX==0||fruitX==width)
	goto resetfruitX;

	resetfruitY:fruitY=10;
	//resetfruitY:fruitY=rand()%20;
	if(fruitY==0||fruitY==height)
	goto resetfruitY;
}

void setupGame(void) {
	headX=height/2;
	headY=width/2;
}

void fruitCheck(void) {
 	if(headX==fruitX && headY==fruitY) {
		score+=10;
		size++;
		if(speed>50000)
		speed-=500;
		placeFruit();
	}
}

void inputCheck(void) {
	char c = getC();
 	if(c) {
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
}

void gameOverCheck(void) {
	int i;
	for(i=0;i<size;i++) {
		if(headX==bodyX[i] && headY==bodyY[i])
			gameOver=1;
	}
	if(headX==width||headX==0||headY==height||headY==0)
		gameOver=1;
}