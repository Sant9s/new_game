#ifndef LIB_H
#define LIB_H

#include <stdint.h>

//ACA DECLARO LAS FUNCIONES DE ASSEMBLER QUE DIRECTAMENTE QUIERO UTILIZAR EN EL MAIN DEL KERNEL.C
//NO HACERLO PRODUCE QUE LA/S FUNCIONES DE LIBASM.ASM NO PUEDAN SER INVOCADAS EN EL MAIN

void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);


//solo puedo usar cpuVendor pero no puedo utilizar ninguna de las "get_time" o "get_date" en kernel.c
char * cpuVendor(char *result);


#endif