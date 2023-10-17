#ifndef SYSCALLS_H
#define SYSCALLS_H

#include <stdint.h>

void sys_write(char *text, int lenght, int fd);

#endif