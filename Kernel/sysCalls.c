#include <sysCalls.h>
#include <naiveConsole.h>

void sys_write(char *text, int lenght, int fd) {
    if (fd == 1) {
        ncPrint(text);
    } else if (fd == 2) {
        ncPrintColor(text, 0x04, 0x00);
    }
    
}