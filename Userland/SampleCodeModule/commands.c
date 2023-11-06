#include "commands.h"
#include "UserSyscalls.h"
#include "utils.h"
#include "funcAsm.h"
#include "include/shell.h"
#include <snake.h>
#include <colors.h>

static char command_list[COMMAND_AMOUNT][10] = {"HELP", "TIME", "REGSTATE", "DIV0", "INVALOP", "ZOOMOUT", "ZOOMIN", "SNAKE", "CLEAR","EXIT"};
void (*functionPointers[COMMAND_AMOUNT])() = {help, time, call_regState, div0, invalidOp, zoomOut, zoomIn, play_snake, clearScreen, exitShell};

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

void commandNotFound(char * command){
    putString("ERROR: - '", GREEN);
    putString(command, GREEN);
    putString("' is not a valid command\n", GREEN);
    putString("Type help to show all commands\n", GREEN);
}


//imprime la lista de comandos disponibles
void help(){
    putString("Command list:\n", GREEN); 
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

void clearScreen(){
    call_clear_screen();
    clearScreenArray();
}

void zoomIn(){
    call_clear_screen();
    call_zoomOut();
    showScreen();
}

void zoomOut(){
    call_clear_screen();
    call_zoomIn();
    showScreen();
}

void exitShell(){
    putString("Exiting Shell", RED);
    call_sleepms(100);
    putC('.',RED);
    call_sleepms(130);
    putC('.',RED);
    call_sleepms(180);
    putC('.',RED);
    changeStatus();
}

void play_snake(){
    char c;
    int flag = 1;
    while (flag){
        putNewLine();
        putString("Type 1 for 1 player or type 2 for 2 players: ", GREEN);
        c = getC();
        if (c == '1'){
            flag = 0;
            snake();
        }
        if (c == '2'){
            flag = 0;
            snake_multiplayer();
        }
        if (flag) putString(" Error", RED);
    }
 }
    
