#include <time.h>
#include <stdint.h>
#include "include/defs.h"
#include "naiveConsole.h"
#include "include/interrupts.h"
#include "include/exceptions.h"
#include "registers.h"
#include "drivers/include/videoDriver.h"
#include "keyboard_buffer.h"
#include "include/lib.h"
#include "time.h"
#include "drivers/include/keyboard_driver.h"
#include "syscalls.h"


static void int_20();
static void int_21();
static int int_80(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);


void irqDispatcher(uint64_t irq, uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9){
	switch(irq){
		case 0:
			int_20();
			break;
		case 1:
			int_21();
			break;
		case 96:
			int_80(rdi, rsi, rdx, rcx, r8, r9);
			break;
		default:
			return;
		return;
	}
}

void int_20() {
	timer_handler();
}

void int_21() {
	keyboard_handler();
}

// handles syscalls and recieves the syscall number and registers at that instant
int int_80(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {

    switch (rdi)
	{
	case 1:
		sys_write((char*)rsi, rdx, rcx);
		break;
	
	case 2:
		sys_read((char*)rsi, rdx, rcx);
		break;

	case 3:
		TimeClock((char*)rsi);
		break;

	case 4:
		printRegAsm();
		break;

	case 5:
		changeSize(1);
		break;

	case 6:
		changeSize(-1);
		break;

	case 7:
		sleepms(rsi);
		break;

	case 8:
		clear();
		break;
	case 9:
		drawCursor();
		break;
	case 10:
		deleteCursor();
		break;
	case 11:
		clearScreen();
		break;
	case 12:
		getBufferPosition((int*)rsi);
		break;
	case 13:
		getLastKey((char*)rsi, rdx);
		break;

	default:
		return 0;
	}
	return 0;
}