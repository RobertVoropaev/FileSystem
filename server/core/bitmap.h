//
// Created by RobertVoropaev on 14.04.19.
//


#ifndef FILESYSTEM_SECTORS_BITMAP_H
#define FILESYSTEM_SECTORS_BITMAP_H


#include "../settings.h"
#include "sectors.h"

/**
 * 1 - занят, 0 - свободен
 */
int bitmap[BITMAP_SIZE];


/**
 * Первичное заполнение bitmap блока BITMAP_BLOCK
 */
void filling_blocks_bitmap(){
    char buf[BLOCK_SIZE];

    for(int i = 0; i < BLOCK_SIZE; i++){
        buf[i] = '|';
    }

    for(int i = 0; i < BITMAP_SIZE; i++){
        buf[i] = '0';
    }

    for(int i = 0; i < INODE_TABLE_START_BLOCK + INODE_TABLE_BLOCK_COUNT; i++) {
        buf[i] = '1';
    }
    buf[ROOT_DIRECTORY_BLOCK] = '1';

    buf[BLOCK_SIZE - 1] = '\0';

    set_sector(buf, BITMAP_BLOCK);
}


/**
 * Чтение bitmap блока
 */
void read_bitmap(){
    char buf[BLOCK_SIZE];
    get_sector(buf, BITMAP_BLOCK);

    for(int i = 0; i < BITMAP_SIZE; i++){
        char c = buf[i];
        if(c == '1'){
            bitmap[i] = 1;
        } else if ( c == '0'){
            bitmap[i] = 0;
        } else {
            perror("Read bitmap error: unknown symbol");
        }
    }
}


/**
 * Запись bitmap в блок
 */
void write_bitmap(){
    char buf[BLOCK_SIZE];

    for(int i = 0; i < BLOCK_SIZE; i++){
        buf[i] = '|';
    }

    for(int i = 0; i < BITMAP_SIZE; i++){
        buf[i] = bitmap[i] + '0';
    }

    buf[BLOCK_SIZE - 1] = '\0';
    set_sector(buf, BITMAP_BLOCK);
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

#endif //FILESYSTEM_SECTORS_BITMAP_H
