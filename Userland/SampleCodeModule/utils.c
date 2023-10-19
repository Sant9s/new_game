#include <utils.h>
#include <stdint.h>


void printf(char key) {
    call_sys_write(&key, 1, 1);
}