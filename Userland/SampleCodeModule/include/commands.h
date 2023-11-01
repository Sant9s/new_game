#ifndef __COMMANDS_H__
#define __COMMANDS_H__

#include "utils.h"
#include "UserSyscalls.h"

#define COMMAND_LEN 8

enum Commands {
    HELP,
    TIME,
    REG_STATE,
    DIV0,
    INVALOP,
    ZOOMOUT,
    ZOOMIN,
    SNAKE
};



void __seek_command__(char * command); //Busca el comando ingresado por el usuario
void __call_command__(int i, char * command); //Llama al comando ingresado por el usuario
void help(); //Imprime la lista de comandos
void time(); //Imprime la hora actual
void Div0(); //Divide por 0 y genera una excepcion
void invalidOp(); //Genera una excepcion de operacion invalida

#endif /*__COMMANDS_H__*/