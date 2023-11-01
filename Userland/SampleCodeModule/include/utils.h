#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdarg.h>


char getC();
void putC(char c, int color);
void putString(char * str, int color);
int strCompare(char * str1, char * str2);
void removeLeadingTrailingSpaces(char* str);
int null_or_space(char c);
int scanF(char * format, ...);
void printF(const char * format, ...);
void putNewLine();
#endif /* UTILS_H */