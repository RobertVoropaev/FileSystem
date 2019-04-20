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

int write_file(const char path[MAX_PATH_LEN],
               char str[BLOCK_SIZE]){
    char dir[MAX_PATH_LEN];
    char name[FILE_NAME_SIZE];
    if(get_dir_and_name_in_path(path, dir, name) == -1){
        fprintf(stderr, "Incorrect path\n");
        return -1;
    }

    int dir_inode = find_inode_directory(dir);
    int file_inode = find_inode_in_directory(name, dir_inode);
    if(inode_table[file_inode].type != 'f'){
        printf("It's not a file\n");
        return -1;
    }
    int file_block = get_block(file_inode);

    int s = 0;
    for(int i = 0; i < BLOCK_SIZE; i++){
        if(str[i] == '\0'){
            s = i;
            break;
        }
    }
    for(int i = s; i < BLOCK_SIZE; i++){
        str[i] = EMPTY_SYMBOL;
    }
    str[BLOCK_SIZE - 1] = '\0';


    set_sector(str, file_block);
    return 0;
}

int read_file(const char path[MAX_PATH_LEN]){
    char dir[MAX_PATH_LEN];
    char name[FILE_NAME_SIZE];
    if(get_dir_and_name_in_path(path, dir, name) == -1){
        fprintf(stderr, "Incorrect path\n");
        return -1;
    }

    int dir_inode = find_inode_directory(dir);
    int file_inode = find_inode_in_directory(name, dir_inode);
    if(inode_table[file_inode].type != 'f'){
        printf("It's not a file\n");
        return -1;
    }
    int file_block = get_block(file_inode);

    char buf[BLOCK_SIZE];
    get_sector(buf, file_block);
    for(int i = 0; i < BLOCK_SIZE; i++){
        if(buf[i] == EMPTY_SYMBOL){
            buf[i] = '\0';
        }
    }
    printf("%s", buf);
    return 0;
}

#endif //FILESYSTEM_СOMMAND_MAIN_H
