//
// Created by RobertVoropaev on 16.04.19.
//

#ifndef FILESYSTEM_RUN_H
#define FILESYSTEM_RUN_H

#include "../settings.h"
#include "initial.h"
#include "main.h"

/**
 * Функция, циклично обрабатывающая команды пользователя
 */
void run() {
    if(FILE_SYSTEM_IS_CREATED){
        load_file_system_structure();
    }

    char full_command[FULL_COMMAND_SIZE];
    while(1){
        fgets(full_command, FULL_COMMAND_SIZE, stdin);
        if(full_command[0] == '\n'){
            continue;
        }
        for(int i = 0; i < FULL_COMMAND_SIZE; i++){
            if(full_command[i] == '\n'){
                full_command[i] = '\0';
                break;
            }
        }
        if(!strcmp(full_command, INIT)){
            if(FILE_SYSTEM_IS_CREATED){
                printf("File system has already been created.\n"
                               "Want to re-create? (All data will be lost) [yes/no]:\n");
                char answer[4];
                fgets(answer, 4, stdin);
                for(int i = 0; i < 4; i++){
                    if(answer[i] == '\n'){
                        answer[i] = '\0';
                        break;
                    }
                }
                if(strcmp(answer, "yes\0")){
                    continue;
                }
            }
            init_file_system();
            load_file_system_structure();
            printf("File system created!\n");
        }
        else if(!strcmp(full_command, EXIT)){
            printf("Exit\n");
            break;
        }
        else if(!strcmp(full_command, HELP)){
            printf("%s\n", INIT);
            printf("%s\n", EXIT);
            printf("%s <path>\n", MKDIR);
            printf("%s <path>\n", RMDIR);
            printf("%s <path>\n", TOUCH);
            printf("%s <path>\n", RM);
            printf("%s <path>\n", LS);
        }
        else if(!strcmp(full_command, LS)){
            print_directory("\0");
        }
        else {
            char command[COMMAND_SIZE];
            char path[MAX_PATH_LEN];
            command[0] = '\0';
            path[0] = '\0';
            get_command_and_path(full_command, command, path);
            if(!strcmp(command, MKDIR)){
                if(create_directory(path) != 0){
                    printf("Error\n");
                }
                else {
                    printf("Directory created!\n");
                }
            }
            else if(!strcmp(command, RMDIR)){
                if(delete_directory(path) != 0){
                    printf("Error\n");
                }
                else {
                    printf("Directory deleted!\n");
                }
            }
            else if(!strcmp(command, TOUCH)){
                if(create_file(path) != 0){
                    printf("Error\n");
                }
                else {
                    printf("File created!\n");
                };
            }
            else if(!strcmp(command, RM)){
                if(delete_file(path) != 0){
                    printf("Error\n");
                }
                else {
                    printf("File deleted!\n");
                };
            }
            else if(!strcmp(command, LS)){
                if(print_directory(path) != 0){
                    printf("Error\n");
                }
            }
            else{
                fprintf(stderr, "Unknown command\n");
            }
        }
    }
}


#endif //FILESYSTEM_RUN_H
