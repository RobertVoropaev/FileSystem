//
// Created by RobertVoropaev on 15.04.19.
//

#ifndef FILESYSTEM_COMMAND_H
#define FILESYSTEM_COMMAND_H


#include <stdlib.h>
#include "../settings.h"
#include "../core/inode.h"
#include "../core/directory.h"
#include "../core/bitmap.h"
#include "dir_inst.h"


/**
 * Добавляет файл/папку в структуру директории блока
 * @param dir_inode inode папки
 * @param name имя файла с \0
 * @param type тип файла/папки
 */
int create_file_or_dir_in_directory(int dir_inode,
                                     const char *name,
                                     char type){
    if(inode_table[dir_inode].type != 'd'){
        fprintf(stderr, "It's not a directory\n");
        return -1;
    }

    char name_copy[FILE_NAME_SIZE];
    strcpy(name_copy, name);
    valid_name(name_copy);

    if(find_inode_in_directory(name_copy, dir_inode) != -1){
        fprintf(stderr, "This file has been created in the directory\n");
        return -1;
    }

    int dir_block = inode_table[dir_inode].iblock[0];

    struct directory_element directory[MAX_FILE_IN_DIRECTORY];
    int* file_count = malloc(sizeof(int));
    read_directory(directory, file_count, dir_block);

    if(*file_count == MAX_FILE_IN_DIRECTORY){
        fprintf(stderr, "Max file in the directory\n");
        return -1;
    }

    int new_inode = get_free_inode();
    if(new_inode == -1){
        fprintf(stderr, "No free inode yet\n");
        return -1;
    }

    int new_block = get_free_block();
    if(new_block == -1){
        fprintf(stderr, "No free block yet\n");
    }

    bitmap[new_block] = 1;

    inode_table[new_inode].type = type;
    inode_table[new_inode].len_iblock = 1;
    inode_table[new_inode].iblock[0] = new_block;

    if(type == 'd'){
        filling_new_directory(new_block);
    }

    directory[*file_count].inodeID = new_inode;
    strcpy(directory[*file_count].name, name_copy);

    write_bitmap();
    write_inode_table();
    write_directory(directory, *file_count + 1, dir_block);
}


/**
 * Удаляет файл/папку из структуры директории блока
 * @param dir_inode папки
 * @param name имя файла \0
 */
int delete_file_or_dir_in_directory(int dir_inode,
                                     const char *name){
    if(inode_table[dir_inode].type != 'd'){
        fprintf(stderr, "It's not a directory\n");
        return -1;
    }

    char name_copy[FILE_NAME_SIZE];
    strcpy(name_copy, name);
    valid_name(name_copy);

    int file_inode = find_inode_in_directory(name_copy, dir_inode);
    if(file_inode == -1){
        fprintf(stderr, "This file was not found in the directory\n");
        return -1;
    }
    int file_block = inode_table[file_inode].iblock[0];

    if(inode_table[file_inode].type == 'd'){
        struct directory_element directory[MAX_FILE_IN_DIRECTORY];
        int* file_count = malloc(sizeof(int));
        read_directory(directory, file_count, file_block);
        if(*file_count != 0){
            fprintf(stderr, "This directory is not empty\n");
            return -1;
        }
    }

    int dir_block = inode_table[dir_inode].iblock[0];

    struct directory_element directory[MAX_FILE_IN_DIRECTORY];
    int* file_count = malloc(sizeof(int));
    read_directory(directory, file_count, dir_block);

    delete_inode_in_directory_by_structure(directory, *file_count, file_inode);
    inode_table[file_inode].type = 'e';
    inode_table[file_inode].len_iblock = 0;
    bitmap[file_block] = 0;
    clear_sector(file_block);

    write_bitmap();
    write_inode_table();
    write_directory(directory, *file_count - 1, dir_block);
}


#endif //FILESYSTEM_COMMAND_H
