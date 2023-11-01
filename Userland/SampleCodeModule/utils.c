#include <stdio.h>
#include <stdarg.h>
#include <utils.h>
#include <UserSyscalls.h>
#include <buffer.h>
#include <colors.h>


char getC(){
    char c;
    call_sys_read(&c, 1, 0);
    return c;
}

void putNewLine(){
    putC('\n', 0);
}

void putC(char c, int color){
    call_sys_write(&c, 1, color);
}

void putString(char * str, int color){
    call_sys_write(str, 0, color); 
}

// Similar a strcmp pero solo compara hasta el primer espacio o el fin de la cadena (lo que ocurra primero)
int strCompare(char * str1, char * str2){ 
    while (*str1 && *str2) {                    // removes spaces at the beginning and end of the string
        removeLeadingTrailingSpaces(str2);                  
        char c1 = *str1;
        char c2 = *str2;

        // Convert characters to lowercase for comparison
        if (c1 >= 'A' && c1 <= 'Z') {
            c1 += 'a' - 'A';
        }
        if (c2 >= 'A' && c2 <= 'Z') {
            c2 += 'a' - 'A';
        }

        if (c1 != c2) {
            return c1 - c2;
        }

        str1++;
        str2++;
    }

    // Compare the lengths of the strings
    return (*str1 - *str2);
}

void removeLeadingTrailingSpaces(char* str) {
    if (str == 0) {
        return; // Handle NULL input
    }

    char* start = str;
    char* end = str;

    // Find the end of the string
    while (*end != '\0') {
        end++;
    }
    end--; // Move back from the null terminator

    // Remove trailing spaces
    while (end >= str && (*end == ' ' || *end == '\t')) {
        end--;
    }

    // Null-terminate the new string
    *(end + 1) = '\0';

    // Find the first non-space character from the beginning
    start = str;
    while (*start == ' ' || *start == '\t') {
        start++;
    }

    // Shift characters to remove leading spaces
    while (*start) {
        *str = *start;
        str++;
        start++;
    }

    *str = '\0'; // Null-terminate the resulting string
}

void putInt(int num) {
    if (num < 0) {
        putC('-', GREEN);
        num = -num;
    }

    int divisor = 1;
    while (num / divisor >= 10) {
        divisor *= 10;
    }

    while (divisor > 0) {
        int digit = num / divisor;
        putC('0' + digit, GREEN);
        num %= divisor;
        divisor /= 10;
    }
}
