#ifndef USERSYSCALLS_H
#define USERSYSCALLS_H

#define STDIN 0
#define STDOUT 1
#define STDERR 2

void call_sys_read(char *buf, int size, int fd);
void call_sys_write(char *buf, int size, int fd);


#endif