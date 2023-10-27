#include <keyboardBuffer.h>
#include <help.h>
#include <stdint.h>
#include <videoDriver.h>
#include <string.h>


char buff[BUFFER_SIZE];
uint16_t indexBuff = 0;

uint16_t screen[BUFFER_SIZE];
uint16_t indexScreen = 0;

int circularBuffer = 0;             // flag to check if the buffer reached its end

void help();
void div0();
void commandNotFound();
void clearScreen();

void clearBuffer() {
    for (int i = 0; i < BUFFER_SIZE; i++) {
        buff[i] = '\0';
    }
    indexBuff = 0;
}

void addBuffer(char key) {
    if(indexBuff == BUFFER_SIZE){
       indexBuff = 0;                      // circular buffer
    }
    buff[indexBuff++] = key;
}

void addScreen(char key) {
    screen[indexScreen++] = key;
}

void deleteBuffer() {
    if(indexBuff >= 1){
        buff[--indexBuff] = '\0';                                       // que pasa si indexBuff es 0??
        screen[--indexScreen] = '\0';
    }
}