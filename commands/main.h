//
// Created by RobertVoropaev on 17.04.19.
//

#ifndef FILESYSTEM_СOMMAND_MAIN_H
#define FILESYSTEM_СOMMAND_MAIN_H

#include "../settings.h"
#include "str_proc.h"
#include "base.h"

int create_directory(const char path[MAX_PATH_LEN]){
    char dir[MAX_PATH_LEN];
    char name[FILE_NAME_SIZE];
    if(get_dir_and_name_in_path(path, dir, name) == -1){
        fprintf(stderr, "Incorrect path\n");
        return -1;
    }

    int inodeDir = find_inode_directory(dir);
    if(create_file_or_dir_in_directory(inodeDir, name, 'd') == -1){
        return -1;
    }
    return 0;
}

int delete_directory(const char path[MAX_PATH_LEN]){
    char dir[MAX_PATH_LEN];
    char name[FILE_NAME_SIZE];
    if(get_dir_and_name_in_path(path, dir, name) == -1){
        fprintf(stderr, "Incorrect path\n");
        return -1;
    }

    int inodeDir = find_inode_directory(dir);
    if(delete_file_or_dir_in_directory(inodeDir, name) == -1){
        return -1;
    }
    return 0;
}

int create_file(const char path[MAX_PATH_LEN]){
    char dir[MAX_PATH_LEN];
    char name[FILE_NAME_SIZE];
    if(get_dir_and_name_in_path(path, dir, name) == -1){
        fprintf(stderr, "Incorrect path\n");
        return -1;
    }

    int inodeDir = find_inode_directory(dir);
    if(create_file_or_dir_in_directory(inodeDir, name, 'f') == -1){
        return -1;
    }
    return 0;
}

int delete_file(const char path[MAX_PATH_LEN]){
    char dir[MAX_PATH_LEN];
    char name[FILE_NAME_SIZE];
    if(get_dir_and_name_in_path(path, dir, name) == -1){
        fprintf(stderr, "Incorrect path\n");
        return -1;
    }

    int inodeDir = find_inode_directory(dir);
    if(delete_file_or_dir_in_directory(inodeDir, name) == -1){
        return -1;
    }
    return 0;
}

int print_directory(const char path[MAX_PATH_LEN]){
    int inodeDir = find_inode_directory(path);
    if(inodeDir == -1){
        fprintf(stderr, "Incorrect path\n");
        return -1;
    }

    int blockDir = get_block(inodeDir);


    struct directory_element directory[MAX_FILE_IN_DIRECTORY];
    int* file_count = malloc(sizeof(int));
    read_directory(directory, file_count, blockDir);

    if(*file_count == 0){
        printf("Directory is empty\n");
        return 0;
    }

    for(int i = 0; i < *file_count; i++){
        printf("%c %s\n",
               inode_table[directory[i].inodeID].type,
               directory[i].name);
    }
    return 0;
}

#endif //FILESYSTEM_СOMMAND_MAIN_H
