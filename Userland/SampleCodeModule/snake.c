// C program to build the complete 
// snake game 
#include <stdlib.h> 
#include <unistd.h>

#include <utils.h>

struct snake *Snake;
struct apple *Apple;
int dir[2] = {0, -1};

// Function to generate the fruit 
// within the boundary 
void setup() 
{ 
	gameover = 0; 

	// Stores height and width 
	x = height / 2; 
	y = width / 2; 
label1: 
    fruitx = 2;
	//fruitx = rand() % 20; 
	if (fruitx == 0) 
		goto label1; 
label2: 
    fruity = 2;
	//fruity = rand() % 20; 
	if (fruity == 0) 
		goto label2; 
	score = 0; 
} 

// Function to draw the boundaries 
void draw() 
{ 
	for (i = 0; i < height; i++) { 
		for (j = 0; j < width; j++) { 
			if (i == 0 || i == width - 1 || j == 0 || j == height - 1) { 
                    putString("#");
			} 
			else { 
				if (i == x && j == y) 
                    putString("0");
				else if (i == fruitx && j == fruity) 
                    putString("*");
				else
                    putString(" ");
			} 
		} 
        putString("\n");
	} 

	// Print the score after the 
	// game ends
    printF("score = %d", score);
    putString("\n");
	putString("press X to quit the game");
}

// Function to take the input 
void input() 
{ 
    char c = getC();
    switch (c) { 
    case 'a': 
        flag = 1; 
        break; 
    case 's': 
        flag = 2; 
        break; 
    case 'd': 
        flag = 3; 
        break; 
    case 'w': 
        flag = 4; 
        break; 
    case 'x': 
        gameover = 1; 
        break; 
    }
} 

// Function for the logic behind 
// each movement 
void logic() 
{ 
	//sleep(0.01); 
	switch (flag) { 
	case 1: 
		y--; 
		break; 
	case 2: 
		x++; 
		break; 
	case 3: 
		y++; 
		break; 
	case 4: 
		x--; 
		break; 
	default: 
		break; 
	} 

	// If the game is over 
	if (x < 0 || x > height 
		|| y < 0 || y > width) 
		gameover = 1; 

	// If snake reaches the fruit 
	// then update the score 
	if (x == fruitx && y == fruity) { 
	label3:
        fruitx = 2;
		//fruitx = rand() % 20; 
		if (fruitx == 0) 
			goto label3; 

	// After eating the above fruit 
	// generate new fruit 
	label4: 
        fruity = 2;
		//fruity = rand() % 20; 
		if (fruity == 0) 
			goto label4; 
		score += 10; 
	} 
} 

// Driver Code 
void snake() 
{ 
	//int m, n; 

	// Generate boundary 
	setup(); 

	// Until the game is over 
	while (!gameover) { 

		// Function Call 
		draw(); 
		input(); 
		logic(); 
	} 
} 
