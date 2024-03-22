#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdarg.h>


char getC();
void putC(char c, int color);
void putString(char * str, int color);
int strCompare(char * str1, char * str2);
void clearScreen();
void removeLeadingTrailingSpaces(char* str);
void removePreviousSpaces(char* str);
void removeEndingSpaces(char* str);
void putNewLine();
void putInt(int num);
void putIntColor(int num, int color);
void putCursor();
void removeCursor();
void own_printf(const char * format, ...);
int own_scanf(char * format, ...);
int readInt(int* d);
int readString(char *s);
int readHexInt(int* d);
void setFontSize(int size);
void putCharColorPos(char character, int color, int x, int y);
void putSquare(int x, int y, int size, int color);
#endif /* UTILS_H */