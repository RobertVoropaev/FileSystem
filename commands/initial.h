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


#endif //FILESYSTEM_INITIAL_H
