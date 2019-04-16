//
// Created by RobertVoropaev on 16.04.19.
//

#ifndef FILESYSTEM_RUN_H
#define FILESYSTEM_RUN_H

#include "command_base.h"

void run() {
    if(FILE_SYSTEM_IS_CREATED){
        load_file_system_structure();
    }

    char command[COMMAND_SIZE];
    while(1){
        scanf("%s", command);
        if(!strcmp(command, INIT_COMMAND)){
            if(FILE_SYSTEM_IS_CREATED){
                printf("File system has already been created.\n"
                               "Want to re-create? (All data will be lost) \n[yes/no]: ");
                char answer[5];
                scanf("%s", answer);
                if(!strcmp(answer, "no")){
                    continue;
                }
            }
            init_file_system();
            load_file_system_structure();
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
}


#endif //FILESYSTEM_RUN_H
