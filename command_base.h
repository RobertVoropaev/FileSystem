//
// Created by RobertVoropaev on 15.04.19.
//

#ifndef FILESYSTEM_COMMAND_H
#define FILESYSTEM_COMMAND_H

#include <stdlib.h>
#include "sectors.h"
#include "settings.h"
#include "inode.h"
#include "directory.h"
#include "blocks_bitmap.h"

/**
 * Заполняет главный файл и блоки inode таблицы и bitmap и создает root каталог
 * при первом запуске системы.
 */
void init_file_system(){
    filling_main_file();
    filling_blocks_bitmap();
    filling_inode_table();

    filling_new_directory(ROOT_DIRECTORY_BLOCK);

    read_inode_table();
    inode_table[ROOT_INODE_ID].type = 'd';
    inode_table[ROOT_INODE_ID].len_iblock_arr = 1;
    inode_table[ROOT_INODE_ID].iblock[0] = ROOT_DIRECTORY_BLOCK;
    write_inode_table();

    FILE_SYSTEM_IS_CREATED = 1;
}

/**
 * Загрузка в пармять всех нужных структур.
 * Должна запускать пере каждой работы ФС.
 */
void load_file_system_structure(){
    read_bitmap();
    read_inode_table();
}

/**
 * @return первый свободный блок данных,  -1 - если свободных нет
 */
int get_free_block(){
    for(int i = 0; i < BITMAP_SIZE; i++){
        if(!bitmap[i]){
            return i;
        }
    }
}

/**
 * @return первый свободный inode, -1 - если свободных нет
 */
int get_free_inode(){
    for(int i = 0; i < INODE_TABLE_SIZE; i++){
        if(inode_table[i].type == 'e'){
            return i;
        }
    }
}

/**
 * Ищет inode по имени в папке
 * @param name имя файла с \0
 * @param dir_inode inode папки
 * @return inodeID - если файл в папке есть, -1 - если нет
 */
int find_inode_in_directory(const char name[FILE_NAME_SIZE], int dir_inode){
    if(inode_table[dir_inode].type != 'd'){
        fprintf(stderr, "It's not a directory\n");
        return -1;
    }
    int dir_block = inode_table[dir_inode].iblock[0];

    struct directory_element directory[MAX_FILE_IN_DIRECTORY];
    int* file_count = malloc(sizeof(int));
    read_directory(directory, file_count, dir_block);

    for(int i = 0; i < *file_count; i++){
        if(!strcmp(directory[i].name, name)){
            return directory[i].inodeID;
        }
    }
    return -1;
}


/**
 * Добавляет файл/папку в директорию
 * @param dir_inode inode папки
 * @param name имя файла с \0
 * @param type тип файла/папки
 */
