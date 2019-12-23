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
    char response[MAX_RESPONSE_LEN];

    if(get_fs_creation_status() == 1){
        load_file_system_structure();

        strcpy(response, "File system is ready!\n"
                "Enter command:\n");
        printf(response);

    }
    else{
        init_file_system();
        load_file_system_structure();

        strcpy(response, "File system created!\n");
        printf(response);
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
            if(get_fs_creation_status() == 1){
                strcpy(response, "File system has already been created.\n"
                        "Want to re-create? (All data will be lost) [yes/no]:\n");
                printf(response);

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
            fprintf(stdin, "Processing...\n");

            init_file_system();
            load_file_system_structure();

            strcpy(response, "File system created!\n");
            printf(response);
        }
        else if(!strcmp(full_command, EXIT)){
            break;
        }
        else if(!strcmp(full_command, HELP)){
            sprintf(response, "%s\n"
                    "%s\n"
                    "%s <path>\n"
                    "%s <path>\n"
                    "%s <path>\n"
                    "%s <path>\n"
                    "%s <path>\n",
                    INIT, EXIT, MKDIR, RMDIR, TOUCH, RM, LS);
            printf(response);
        }
        else if(!strcmp(full_command, LS)){
            strcpy(response, get_ls_directory("\0"));
            printf(response);
        }
        else {
            char command[COMMAND_SIZE];
            char path[MAX_PATH_LEN];

            command[0] = '\0';
            path[0] = '\0';

            get_command_and_path(full_command, command, path);
            if(!strcmp(command, MKDIR)){
                fprintf(stdin, "Processing...\n");
                if(create_directory(path) != 0){
                    strcpy(response, "Error\n");
                    printf(response);
                }
                else {
                    strcpy(response, "Directory created!\n");
                    printf(response);
                }
            }
            else if(!strcmp(command, RMDIR)){
                fprintf(stdin, "Processing...\n");
                if(delete_directory(path) != 0){
                    strcpy(response, "Error\n");
                    printf(response);
                }
                else {
                    strcpy(response, "Directory deleted!\n");
                    printf(response);
                }
            }
            else if(!strcmp(command, TOUCH)){
                fprintf(stdin, "Processing...\n");
                if(create_file(path) != 0){
                    strcpy(response, "Error\n");
                    printf(response);
                }
                else {
                    strcpy(response, "File created!\n");
                    printf(response);
                };
            }
            else if(!strcmp(command, RM)){
                fprintf(stdin, "Processing...\n");
                if(delete_file(path) != 0){
                    strcpy(response, "Error\n");
                    printf(response);
                }
                else {
                    strcpy(response, "File deleted!\n");
                    printf(response);
                };
            }
            else if(!strcmp(command, LS)){
                if(print_directory(path) != 0){
                    strcpy(response, "Error\n");
                    printf(response);
                }
            }
            else if(!strcmp(command, CAT)){
                if(read_file(path) != 0){
                    strcpy(response, "Error\n");
                    printf(response);
                }
            }
            else if(!strcmp(command, ECHO)){
                strcpy(response, "Enter text:\n");
                printf(response);

                char buf[BLOCK_SIZE];
                fgets(buf, BLOCK_SIZE, stdin);
                for(int i = 0; i < FULL_COMMAND_SIZE; i++){
                    if(full_command[i] == '\n'){
                        full_command[i] = '\0';
                        break;
                    }
                }
                if(write_file(path, buf) != 0){
                    strcpy(response, "Error\n");
                    printf(response);
                } else {
                    strcpy(response, "Text saved\n");
                    printf(response);
                }
            }
            else{
                strcpy(response, "Unknown command\n");
                printf(response);
            }
        }
    }
}


#endif //FILESYSTEM_RUN_H
