#include <naiveConsole.h>
#include <keyboardDriver.h>
#include <videoDriver.h>
#include <keyboardBuffer.h>

static char ScanCodes[256]={0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=',
'\b', '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0, 'a', 's', 
'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', 0, 0 };

int getKey();


void keyboard_handler(){
	int key = getKey();
	
    static int capsLockOn = 0;

    if (key == 0x3A) {
        capsLockOn = 1 - capsLockOn;
    }


	if(key == 0x39){ // space
		clearBuffer();
        space();
		return;
	}
	if(ScanCodes[key] == '\b'){
		deleteBuffer();
		backspace();
		return;
	}
	if(ScanCodes[key] == '\t'){
		clearBuffer();
        tab();
		return;
	}
	if(ScanCodes[key] == '\n'){
		checkCommands();
		clearBuffer();
		newline();
		return;
	}
	if( key>=0 && key<=256 && ScanCodes[key] != 0 ){
        if (capsLockOn) {
			drawChar(0XFF0000, ScanCodes[key] - ('a' - 'A'));
			addBuffer(ScanCodes[key]);
            //ncPrintChar(ScanCodes[key] - ('a' - 'A'));
        } else {
			drawChar(0xFF0000, ScanCodes[key]);
			addBuffer(ScanCodes[key]);
		    //ncPrintChar(ScanCodes[key]);
        }
		return;
	}
}