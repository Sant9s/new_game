#ifndef VIDEO_DRIVER_H
#define VIDEO_DRIVER_H

#include <stdint.h>

void drawChar(uint64_t hexColor, char character);
void drawString(uint64_t hexColor, char* word);
void backspace();
void newline();
void tab();
void space();
void changeSize(uint32_t new_size);
void clear();

#endif