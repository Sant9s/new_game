#include "commands.h"
#include "UserSyscalls.h"
#include "stdint.h"
#include "utils.h"
#include "funcAsm.h"
#include "shell.h"
#include <snake.h>
#include <colors.h>

static char command_list[COMMAND_AMOUNT][10] = {"HELP", "TIME", "REGSTATE", "DIV0", "INVALOP", "ZOOMOUT", "ZOOMIN", "SNAKE","INVALIDOP", "CLEAR"};
void (*functionPointers[COMMAND_AMOUNT])() = {help, time, call_regState, div0, invalidOp, call_zoomIn, call_zoomOut, snake, invalidOp, clearScreen};

//busca el comando en la lista de comandos y llama a la funcion correspondiente
void checkCommands(char * command){
    removeLeadingTrailingSpaces(command); 
    for (int i = 0; i < COMMAND_AMOUNT; i++){
        if (strCompare(command_list[i], command) == 0){ 
                functionPointers[i]();
                return;
        }
    }
    commandNotFound(command);
}

void test(){
    clearScreen();
}

void commandNotFound(char * command){
    putString("ERROR: - '", RED);
    putString(command, RED);
    putString("' is not a valid command\n", RED);
    putString("Type help to show all commands\n", GREEN);
}


//imprime la lista de comandos disponibles
void help(){
    putString("Commands list:\n", GREEN); 
    for (int i = 0; i < COMMAND_AMOUNT; i++){
        putString(" - ", GREEN);
        putString(command_list[i], GREEN);
        putNewLine();
    }
}

void time(){
    char timeClock[50];
    call_timeClock(timeClock);
    putString(timeClock, GREEN);
    putNewLine();
}

void div0(){
    int a = 0;
    int b = 1;
     a = b/a;
}

void invalidOp(){
    invalidOpAsm();
}