void create_file_in_directory(int dir_inode,
                              const char name[FILE_NAME_SIZE],
                              char type){
    if(inode_table[dir_inode].type != 'd'){
        fprintf(stderr, "It's not a directory\n");
        return;
    }

    char name_copy[FILE_NAME_SIZE];
    strcpy(name_copy, name);
    valid_name(name_copy);

    if(find_inode_in_directory(name_copy, dir_inode) != -1){
        fprintf(stderr, "This file has been created in the directory\n");
        return;
    }

    int dir_block = inode_table[dir_inode].iblock[0];

    struct directory_element directory[MAX_FILE_IN_DIRECTORY];
    int* file_count = malloc(sizeof(int));
    read_directory(directory, file_count, dir_block);

    if(*file_count == MAX_FILE_IN_DIRECTORY){
        fprintf(stderr, "Max file in the directory\n");
        return;
    }

    int new_inode = get_free_inode();
    if(new_inode == -1){
        fprintf(stderr, "No free inode yet\n");
        return;
    }

    int new_block = get_free_block();
    if(new_block == -1){
        fprintf(stderr, "No free block yet\n");
    }

    bitmap[new_block] = 1;

    inode_table[new_inode].type = type;
    inode_table[new_inode].len_iblock_arr = 1;
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
 * Удаляет файл/папку из директории
 * @param dir_inode папки
 * @param name имя файла \0
 */
void delete_file_in_directory(int dir_inode, const char name[FILE_NAME_SIZE]){
    if(inode_table[dir_inode].type != 'd'){
        fprintf(stderr, "It's not a directory\n");
        return;
    }

    char name_copy[FILE_NAME_SIZE];
    strcpy(name_copy, name);
    valid_name(name_copy);

    int file_inode = find_inode_in_directory(name_copy, dir_inode);
    if(file_inode == -1){
        fprintf(stderr, "This file was not found in the directory\n");
        return;
    }
    int file_block = inode_table[file_inode].iblock[0];

    if(inode_table[file_inode].type == 'd'){
        struct directory_element directory[MAX_FILE_IN_DIRECTORY];
        int* file_count = malloc(sizeof(int));
        read_directory(directory, file_count, file_block);
        if(*file_count != 0){
            fprintf(stderr, "This directory is not empty\n");
            return;
        }
    }

    int dir_block = inode_table[dir_inode].iblock[0];

    struct directory_element directory[MAX_FILE_IN_DIRECTORY];
    int* file_count = malloc(sizeof(int));
    read_directory(directory, file_count, dir_block);

    delete_inode_in_directory(directory, *file_count, file_inode);
    inode_table[file_inode].type = 'e';
    inode_table[file_inode].len_iblock_arr = 0;
    bitmap[file_block] = 0;

    write_bitmap();
    write_inode_table();
    write_directory(directory, *file_count - 1, dir_block);
}

int find_inode_directory(const char path[MAX_PATH_LEN]){
    if(path[0] != '\0'){
        return ROOT_INODE_ID;
    }
    if(path[0] != '/'){
        fprintf(stderr, "Incorrect path\n");
        return -1;
    }

    int i_path = 1;
    int i_name = 0;
    char name[FILE_NAME_SIZE];
    int current_inodeID = ROOT_INODE_ID;
    while(1){
        char c = path[i_path++];
        if(c == '/'){
            current_inodeID = find_inode_in_directory(name, current_inodeID);
            if(current_inodeID == -1){
                fprintf(stderr, "This file was not found in the directory\n");
                return -1;
            }
            i_name = 0;
        }
        else if(c == '\0'){
            current_inodeID = find_inode_in_directory(name, current_inodeID);
            if(current_inodeID == -1){
                fprintf(stderr, "This file was not found in the directory\n");
                return -1;
            }
            return current_inodeID;
        }
        else {
            name[i_name++] = c;
        }
    }
}

/**
 * Получение каталоговой и файловой части пути
 * ex. /dir1/dir2/file - > /dir1/dir2 and file
 * @param path полный путь с \0
 * @param dir каталоговая часть
 */
void get_dir_and_name_in_path(const char path[MAX_PATH_LEN],
                              char dir[MAX_PATH_LEN],
                              char name[FILE_NAME_SIZE]){
    if(path[0] != '/'){
        fprintf(stderr, "Incorrect path\n");
        return;
    }
    dir[0] = '\0';

    int i_path = 1;
    int i_dir = 0;
    int i_name = 0;
    int i_cur = 0;
    char cur_name[FILE_NAME_SIZE];
    while(1){
        char c = path[i_path++];
        if(c == '/'){
            cur_name[i_cur++] = c;
            for(int i = 0; i < i_cur; i++){
                dir[i + i_dir] = cur_name[i];
            }
            i_dir += i_cur;
            i_cur = 0;
        }
        else if(c == '\0'){
            for(int i = 0; i < i_cur; i++){
                name[i] = cur_name[i];
                i_name += i_cur;
            }
            name[i_cur] = '\0';
            dir[i_dir - 1] = '\0';
            break;
        }
        else {
            cur_name[i_cur++] = c;
        }
    }
}



#endif //FILESYSTEM_COMMAND_H
