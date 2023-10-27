


char buff[BUFFER_SIZE];
uint16_t indexBuff = 0;

uint16_t screen[BUFFER_SIZE];
uint16_t indexScreen = 0;

int circularBuffer = 0;             // flag to check if the buffer reached its end

void help();
void div0();
void commandNotFound();
void clearScreen();

void snake(){
    drawString(GREEN, "snake");
}

char *zoomInKey = {0x1D, 0x35};
char *zoomOutKey = {0x1D, 0x18};
static char command_list[COMMAND_AMOUNT][COMMAND_LENGTH_MAX] = {"help", "div0", "zoomIn", "zoomOut", "clear", "snake","" };
void (*functionPointers[])() = {help, div0, zoomIn, zoomOut, clearScreen, snake, clearBuffer};

void commands(){
    newline();
    for(int i = 0; i < COMMAND_AMOUNT; i++){
        if(strcasecmp(buff, command_list[i]) == 0){
            functionPointers[i]();
            return;
        }
    }
    commandNotFound();
    return;

}     

int strcasecmp(const char *str1, const char *str2) {
    while (*str1 && *str2) {  
        removeLeadingTrailingSpaces(str1);                  // removes spaces at the beginning and end of the string
        removeLeadingTrailingSpaces(str2);                  
        char c1 = *str1;
        char c2 = *str2;

        // Convert characters to lowercase for comparison
        if (c1 >= 'A' && c1 <= 'Z') {
            c1 += 'a' - 'A';
        }
        if (c2 >= 'A' && c2 <= 'Z') {
            c2 += 'a' - 'A';
        }

        if (c1 != c2) {
            return c1 - c2;
        }

        str1++;
        str2++;
    }

    // Compare the lengths of the strings
    return (*str1 - *str2);
}

void removeLeadingTrailingSpaces(char* str) {
    if (str == NULL) {
        return; // Handle NULL input
    }

    char* start = str;
    char* end = str;

    // Find the end of the string
    while (*end != '\0') {
        end++;
    }
    end--; // Move back from the null terminator

    // Remove trailing spaces
    while (end >= str && (*end == ' ' || *end == '\t')) {
        end--;
    }

    // Null-terminate the new string
    *(end + 1) = '\0';

    // Find the first non-space character from the beginning
    start = str;
    while (*start == ' ' || *start == '\t') {
        start++;
    }

    // Shift characters to remove leading spaces
    while (*start) {
        *str = *start;
        str++;
        start++;
    }

    *str = '\0'; // Null-terminate the resulting string
}

void zoomOut() {
    call_sys_zoomOut();
}

void zoomIn() {
    call_sys_zoomIn();
}

void commandNotFound(){
    drawString(RED, "ERROR: Command ");
    drawString(RED, buff);
    drawString(RED, " not found");
    newline();
}

void div0(){
    clear();
    drawString(RED, "ERROR: CANNOT DIVIDE BY ZERO");
    newline();
}

void help() {
    call_sys_write("estos son los comandos");
    call_sys_commands();
}

void clearScreen() {
    clear();
}