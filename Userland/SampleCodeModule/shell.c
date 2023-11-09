#include "include/shell.h"
#include <UserSyscalls.h>
#include <commands.h>
#include <buffer.h>
#include <colors.h>

static char buffer[BUFFER_SIZE] = {0};
static char screen[SCREEN_SIZE] = {0};
int screenIndx = 0;
int status;

void putLine(){   
    own_printf("user > ");
    putCursor();
}

void clearScreenArray(){
    for (int i = 0; i < SCREEN_SIZE; i++){
        screen[i] = 0;
    }
    screenIndx = 0;
}

void read_buffer(){ 
    int i = 0;
    int end_of_buffer = 0;
    int flag = 0;
    while (i < BUFFER_SIZE) {
        char c = getC();
        end_of_buffer = (i == BUFFER_SIZE-1);
        if (c == '\b'){
            if ( i > 0 && screenIndx > 0){
                i--;
                screen[screenIndx--] = 0;
                screen[screenIndx--] = 0;
            }
            else flag = 1;
        } else if (c == '\n'){
            removeCursor();
            putC(c, GREEN);
            buffer[i]=0;
            if(*buffer != 0) checkCommands(buffer);
            if(status) putLine();
            clearBuffer(); 
            return;
        } else if(c == '-'){
            removeCursor();
            putNewLine();
            own_printf("snapshot taken!");
            putNewLine();
            putLine();
            flag = 1;
        }
        else if(c != '-'){
            if (!end_of_buffer)
                buffer[i++] = c;
            else
                flag = 1;}
        if (!flag){
            removeCursor();
            putC(c, GREEN);
            putCursor();
        }

        flag = 0;
    }
    return;
}

void putIntoScreen(char * str){
    while (*str != 0){
        if (screenIndx < SCREEN_SIZE)
        screen[screenIndx++] = *str;
        str++;
    }
}

void showScreen(){
    call_sys_write(screen,0,0);
}

void changeStatus(){
    status = 0;
}



void welcome(){
    char * WELCOME_MESSAGE = "Welcome to the shell\nType help to show all commands\n";
    own_printf("%s", WELCOME_MESSAGE);
}

int start_shell(){
    welcome();
    putLine();
    status = 1;
    while (status){
        read_buffer();
    }
    return 0;
}