//
// Created by RobertVoropaev on 16.04.19.
//

#ifndef FILESYSTEM_RUN_H
#define FILESYSTEM_RUN_H

#include "command_base.h"

void get_command_and_path(const char full_command[FULL_COMMAND_SIZE],
                          char command[COMMAND_SIZE],
                          char path[MAX_PATH_LEN]){
    int s = 0;
    int is_command = 1;
    for(int i = 0; i < strlen(full_command); i++){
        char c = full_command[i];
        if(c == ' '){
            s = i + 1;
            command[i] = '\0';
            is_command = 0;
        }
        else {
            if(is_command){
                command[i] = c;
            }
            else {
                path[i - s] = c;
            }
        }
    }
}

void create_directory(const char path[MAX_PATH_LEN]){
    char dir[MAX_PATH_LEN];
    char name[FILE_NAME_SIZE];
    get_dir_and_name_in_path(path, dir, name);

    int inodeDir = find_inode_directory(dir);
    create_file_in_directory(inodeDir, name, 'd');
}

void delete_directory(const char path[MAX_PATH_LEN]){
    char dir[MAX_PATH_LEN];
    char name[FILE_NAME_SIZE];
    get_dir_and_name_in_path(path, dir, name);

    int inodeDir = find_inode_directory(dir);
    delete_file_in_directory(inodeDir, name);
}

void create_file(const char path[MAX_PATH_LEN]){
    char dir[MAX_PATH_LEN];
    char name[FILE_NAME_SIZE];
    get_dir_and_name_in_path(path, dir, name);

    int inodeDir = find_inode_directory(dir);
    create_file_in_directory(inodeDir, name, 'f');
}

void delete_file(const char path[MAX_PATH_LEN]){
    char dir[MAX_PATH_LEN];
    char name[FILE_NAME_SIZE];
    get_dir_and_name_in_path(path, dir, name);

    int inodeDir = find_inode_directory(dir);
    delete_file_in_directory(inodeDir, name);
}

void print_dir(const char path[MAX_PATH_LEN]){
    int inodeDir = find_inode_directory(path);
    int blockDir = get_block(inodeDir);

    struct directory_element directory[MAX_FILE_IN_DIRECTORY];
    int* file_count = malloc(sizeof(int));
    read_directory(directory, file_count, blockDir);

    for(int i = 0; i < *file_count; i++){
        printf("%c %s\n",
               inode_table[directory[i].inodeID].type,
               directory[i].name);
    }
}

void run() {
    if(FILE_SYSTEM_IS_CREATED){
        load_file_system_structure();
    }

    char command[FULL_COMMAND_SIZE];
    while(1){
        scanf("%s", command);
        if(!strcmp(command, INIT)){
            if(FILE_SYSTEM_IS_CREATED){
                printf("File system has already been created.\n"
                               "Want to re-create? (All data will be lost) \n[yes/no]: ");
                char answer[4];
                scanf("%s", answer);
                if(!strcmp(answer, "no")){
                    continue;
                }
            }
            init_file_system();
            load_file_system_structure();
            printf("File system is created!\n");
        }
        else if(!strcmp(command, EXIT)){
            printf("exit\n");
            break;
        }
        else if(!strcmp(command, HELP)){
            printf("%s\n", INIT);
            printf("%s <path>\n", MKDIR);
            printf("%s <path>\n", RMDIR);
            printf("%s <path>\n", TOUCH);
            printf("%s <path>\n", RM);
            printf("%s <path>\n", LS);
        }
        else {
            char com[COMMAND_SIZE];
            char path[MAX_PATH_LEN];
            get_command_and_path(command, com, path);
            if(!strcmp(com, MKDIR)){
                create_directory(path);
            }
            else if(!strcmp(com, RMDIR)){
                delete_directory(path);
            }
            else if(!strcmp(com, TOUCH)){
                create_file(path);
            }
            else if(!strcmp(com, RM)){
                delete_file(path);
            }
            else if(!strcmp(com, LS)){
                print_dir(path);
            }
            else{
                fprintf(stderr, "Unknown command\n");
            }
        }
    }
}


#endif //FILESYSTEM_RUN_H
