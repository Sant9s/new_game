#include <registers.h>
#include "../drivers/include/videoDriver.h"
#include <interrupts.h>



void printRegisters(registerStructT * registers) {
	drawWordColor(WHITE, "RIP = ");
	drawRegisters(registers->rip);
	drawWordColor(WHITE, "RAX = ");
	drawRegisters(registers->rax);
	drawWordColor(WHITE, "RBX = ");
	drawRegisters(registers->rbx);
	drawWordColor(WHITE, "RCX = ");
	drawRegisters(registers->rcx);
	drawWordColor(WHITE, "RDX = ");
	drawRegisters(registers->rdx);
	drawWordColor(WHITE, "RSP = ");
	drawRegisters(registers->rsp);
	drawWordColor(WHITE, "RBP = ");
	drawRegisters(registers->rbp);
	drawWordColor(WHITE, "RSI = ");
	drawRegisters(registers->rsi);
	drawWordColor(WHITE, "RDI = ");
	drawRegisters(registers->rdi);
	drawWordColor(WHITE, "R8  = ");
	drawRegisters(registers->r8);
	drawWordColor(WHITE, "R9  = ");
	drawRegisters(registers->r9);
	drawWordColor(WHITE, "R10 = ");
	drawRegisters(registers->r10);
	drawWordColor(WHITE, "R11 = ");
	drawRegisters(registers->r11);
	drawWordColor(WHITE, "R12 = ");
	drawRegisters(registers->r12);
	drawWordColor(WHITE, "R13 = ");
	drawRegisters(registers->r13);
	drawWordColor(WHITE, "R14 = ");
	drawRegisters(registers->r14);
	drawWordColor(WHITE, "R15 = ");
	drawRegisters(registers->r15);
}


void printReg(){
	if (!snapshot()) saveState();
	printRegAsm();
}