#include <commands.h>
#include <utils.h>

static char command_list[COMMAND_LEN][10] = {"HELP", "ZOOMIN", "ZOOMOUT"};

//busca el comando en la lista de comandos y llama a la funcion correspondiente
void __seek_command__(char * command){
    

    for (int i = 0; i < COMMAND_LEN; i++){
        if (strcmpspace(command_list[i],command) == 0){
                __call_command__(i, command);
                return;
        }
    }
    __call_command__(-1, command);
}

void __call_command__(int i, char * command){
    switch (i)
    {
    case HELP:;
        help();
        return;
    case ZOOMIN:;
        //zoomIn();
        return;
    case ZOOMOUT:;
        //zoomOut();
        return;
    default:;
        call_sys_write("ERROR - Comando no reconocido",30,2);
        putC('\n');
        return;
    }
}

//imprime la lista de comandos disponibles
void help(){
    call_sys_write("Lista de commandos disponibles:\n", 34,1);
    for (int i = 0; i < COMMAND_LEN; i++){
        call_sys_write(command_list[i],40,1);
        call_sys_write("\n",1,1);
    }
}