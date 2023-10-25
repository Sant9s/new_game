#include <sysCalls.h>
#include <naiveConsole.h>
#include <videoDriver.h>
#include <keyboardBuffer.h>
#include "./drivers/include/scanCodes.h"

void sys_write(char *text, int lenght, int fd) {
    if (fd == 1) {
        drawChar(0xFF0000, text[0]);
    } else if (fd == 2) {
        drawChar(0xFFFF00, text[0]);
    }
    
}

void sys_read( char *buf, int len, int filedescriptor){
    int pos = getBuffPosition();
    char aux = 0;
    for (int i = 0; i < len; ){
        aux = getBuffCharAt(pos);
        if (aux > 0 && aux <= 255){
            if (aux == 0x39)
                buf[i++]=' ';
            else
                buf[i++]=ScanCodes[(int)aux];
            setBuffPos(pos+1);
        }
        pos = getBuffPosition();
    }
    return;
}