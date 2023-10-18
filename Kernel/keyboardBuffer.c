#include <keyboardBuffer.h>
#include <help.h>
#include <stdint.h>
#include <videoDriver.h>

uint16_t buff[4096];
uint16_t index = 0;

void help();

void clearBuffer() {
    for (int i = 0; i < 4096; i++) {
        buff[i] = '\0';
    }
}

void addBuffer(char key) {
    buff[index] = key;
    index++;
}

void deleteBuffer() {
    index--;
    buff[index] = '\0';
}

void checkCommands() {
    char target[] = "help";  // Target string to search for
    int strlen = 4;
    if (index >= 4) {
        // Check if enough characters are available in the buffer
        for (int i = 0; i <= index - strlen; i++) {
            // Check if the target string is present starting at index i
            int match = 1;
            for (int j = 0; j < strlen; j++) {
                if ((char)buff[i + j] != target[j]) {
                    match = 0;
                    break;
                }
            }

            if (match) {
                help();
                break;
            }
        }
    }
}