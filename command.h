//
// Created by RobertVoropaev on 15.04.19.
//

#ifndef FILESYSTEM_COMMAND_H
#define FILESYSTEM_COMMAND_H

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
 * Запускается при каждой загруска ФС.
 */
void load_file_system_structure(){
    read_blocks_bitmap();
    read_inode_table();
}


int get_inode_last_directory(char path[MAX_PATH_LEN]){
    if(path[0] != '/'){
        perror("Incorrect path\n");
    }
    int cur_char_in_path = 0;

    struct directory_element directory[MAX_FILE_IN_DIRECTORY];
    int* file_count = malloc(sizeof(int));

    int inodeID = ROOT_INODE_ID;
    while(1) { // поправить
        int cur_block = inode_table[inodeID].iblock[0];
        read_directory(directory, file_count, cur_block);

        char dir_str[FILE_NAME_SIZE];
        for (int i = 0; i < FILE_NAME_SIZE; i++) {
            dir_str[i] = EMPTY_SYMBOL;
        }

        int cur_char_in_dir = 0;
        while (path[cur_char_in_path] != '/') {
            dir_str[cur_char_in_dir++] = path[cur_char_in_path++];
        }
        dir_str[cur_char_in_dir] = '\0';
        valid_name(dir_str);

        int is_found = 0;
        for (int i = 0; i < *file_count; i++) {
            if (!strcmp(directory[i].name, dir_str)) {
                inodeID = directory[i].inodeID;
                is_found = 1;
            }
        }
        if(!is_found){
            return inodeID;
        }
    }
}

int get_free_block(){
    for(int i = 0; i < BITMAP_SIZE; i++){
        if(!block_bitmap[i]){
            return i;
        }
    }
}

int get_free_inode(){
    for(int i = 0; i < INODE_TABLE_SIZE; i++){
        if(inode_table[i].type == 'e'){
            return i;
        }
    }
}

void create(char path[MAX_PATH_LEN], char type){
    int dir_inode_id = get_inode_last_directory(path);
    int dir_block = inode_table[dir_inode_id].iblock[0];

    int new_file_block = get_free_block();
    block_bitmap[new_file_block] = 1;

    int new_file_inode = get_free_inode();
    inode_table[new_file_inode].type = type;
    inode_table[new_file_inode].len_iblock_arr = 1;
    inode_table[new_file_inode].iblock[0] = get_free_block();

    if(type == 'd'){
        filling_new_directory(new_file_block);
    }

    struct directory_element directory[MAX_FILE_IN_DIRECTORY];
    int* file_count = malloc(sizeof(int));
    read_directory(directory, file_count, dir_block);
    if(*file_count == MAX_FILE_IN_DIRECTORY){
        perror("Directory limit");
        return;
    }
    directory[*file_count].inodeID = new_file_inode;

    int len = strlen(path);
    char file_name[FILE_NAME_SIZE];
    int s;
    for(int i = 0; i < len; i++){
        char c = path[i];
        if(c == '/'){
            s = 0;
        }
        else{
            file_name[s++] = c;
        }
    }
    for(int i = s; i < FILE_NAME_SIZE; i++){
        file_name[i] = EMPTY_SYMBOL;
    }
    file_name[FILE_NAME_SIZE - 1] = '\0';

    for(int i = 0; i < FILE_NAME_SIZE; i++){
        directory[*file_count].name[i] = file_name[i];
    }

    write_blocks_bitmap();
    write_inode_table();
    write_directory(directory, *file_count + 1, dir_block);
}


#endif //FILESYSTEM_COMMAND_H
