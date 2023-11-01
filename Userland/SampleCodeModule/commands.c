#include "commands.h"
#include "UserSyscalls.h"
#include "stdint.h"
#include "utils.h"
#include "funcAsm.h"
#include "shell.h"
static char command_list[COMMAND_LEN][10] = {"HELP", "TIME", "REGSTATE", "DIV0", "INVALOP", "ZOOMOUT", "ZOOMIN"};

//busca el comando en la lista de comandos y llama a la funcion correspondiente
void __seek_command__(char * command){
    

    for (int i = 0; i < COMMAND_LEN; i++){
        if (strcmpspace(command_list[i],command) == 0){
                __call_command__(i, command);
                return;
        }
    }
    __call_command__(-1, command);
}

void __call_command__(int i, char * command){
    switch (i)
    {
    case HELP:;
        help();
        return;
    case TIME:;
        time();
        return;
    case REG_STATE:;
        call_regState();
        return;
    case DIV0:;
        Div0();
        return;
    case INVALOP:;
        invalidOp();
        return;
    case ZOOMOUT:;
        call_zoomOut();
        return;
    case ZOOMIN:;
        call_zoomIn();
        return;
    default:;
        call_sys_write("ERROR - Command not recognized",30,2);
        putC('\n');
        return;
    }
}

//imprime la lista de comandos disponibles
void help(){
    call_sys_write("Commands list:\n", 34,1);
    for (int i = 0; i < COMMAND_LEN; i++){
        call_sys_write(command_list[i],40,1);
        call_sys_write("\n",1,1);
    }
}

void time(){
    char timeClock[50];
    call_timeClock(timeClock);
    char c;
    for (int i = 0; (c = timeClock[i]) != 0; i++){
        putC(c);
    }
    putC('\n');
}

void Div0(){
    int a = 0;
    int b = 1;
     a = b/a;
}

void invalidOp(){
    invalidOpAsm();
}