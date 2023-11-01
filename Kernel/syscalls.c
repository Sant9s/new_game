#include "include/syscalls.h"
#include "drivers/include/videoDriver.h"
#include "include/defs.h"
#include "drivers/include/keyboard_driver.h"
#include "keyboard_buffer.h"
#include "include/interrupts.h"



void sys_write(char *buf, int len, int filedescriptor){

    switch (filedescriptor){
        case STDOUT: drawWord(buf);
            return;
        case STDERR: drawWordColor(RED, buf);
            return;
        default: invalidFd();
    }
    
}

void sys_read( char *buf, int len, int filedescriptor){

    switch (filedescriptor){
        case STDIN:;
            int pos = getBufferPosition();
            char aux = 0;
            for (int i = 0; i < len; ){
                _hlt();
                aux = getCharAt(pos);
                if (aux > 0 && aux <= 255){
                    if (aux == 0x39)
                        buf[i++]=' ';
                    else
                        buf[i++]=aux;
                    setPos(pos+1);
                }
                pos = getBufferPosition();
            }
            return;
        default: invalidFd();
    }

}
