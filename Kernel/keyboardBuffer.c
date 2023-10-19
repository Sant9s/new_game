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
    if (indexBuff >= 4) {
        // Check if enough characters are available in the buffer
        for (int i = 0; i <= indexBuff - strlen; i++) {
            // Check if the target string is present starting at index i
            int match = 1;
            for (int j = 0; j < strlen; j++) {
                if ((char)buff[i + j] != target[j]) {
                    match = 0;
                    break;
                }
            }

            if (match) {
                help();
                break;
            }
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