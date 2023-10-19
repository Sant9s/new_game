#include <sysCalls.h>
#include <naiveConsole.h>
#include <videoDriver.h>

void sys_write(char *text, int lenght, int fd) {
    if (fd == 1) {
        drawChar(0xFF0000, text[0]);
    } else if (fd == 2) {
        drawChar(0xFFFF00, text[0]);
    }
    
}