//
// Created by RobertVoropaev on 14.04.19.
//

#ifndef FILESYSTEM_INODE_H
#define FILESYSTEM_INODE_H

#include "constants.h"
#include "sectors.h"

struct inode {
    char type; // e - free, f - file, d - directory
    int len_iblock_arr; // ex. 0003
    int iblock[IBLOCK_ARRAY_SIZE]; // ex. 0003 0023 0012
};

struct inode inode_table[INODE_TABLE_SIZE];

void filling_inode_table(){
    char inode_str[INODE_SIZE];

    inode_str[0] = 'e';
    inode_str[1] = ';';

    for(int i = 2; i < 6; i++){
        inode_str[i] = '0';
    }
    inode_str[6] = ';';

    for(int i = 7; i < INODE_SIZE - 1; i++){
        inode_str[i] = '0';
    }
    inode_str[INODE_SIZE - 1] = '|';

    char table_buf[BLOCK_SIZE];
    int n = BLOCK_SIZE / INODE_SIZE;
    for(int i = 0; i < n; i++){
        for(int j = 0; j < INODE_SIZE; j++){
            table_buf[i * INODE_SIZE + j] = inode_str[j];
        }
    }
    table_buf[BLOCK_SIZE - 1] = '\0';

    for(int i = INODE_TABLE_START_BLOCK;
        i < INODE_TABLE_BLOCK_COUNT + INODE_TABLE_START_BLOCK; i++){
        set_sector(table_buf, i);
    }
}

struct inode* read_inode_table(){
    char buf[BLOCK_SIZE];
    for(int table_block = 0; table_block < INODE_TABLE_BLOCK_COUNT; table_block++) {
        get_sector(buf, table_block + INODE_TABLE_START_BLOCK);

        for (int i = 0; i < INODE_TABLE_IN_BLOCK; i++) {
            int j = i * INODE_SIZE; //номер в буфере
            int t = INODE_TABLE_IN_BLOCK * table_block + i; //номер в таблице

            inode_table[t].type = buf[j];
            j += 2;

            int num1, num2, num3, num4;
            num1 = buf[j++] - '0';
            num2 = buf[j++] - '0';
            num3 = buf[j++] - '0';
            num4 = buf[j++] - '0';
            inode_table[t].len_iblock_arr = 1000 * num1 + 100 * num2 + 10 * num3 + num4;
            j += 1;

            for (int s = 0; s < IBLOCK_ARRAY_SIZE; s++) {
                num1 = buf[j++] - '0';
                num2 = buf[j++] - '0';
                num3 = buf[j++] - '0';
                num4 = buf[j++] - '0';
                inode_table[t].iblock[s] = 1000 * num1 + 100 * num2 + 10 * num3 + num4;
            }

        }

    }
}


void print_inode_table(){
    for(int i = 0; i < INODE_TABLE_SIZE; i++){
        printf("%c;%d;", inode_table[i].type, inode_table[i].len_iblock_arr);
        for(int j = 0; j < IBLOCK_ARRAY_SIZE; j++){
            printf("%d", inode_table[i].iblock[j]);
        }
        printf("|\n");
    }
}



#endif //FILESYSTEM_INODE_H
