#ifndef __COMMANDS_H__
#define __COMMANDS_H__

#include "utils.h"
#include "UserSyscalls.h"

#define COMMAND_AMOUNT 10

void checkCommands(char * command); // searches for command
void commandNotFound(char * command);
void help(); // prints list of commands
void time(); // prints current time
void div0(); // divides by zero and throws an exception
void invalidOp(); // generates invalid opcode exception
void clearScreen();
void zoomIn();
void zoomOut();
void exitShell();
void play_snake();
void regState();

#endif /*__COMMANDS_H__*/