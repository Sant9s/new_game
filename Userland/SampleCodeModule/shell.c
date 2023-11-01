#include <shell.h>
#include <UserSyscalls.h>
#include <commands.h>
#include <buffer.h>
#include <colors.h>

static char buffer[BUFFER_SIZE] = {0};

void putLine(){   
    putString("user > ", GREEN);
}

void read_buffer(){ 
    int i = 0;
    int end_of_buffer = 0;
    int flag = 0;
    while (i < BUFFER_SIZE) {
        char c = getC();
        end_of_buffer = (i == BUFFER_SIZE-1);
        if (c == '\b'){
            if ( i > 0)
                i--;
            else flag = 1;
        } else if (c == '\n'){
            putC(c, GREEN);
            if ( i == 0 ){
                clearBuffer();
                return;
            }
            buffer[i]=0;
            checkCommands(buffer);
            clearBuffer(); 
            return;
        } else{
            if (!end_of_buffer)
                buffer[i++] = c;
            else
                flag = 1;}
        if (!flag)
            putC(c, GREEN);
        flag = 0;
    }
    return;
}

void welcome(){
    char * WELCOME_MESSAGE = "Welcome to the shell\nType help to show all commands\n";
    putString(WELCOME_MESSAGE, GREEN);
}

int start_shell(){
    welcome();
    while (1){
        putLine();
        read_buffer();
    }
    return 0;
}