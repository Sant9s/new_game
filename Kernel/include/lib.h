#ifndef LIB_H
#define LIB_H

#include <stdint.h>
#include "time.h"

void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);
void sleep(uint32_t seconds);
void sleepms(int mseconds);
void nanoms(int nseconds);
char * cpuVendor(char* vendor);
uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base); //Convierte un numero a la base que se le pase

#endif /* LIB_H */