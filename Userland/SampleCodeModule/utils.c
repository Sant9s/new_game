#include <stdio.h>
#include <stdarg.h>
#include <utils.h>
#include <UserSyscalls.h>
#include <buffer.h>
#include <colors.h>


// int null_or_space(char c){
//     return (c == '\0' || c == ' ');
// }

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

// void putInt(int num) {
//     if (num < 0) {
//         putC('-', GREEN);
//         num = -num;
//     }

//     int divisor = 1;
//     while (num / divisor >= 10) {
//         divisor *= 10;
//     }

//     while (divisor > 0) {
//         int digit = num / divisor;
//         putC('0' + digit, GREEN);
//         num %= divisor;
//         divisor /= 10;
//     }
// }

// int readInt(int* d){
//     int value = 0;
//     int sign = 1; //en principio positivo
//     char c = getC();

//     if(c == '-'){
//         sign = -1;
//         c = getC();
//     }

//     while((c != '\0') && (c >= '0' && c <= '9')){
//         value = (c - '0') + value*10;
//         c = getC();
//     }

//     *d = value * sign;
//     return 1;
// }

// int readString(char *s){
//     int i = 0;
//     char c = getC();

//     while(c != '\0' && c != '\n'){
//         s[i++] = c;
//         c = getC();
//     }
//     s[i] = '\0'; //null terminated
//     return i;
// }


// int readHexInt(int* d){
//     int value = 0;
//     int sign = 1;
//     char c = getC();

//     if(c == '-'){
//         sign = -1;
//         c = getC();
//     }

//     while(c != '\0' && ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F'))){
//         if(c >= 'A' && c <= 'F'){
//             c = c - 'A' + 10;
//         }else{
//             c = c - '0';
//         }
//         value = value * 16 + c;
//         c = getC();
//     }

//     *d = value * sign;
//     return 1;
// }

// int scanF(char * format, ...){
//     va_list args;
//     va_start(args, format);
//     int toRet = 0;

//     while(*format != '\0' ){

//         if(*format == '%'){
//             format++; 
//             switch(*format){
//                 case 'c': {
//                     char* c = va_arg(args, char*);
//                     *c = getC();
//                     toRet++;
//                     break;
//                 }
//                 case 'd': {
//                     int* d = va_arg(args, int*);
//                     toRet += readInt(d);
//                     break;
//                 }
//                 case 's': {
//                     char* s = va_arg(args, char*);
//                     toRet += readString(s);
//                     break;
//                 }
//                 case 'x': {
//                     int* d = va_arg(args, int*);
//                     toRet += readHexInt(d);
//                     break;
//                 }
//                 default:
//                 break;
//             }
//         }
//         format++;
//     }
//     va_end(args);

//     return toRet;
// }