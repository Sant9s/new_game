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
    putIntoScreen(&c);
}

void putString(char * str, int color){
    call_sys_write(str, 0, color);              // lo escribe en el buffer nomas (creo) TESTEAR
    putIntoScreen(str);
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
    putIntColor(num, GREEN);
}

void putIntColor(int num, int color) {
    if (num < 0) {
        putC('-', color);
        num = -num;
    }

    int divisor = 1;
    while (num / divisor >= 10) {
        divisor *= 10;
    }

    while (divisor > 0) {
        int digit = num / divisor;
        putC('0' + digit, color);
        num %= divisor;
        divisor /= 10;
    }
}

void putCursor(){
    call_cursor();
}

void removeCursor(){
    call_delete_cursor();
}

void own_printf(const char * format, ...){
    va_list args;
    va_start(args, format);


    while(*format != '\0'){
        if(*format == '%'){
            format++;

            switch(*format){
                case 'c': {
                    char c = va_arg(args, char*);
                    putC(c, GREEN);
                    break;
                }
                case 'd': {
                    int d = va_arg(args, int*);
                    putInt(d);
                    break;
                }
                case 's': {
                    char* s = va_arg(args, char*);
                    putString(s, GREEN);
                    break;
                }
            }
        } else {
            putC(*format, GREEN);
        }
        format++;
    }
}

int own_scanf(char * format, ...){
    va_list args;
    va_start(args, format);
    int toRet = 0;

    while(*format != '\0' ){

        if(*format == '%'){
            format++; 
            switch(*format){
                case 'c': {
                    char* c = va_arg(args, char*);
                    *c = getC();
                    toRet++;
                    break;
                }
                case 'd': {
                    int* d = va_arg(args, int*);
                    toRet += readInt(d);
                    break;
                }
                case 's': {
                    char* s = va_arg(args, char*);
                    toRet += readString(s);
                    break;
                }
                case 'x': {
                    int* d = va_arg(args, int*);
                    toRet += readHexInt(d);
                    break;
                }
                default:
                break;
            }
        }
        format++;
    }
    va_end(args);

    return toRet;
}

int readInt(int* d){
    int value = 0;
    int sign = 1;
    char c = getC();

    if(c == '-'){
        sign = -1;
        c = getC();
    }

    while((c != '\0') && (c >= '0' && c <= '9')){
        value = (c - '0') + value*10;
        c = getC();
    }

    *d = value * sign;
    return 1;
}

int readString(char *s){
    int i = 0;
    char c = getC();

    while(c != '\0' && c != '\n'){
        s[i++] = c;
        c = getC();
    }
    s[i] = '\0'; //null terminated
    return i;
}


int readHexInt(int* d){
    int value = 0;
    int sign = 1;
    char c = getC();

    if(c == '-'){
        sign = -1;
        c = getC();
    }

    while(c != '\0' && ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F'))){
        if(c >= 'A' && c <= 'F'){
            c = c - 'A' + 10;
        }else{
            c = c - '0';
        }
        value = value * 16 + c;
        c = getC();
    }

    *d = value * sign;
    return 1;
}

void setFontSize(int size){
    call_set_font_size(size);
}

void putCharColorPos(char character, int color, int x, int y){
    call_put_char_in_set_position(character, color, x, y);
}

void putSquare(int x, int y, int size, int color){
    call_put_square(x, y, size, color);
}