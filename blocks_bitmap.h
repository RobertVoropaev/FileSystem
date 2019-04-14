//
// Created by RobertVoropaev on 14.04.19.
//

#ifndef FILESYSTEM_SECTORS_BITMAP_H
#define FILESYSTEM_SECTORS_BITMAP_H

#include "constants.h"
#include "inode.h"
#include "sectors.h"

int block_bitmap[BLOCK_COUNT];

void filling_blocks_bitmap(){
    char buf[BLOCK_SIZE];

    for(int i = 0; i < BLOCK_SIZE; i++){
        buf[i] = '|';
    }

    for(int i = 0; i < BLOCK_COUNT; i++){
        buf[i] = '0';
    }

    for(int i = 0; i < INODE_TABLE_START_BLOCK + INODE_TABLE_BLOCK_COUNT; i++) {
        buf[i] = '1';
    }

    buf[BLOCK_SIZE - 1] = '\0';

    set_sector(buf, BITMAP_BLOCK);
}

void read_blocks_bitmap(){
    char buf[BLOCK_SIZE];
    get_sector(buf, BITMAP_BLOCK);

    for(int i = 0; i < BLOCK_COUNT; i++){
        char c = buf[i];
        if(c == '1'){
            block_bitmap[i] = 1;
        } else if ( c == '0'){
            block_bitmap[i] = 0;
        } else {
            perror("Read bitmap error: unknown symbol");
        }
    }
}

void write_blocks_bitmap(){
    char buf[BLOCK_SIZE];

    for(int i = 0; i < BLOCK_SIZE; i++){
        buf[i] = '|';
    }

    for(int i = 0; i < BLOCK_COUNT; i++){
        buf[i] = block_bitmap[i] + '0';
    }

    buf[BLOCK_SIZE - 1] = '\0';
    set_sector(buf, BITMAP_BLOCK);
}

#endif //FILESYSTEM_SECTORS_BITMAP_H
