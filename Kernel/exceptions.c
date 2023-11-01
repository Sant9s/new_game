

#include "include/exceptions.h"
#include "drivers/include/videoDriver.h"
#include "registers.h"
#include "syscalls.h"

#define ZERO_EXCEPTION_ID 0
//maneja las excepciones y recibe el numero de la excepcion y los registros en el momento de la excepcion
void exceptionDispatcher(int exception, registerStructT * registers) {
	if (exception == ZERO_EXCEPTION_ID){
		zero_division();
	}
	else{
		invalid_opcode();
	}
	printRegisters(registers);
	drawWordColor(RED, "Press any key to continue");
	char c;
	sys_read(&c, 1, 0);
}
//excepcion de division por cero
 void zero_division() {
	drawWordColor(RED, "FATAL ERROR: Can not divide by 0");
	newline();
}
//excepcion de codigo invalido
 void invalid_opcode() {
	drawWordColor(RED, "FATAL ERROR: Invalid opcode");
	newline();
}