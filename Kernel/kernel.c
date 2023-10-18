#include <stdint.h>
#include <string.h>
#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <keyboardDriver.h>
#include <idtLoader.h>
#include <sysCalls.h>
//EN EL MAIN SOLO SE PODRAN INVOCAR FUNCIONES DECLARADAS EN LIB.H Y EN NAIVECONSOLE.H
//kernel.c va a usar las funciones declaradas en naiveConsole.h y en lib.h
//en lib.h van a haber funciones que esten directamente implementadas en libasm.asm (funciones basicas), si es que quiero esto
//en naiveConsole.h van a haber funciones que estan implementadas en naiveConsole.c (funciones complejas)

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

static void * const sampleCodeModuleAddress = (void*)0x400000;
static void * const sampleDataModuleAddress = (void*)0x500000;

typedef int (*EntryPoint)();


void clearBSS(void * bssAddress, uint64_t bssSize)
{
	memset(bssAddress, 0, bssSize);
}

void * getStackBase()
{
	return (void*)(
		(uint64_t)&endOfKernel
		+ PageSize * 8				//The size of the stack itself, 32KiB
		- sizeof(uint64_t)			//Begin at the top of the stack
	);
}

void * initializeKernelBinary()
{
	char buffer[10];

	ncPrint("[x64BareBones]");
	ncNewline();

	ncPrint("CPU Vendor:");
	ncPrint(cpuVendor(buffer));
	ncNewline();

	ncPrint("[Loading modules]");
	ncNewline();
	void * moduleAddresses[] = {
		sampleCodeModuleAddress,
		sampleDataModuleAddress
	};

	loadModules(&endOfKernelBinary, moduleAddresses);
	ncPrint("[Done]");
	ncNewline();
	ncNewline();

	ncPrint("[Initializing kernel's binary]");
	ncNewline();

	clearBSS(&bss, &endOfKernel - &bss);

	ncPrint("  text: 0x");
	ncPrintHex((uint64_t)&text);
	ncNewline();
	ncPrint("  rodata: 0x");
	ncPrintHex((uint64_t)&rodata);
	ncNewline();
	ncPrint("  data: 0x");
	ncPrintHex((uint64_t)&data);
	ncNewline();
	ncPrint("  bss: 0x");
	ncPrintHex((uint64_t)&bss);
	ncNewline();

	ncPrint("[Done]");
	ncNewline();
	ncNewline();
	return getStackBase();
}

int main()
{	
	load_idt();

	ncPrint("[Kernel Main]");
	ncNewline();
	ncPrint("  Sample code module at 0x");
	ncPrintHex((uint64_t)sampleCodeModuleAddress);
	ncNewline();
	ncPrint("  Calling the sample code module returned: ");
	ncPrintHex(((EntryPoint)sampleCodeModuleAddress)());
	ncNewline();

	
	// ncNewline();
	// int x = 30;
	// for (int i = x; i < 720; i++)
	// {
	// 	for (int j = x; j < 720; j++)
	// 	{
	// 		putPixel(0xFF0000, i, j);
	// 	}
		
	// }
	// ncNewline();
	
	

	ncPrint("  Sample data module at 0x");
	ncPrintHex((uint64_t)sampleDataModuleAddress);
	ncNewline();
	ncPrint("  Sample data module contents: ");
	ncPrint((char*)sampleDataModuleAddress);
	ncNewline();
    
    //EJ1
	//agrego la funcion ncPrintColor
    ncNewline();
    ncPrintColor("Arquitectura de Computadoras", 0x02, 0x0f);

	//EJ3
	//agrego la funcion TimeClock
	//kernel.c implementa las funciones de lib.h y naiveConsole.h
	//como en lib.h no declaro ninguna funcion de las "get_time" o "get_date"
	//estas no van a poder ser accedidas
	//en naiveConsole.h solo declaro TimeClock, entonces las funciones auxiliares y las
	//de assembler (implementadas y declaradas respectivamente) no van a poder ser accedidas
	//quedando todo resguardado y solo puediendo utilizar TimeClock
	ncPrint(TimeClock());
	ncNewline();


	// for (int i = 0; i <20; i++) {
	// 	int key = getKey();
	// 	keyboard_handler(key);
	// }
	
	while(1);
	

	ncPrint("[Finished]");
	return 0;
}
