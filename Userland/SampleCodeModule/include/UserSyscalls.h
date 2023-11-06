#ifndef USERSYSCALLS_H
#define USERSYSCALLS_H

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
int call_getBuffPosition(int * pos);
void call_getLastKey(char * key, int pos);
#endif /* USERSYSCALLS_H */