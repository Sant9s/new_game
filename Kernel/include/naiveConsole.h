#ifndef NAIVECONSOLE_H
#define NAIVECONSOLE_H

#include <stdint.h>
//ACA DECLARO LAS FUNCIONES QUE QUIERO QUE PUEDAN SER INVOCADAS EN EL MAIN DEL KERNEL.C
//NO HACERLO PRODUCE QUE EL RESTO DE LAS FUNCIONES DE NAIVECONSOLE.C NO PUEDAN SER INVOCADAS


//EJ1
//declaro los prototipos de las funciones print usadas en kernel.c
void ncPrintColor(const char * string, char foreground, char background);
void ncPrintCharColor(char character, char foreground, char background);

//EJ3
//declaro la funcion TimeClock usada en kernel.c
//esta es la unica funcion que va a poder ser usada en el main del kernel.c
//pues es la unica declarada aqui
char * TimeClock();

void ncPrint(const char * string);
void ncPrintChar(char character);
void ncNewline();
void ncPrintDec(uint64_t value);
void ncPrintHex(uint64_t value);
void ncPrintBin(uint64_t value);
void ncPrintBase(uint64_t value, uint32_t base);
void ncClear();
uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base); //Convierte un numero a la base que se le pase

void ncBackspace();

#endif