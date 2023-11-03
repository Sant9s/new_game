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

void clearScreen(){
    call_clear();
}

int strCompare(char * str1, char * str2){   
    while (*str1 && *str2) {                       
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

void removePreviousSpaces(char *str) {
    if (str == NULL) {
        return;                 // check if NULL
    }

    // Find first index that is not a space
    int indexNoSpaces = 0;
    int flag = 0;
    while (str[indexNoSpaces] == ' ' || str[indexNoSpaces] == '\t') {
        indexNoSpaces++;
        if(!flag){
            flag = 1;
        }
    }
    if(flag){
        // Removes spaces from str
        int i = 0;
        while (str[indexNoSpaces]) {
            str[i] = str[indexNoSpaces];
            i++;
            indexNoSpaces++;
        }
        str[i] = '\0';      // NULL character at the end of string
    }
    
}

void removeEndingSpaces(char *str){
     if (str == NULL) {
        return; // Return if the input string is NULL
    }

    int length = 0;
    int i = 0;

    // Find the length of the string
    while (str[i] != '\0') {
        length++;
        i++;
    }

    i = length - 1;

    // Find the index of the last non-space character
    while (i >= 0 && str[i] == ' ') {
        str[i] = '\0';
        i--;
    }
}

void removeLeadingTrailingSpaces(char* str) {
    removePreviousSpaces(str);
    removeEndingSpaces(str);
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

