#include <naiveConsole.h>
#include <keyboardDriver.h>
#include <videoDriver.h>
#include <keyboardBuffer.h>

static char ScanCodes[256]={0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=',
'\b', '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', '+', '\n', '`', 'a', 's', 
'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', 0, 0, '-', 0, 0, 0, ' ' };

int getKey();
static int ctrlPressed = 0;
static int shiftPressed = 0;


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
        //zoomIn();
        zoomIn();
    } else if (ctrlPressed && (key == 0x1B)) { // Minus key on the numeric keypad
        //zoomOut();
        zoomOut();
    }

	if( key == 0x2A){					// shift make
		shiftPressed = 1;
	} else if (key == 0xAA){			// shift break	
		shiftPressed = 0;
	}


	if(key == 0x39){ // space										// podriamos no agregar el space al buffer no?											// cosa para arreglar meter el moveCursorFront() en space() pero hay que entender mejor
		addBuffer(ScanCodes[key]);									// al drawChar()
		addScreen(ScanCodes[key]);
        space();
		return;
	}
	else if(ScanCodes[key] == '\b'){		// backspace
		deleteBuffer();
		backspace();
		return;
	}
	else if(ScanCodes[key] == '\t'){		// tab
		addBuffer(ScanCodes[key]);
		addScreen(ScanCodes[key]);
        tab();
		return;
	}
	else if(ScanCodes[key] == '\n'){		// enter
		addScreen(ScanCodes[key]);
		commands();
		clearBuffer();
		return;
	}
	else if(key>=0 && key<=256 && ScanCodes[key] != 0 ){
		if (ctrlPressed) {
			return;
		}
        if ((!capsLockOn && shiftPressed) || ((capsLockOn && !shiftPressed))) {
			drawChar(0X00FF00, ScanCodes[key] - ('a' - 'A'));
			addScreen(ScanCodes[key] - ('a' - 'A'));
			addBuffer(ScanCodes[key] - ('a' - 'A'));
        } else {
			drawChar(0x00FF00, ScanCodes[key]);
			addScreen(ScanCodes[key]);
			addBuffer(ScanCodes[key]);
        }
		
		return;
	}
}