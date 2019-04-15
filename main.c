#include <stdlib.h>
#include "command.h"

int main(int argc, char *argv[]) {
    char command[COMMAND_SIZE];
    while(1){
        scanf("%s", command);
        if(!strcmp(command, INIT_COMMAND)){
            init_file_system();
            printf("File system is created!\n");
        }
        else if(!strcmp(command, EXIT_COMMAND)){
            printf("exit\n");
            break;
        }
        else{
            printf("unknown command\n");
        }
    }
    return 0;
}
