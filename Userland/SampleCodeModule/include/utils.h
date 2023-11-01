#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdarg.h>


char getC();
void putC(char c);
void putString(char * str);
int strcmp(char * str1, char * str2);
int strcmpspace(char * str1, char * str2);
int null_or_space(char c);
#endif /* UTILS_H */