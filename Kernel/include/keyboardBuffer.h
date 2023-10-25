#ifndef KEYBOARDBUFFER_H
#define KEYBOARDBUFFER_H

#include <stdint.h>

#define BUFF 60
#define SCREEN 4096

uint16_t * getBuff();
uint16_t * getScreen();
int getBuffPosition();
int getScreenPosition();
int setBuffPos(int pos);
int setScreenPos(int pos);
uint16_t getBuffCharAt(int pos);
uint16_t getScreenCharAt(int pos);
void consumeBuffAt(int pos);
void consumeScreenAt(int pos);
void cleanBuff();
void cleanScreen();

#endif