#ifndef __COMMANDS_H__
#define __COMMANDS_H__

#include "utils.h"
#include "userSysCalls.h"

#define COMMAND_LEN 8
#define COLOR_LEN 5

enum Commands {
    HELP,
    ZOOMIN,
    ZOOMOUT
};




void __seek_command__(char * command); //Busca el comando ingresado por el usuario
void __call_command__(int i, char * command); //Llama al comando ingresado por el usuario
void help(); //Imprime la lista de comandos
void zoomIn();
void zoomOut();

#endif /*__COMMANDS_H__*/