#include <keyboardBuffer.h>
#include <help.h>
#include <stdint.h>
#include <videoDriver.h>

uint16_t buff[BUFF];
uint16_t buffLenght = 0;
uint16_t buffPos = 0;

uint16_t screen[SCREEN];
uint16_t screenLenght = 0;
uint16_t screenPos = 0;

uint16_t * getBuff() {
    return buff;
}

uint16_t * getScreen() {
    return screen;
}

int getBuffPosition() {
    return buffPos;
}

int getScreenPosition() {
    return screenPos;
}

int setBuffPos(int pos) {
    buffPos = pos;
    return pos;
}

int setScreenPos(int pos) {
    screenPos = pos;
    return pos;
}

uint16_t getBuffCharAt(int pos) {
    return buff[pos];
}

uint16_t getScreenCharAt(int pos) {
    return screen[pos];
}

void consumeBuffAt(int pos) {
    buff[pos] = 0;
}

void consumeScreenAt(int pos) {
    screen[pos] = 0;
}

void cleanBuff() {
    for (int i = 0; i < BUFF; i++)
    {
        buff[i] = '\0';
    }
}

void cleanScreen() {
    for (int i = 0; i < SCREEN; i++)
    {
        screen[i] = '\0';
    }
}