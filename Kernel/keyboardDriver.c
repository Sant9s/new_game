#include <naiveConsole.h>
#include <keyboardDriver.h>

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
		ncPrint(" ");
		return;
	}
	if(ScanCodes[key] == '\b'){
		ncBackspace();
		return;
	}
	if(ScanCodes[key] == '\t'){
		ncPrint("   ");
		return;
	}
	if(ScanCodes[key] == '\n'){
		ncNewline();
		return;
	}
	if( key>=0 && key<=256 && ScanCodes[key] != 0 ){
        if (capsLockOn) {
            ncPrintChar(ScanCodes[key] - ('a' - 'A'));
        } else {
		    ncPrintChar(ScanCodes[key]);
        }
		return;
	}
}