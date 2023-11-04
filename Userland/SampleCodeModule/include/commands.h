#ifndef __COMMANDS_H__
#define __COMMANDS_H__

#include "utils.h"
#include "UserSyscalls.h"

#define COMMAND_AMOUNT 10





void checkCommands(char * command); //Busca el comando ingresado por el usuario
void commandNotFound(char * command);
void help(); //Imprime la lista de comandos
void time(); //Imprime la hora actual
void div0(); //Divide por 0 y genera una excepcion
void invalidOp(); //Genera una excepcion de operacion invalida
void clearScreen();
void zoomIn();
void zoomOut();
void exitShell();
void play_snake();

#endif /*__COMMANDS_H__*/