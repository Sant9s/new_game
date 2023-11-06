#ifndef USERSYSCALLS_H
#define USERSYSCALLS_H

#define STDIN 0
#define STDOUT 1
#define STDERR 2

void call_sys_read(char *buf, int size, int fd);
void call_sys_write(char *buf, int size, int fd);
void call_timeClock(char *buf);
void call_regState();
void call_zoomOut();
void call_zoomIn();
void call_sleepms(int mseconds);
void call_clear();
void call_cursor();
void call_delete_cursor();
void call_clear_screen();
#endif /* USERSYSCALLS_H */