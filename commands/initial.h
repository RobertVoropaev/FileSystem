//
// Created by RobertVoropaev on 17.04.19.
//

#ifndef FILESYSTEM_INITIAL_H
#define FILESYSTEM_INITIAL_H

#include "../core/sectors.h"
#include "../core/bitmap.h"
#include "../core/inode.h"
#include "../core/directory.h"


/**
 * Устанавливает блок статуса системы на уже создана
 */
void set_fs_creation_status(){
    char buf[BLOCK_SIZE];;
    buf[0] = '1';
    for(int i = 1; i < BLOCK_SIZE - 1; i++){
        buf[i] = EMPTY_SYMBOL;
    }
    buf[BLOCK_SIZE - 1] = '\0';
    set_sector(buf, STATUS_BLOCK);
}


/**
 * Загружает блок статуса системы
 * @return 1 - создана, 0 - нет
 */
int get_fs_creation_status(){
    char buf[BLOCK_SIZE];
    get_sector(buf, STATUS_BLOCK);
    return buf[0] - '0';
}


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
    inode_table[ROOT_INODE_ID].len_iblock = 1;
    inode_table[ROOT_INODE_ID].iblock[0] = ROOT_DIRECTORY_BLOCK;
    write_inode_table();

    read_bitmap();
    bitmap[STATUS_BLOCK] = 1;
    write_bitmap();

    set_fs_creation_status();
}


/**
 * Загрузка в пармять всех нужных структур.
 * Должна запускать пере каждой работы ФС.
 */
void load_file_system_structure(){
    read_bitmap();
    read_inode_table();
}


#endif //FILESYSTEM_INITIAL_H
