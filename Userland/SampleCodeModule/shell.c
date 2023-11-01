#include <shell.h>
#include <UserSyscalls.h>
#include <commands.h>
#include <buffer.h>
#include <colors.h>

static char buffer[BUFFER_SIZE] = {0};

//Lee linea y la manda a __seek_command__
void lineRead(char * line){
    __seek_command__(line);
    //---
    clearBuffer();
}

void putLineStart()
{   
    putString("user > ", GREEN);
}

/*
La función bufferize() lee caracteres de entrada y los almacena en un búfer hasta que se cumplan ciertas condiciones: 
retroceso (\b), salto de línea (\n) o el búfer alcanza su capacidad máxima. 
Si se detecta un retroceso, se retrocede una posición en el búfer si es posible. 
Si se detecta un salto de línea, muestra el caracter en la salida, 
almacena el búfer en una línea y retorna. 
En caso contrario, si el búfer no ha alcanzado su capacidad máxima, 
se almacena el caracter en el búfer y se muestra en la salida. 
La función finaliza cuando se completa el búfer o no hay más caracteres de entrada.
*/
void bufferize (){ 
    int i = 0;
    int end_of_buffer = 0;
    int flag = 0;
    while (i < BUFFER_SIZE) {
        char c = getC();
        end_of_buffer = (i == BUFFER_SIZE-1);
        if (c == '\b'){
            if ( i > 0)
                i--;
            else flag = 1;
        } else if (c == '\n'){
            putC(c, GREEN);
            if ( i == 0 ){
                clearBuffer();
                return;
            }
            buffer[i]=0;
            lineRead(buffer);
            return;
        } else if (c == ';' && i==0) {
            putString("Registers snapshot taken\n", WHITE);
            clearBuffer();
            return;
        } else{
            if (!end_of_buffer)
                buffer[i++] = c;
            else
                flag = 1;}
        if (!flag && c!=';')
            putC(c, GREEN);
        flag = 0;
    }
    return;
}

void welcome(){
    char * WELCOME_MESSAGE = "Welcome to the shell\nType help to show all commands\n";
    putString(WELCOME_MESSAGE, GREEN);
}

int __shell_init__(){
    welcome();
    while (1){
        putLineStart();
        bufferize();
    }
    return 0;
}