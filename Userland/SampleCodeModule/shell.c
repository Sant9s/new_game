#include <utils.h>
#include <buffer.h>
#include <commands.h>

static char buffer[BUFFER_SIZE] = {0};

void lineRead(char * line){
    __seek_command__(line);
    clearBuffer();
}

void bufferize (){
    
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
            putC(c);
            if ( i == 0 ){
                clearBuffer();
                return;
            }
            buffer[i]=0;
            lineRead(buffer);
            return;
        } else{
            if (!end_of_buffer)
                buffer[i++] = c;
            else
                flag = 1;}
        if (!flag)
            putC(c);
        flag = 0;
    }
    return;
}

int load_shell(void) {
    while (1){
        bufferize();
    }
    return 0;
}