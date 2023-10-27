#ifndef KEYBOARDBUFFER_H
#define KEYBOARDBUFFER_H
#define BUFFER_SIZE 4096
#define COMMAND_LENGTH_MAX 8
#define COMMAND_AMOUNT 7

void clearBuffer();
void addBuffer(char key);
void addScreen(char key);
void deleteBuffer();
void commands();
void zoomIn();
void zoomOut();

#endif