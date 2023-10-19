#include <naiveConsole.h>
#include <keyboardDriver.h>
#include <videoDriver.h>
#include <keyboardBuffer.h>

static char ScanCodes[256]={0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=',
'\b', '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', '+', '\n', 0, 'a', 's', 
'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', 0, 0, '-', 0, 0, 0, ' ' };

int getKey();
static int ctrlPressed = 0;


void keyboard_handler(){
	int key = getKey();
	
    static int capsLockOn = 0;

    if (key == 0x3A) {
        capsLockOn = 1 - capsLockOn;
    }

	if (key == 0x1D) {
		ctrlPressed = 1;
	} else if (key == 0x9D) {
		ctrlPressed = 0;
	}

	if (ctrlPressed && (key == 0x35)) { // Plus key on the numeric keypad
        zoomIn();
    } else if (ctrlPressed && (key == 0x1B)) { // Minus key on the numeric keypad
        zoomOut();
	}


	if(key == 0x39){ // space
		addBuffer(ScanCodes[key]);
		addScreen(ScanCodes[key]);
        space();
		return;
	}
	if(ScanCodes[key] == '\b'){
		deleteBuffer();
		backspace();
		return;
	}
	if(ScanCodes[key] == '\t'){
		addBuffer(ScanCodes[key]);
		addScreen(ScanCodes[key]);
        tab();
		return;
	}
	if(ScanCodes[key] == '\n'){
		addScreen(ScanCodes[key]);
		commands();
		clearBuffer();
		newline();
		return;
	}
	if( key>=0 && key<=256 && ScanCodes[key] != 0 ){
		if ((key == 0x35 || key == 0x1B) && ctrlPressed) {
			return;
		}
        if (capsLockOn) {
			drawChar(0XFF0000, ScanCodes[key] - ('a' - 'A'));
			addScreen(ScanCodes[key] - ('a' - 'A'));
			addBuffer(ScanCodes[key] - ('a' - 'A'));
        } else {
			drawChar(0xFF0000, ScanCodes[key]);
			addScreen(ScanCodes[key]);
			addBuffer(ScanCodes[key]);
        }
		return;
	}
}