#ifndef VIDEO_DRIVER_H
#define VIDEO_DRIVER_H

#include <stdint.h>
#define WHITE 0xFFFFFF
#define RED 0xFF0000
#define GREEN 0x00FF00
#define BLUE 0x0000FF
#define BLACK 0x000000

void drawChar(uint64_t hexColor, char character);
void drawString(uint64_t hexColor, char* word);
void backspace();
void newline();
void tab();
void space();
void changeSize(uint32_t new_size);
void clear();

#endif