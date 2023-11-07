#include "include/buffer.h"

void clearBuffer(char * buff){ // cleans buffer
    for (int i = 0; i<BUFFER_SIZE;i++){
        buff[i]=0;
    }
}