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
void putCursor();
void removeCursor();
#endif /* UTILS_H */