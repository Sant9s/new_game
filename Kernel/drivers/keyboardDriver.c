#include <naiveConsole.h>
#include <keyboardDriver.h>
#include <videoDriver.h>
#include <keyboardBuffer.h>
#include "./include/scanCodes.h"

int getKey();
//static int ctrlPressed = 0;


void keyboard_handler(){
	int key = getKey();
	uint16_t * buff = getBuff();
	int buffPos = getBuffPosition();
	uint16_t * screen = getScreen();
	int screenPos = getScreenPosition();

	if (buffPos + 1 < BUFF){
        setBuffPos(buffPos + 1); 
        buff[buffPos + 1] = 0;  
    } else {
        setBuffPos(0);  
        buff[0] = 0;  
    }

	if (screenPos + 1 < SCREEN){
        setScreenPos(screenPos + 1); 
        screen[screenPos + 1] = 0;  
    } else {
        setScreenPos(0);  
        screen[0] = 0;  
    }

	buff[buffPos] = key;
	screen[screenPos] = key;

	
    // static int capsLockOn = 0;

    // if (key == 0x3A) {
    //     capsLockOn = 1 - capsLockOn;
    // }

	// if (capsLockOn) {
	// 	upperCase();
	// } else {
	// 	lowerCase();
	// }

	// if (key == 0x1D) {
	// 	ctrlPressed = 1;
	// } else if (key == 0x9D) {
	// 	ctrlPressed = 0;
	// }

	// if (ctrlPressed && (key == 0x35)) { // Plus key on the numeric keypad
    //     zoomIn();
    // } else if (ctrlPressed && (key == 0x1B)) { // Minus key on the numeric keypad
    //     zoomOut();
	// }
}