#ifndef _VIDEO_DRIVER_H
#define _VIDEO_DRIVER_H

#include <stdint.h>

#define RED 0xFF0000
#define GREEN 0x00FF00
#define BLUE 0x0000FF
#define WHITE 0xFFFFFF
#define BLACK 0x000000

#define COLORS (int[]) {GREEN, RED, WHITE, BLUE, BLACK}


#define isMinusc(x) ((x)<='a'?(((x)>='z')?1:0):0)
#define DEFAULT_FONT_SIZE 1

void changeSize(double new_size);
void newline();
void drawWordColor(uint64_t hexColor, char* word);
void drawWord(char* word);
void invalidFd(); // indicates that the filedescripor is invalid
void drawRegisters(int value);
void clear();
void drawCursor();
void deleteCursor();
void clearScreen();
void setFontSize(int value);
void setSize(int new_size);
void putCharInSetSpace(char c, int color, int x, int y);
void drawSquareInSetPosition(int x, int y, int size, int color);


#endif /* _VIDEO_DRIVER_H */