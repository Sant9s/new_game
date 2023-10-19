#include <keyboardBuffer.h>
#include <help.h>
#include <stdint.h>
#include <videoDriver.h>

uint16_t buff[4096];
uint16_t indexBuff = 0;

uint16_t screen[4096];
uint16_t indexScreen = 0;

void help();

void clearBuffer() {
    for (int i = 0; i < 4096; i++) {
        buff[i] = '\0';
    }
    indexBuff = 0;
}

void addBuffer(char key) {
    buff[indexBuff] = key;
    indexBuff++;
}

void addScreen(char key) {
    screen[indexScreen] = key;
    indexScreen++;
}

void deleteBuffer() {
    indexBuff--;
    buff[indexBuff] = '\0';

    indexScreen--;
    screen[indexScreen] = '\0';
}

void commands() {
    char target[] = "help";  // Target string to search for
    int strlen = 4;

    if (indexBuff == strlen) {
        // Check if the buffer contains only and exactly the characters for "help"
        int match = 1;
        for (int i = 0; i < strlen; i++) {
            if ((char)buff[i] != target[i]) {
                match = 0;
                break;
            }
        }

        if (match) {
            help();
        }
    }
}


void zoomIn() {
    clear();
    changeSize(-1);
    for (int i = 0; i < indexScreen; i++) {
        if (screen[i] == ' ') {
            space();
        } else if (screen[i] == '\t') {
            tab();
        } else if (screen[i] == '\n') {
            newline();
        } else {
            drawChar(0xFF0000, screen[i]);
        }
    }
}

void zoomOut() {
    clear();
    changeSize(1);
    for (int i = 0; i < indexScreen; i++) {
        if (screen[i] == ' ') {
            space();
        } else if (screen[i] == '\t') {
            tab();
        } else if (screen[i] == '\n') {
            newline();
        } else {
            drawChar(0xFF0000, screen[i]);
        }
    }
}