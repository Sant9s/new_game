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

typedef void (*InterruptHandler)(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);

//maneja las interrupciones y recibe el numero de la interrupcion y los registros en el momento de la interrupcion
void irqDispatcher(uint64_t irq, uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
	InterruptHandler interruption[256] = {NULL};
	interruption[0]=&int_20;
	interruption[1]=&int_21;
	interruption[96] = (InterruptHandler)int_80;

    if (irq >= 0 && irq < 256 && interruption[irq] != NULL) {
        InterruptHandler handler = interruption[irq];
        handler(rdi, rsi, rdx, rcx, r8, r9);
		return;
    }
}

void int_20() {
	timer_handler();
}

void int_21() {
	keyboard_handler();
}

//maneja las syscalls y recibe el numero de la syscall y los registros en el momento de la syscall
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

	default:
		return 0;
	}
	return 0;
}