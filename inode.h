//
// Created by RobertVoropaev on 09.04.19.
//

#ifndef FILESYSTEM_INODE_H
#define FILESYSTEM_INODE_H

#include <stdio.h>
#include <stdlib.h>
#include "sectors.h"

const int MAX_NUM_SIZE = 2;
const int MAX_NAME = 12;
const int MAX_ARRAY = 24;
const int INODE_SIZE = 64;
const int INODE_COUNT = SECTOR_SIZE / INODE_SIZE;

struct inode{
    int num;
    char name[MAX_NAME]; // с завершающим нулем
    char type;
    int array[MAX_ARRAY]; // у директорий это номера inode, 99 - пусто
};


/**
 * Первичное заполнение таблицы inode
 * 0...1 - num
 * 2...13 - name
 * 14 - type
 * 15...62 - array
 * 63 - |
 */
void filling_inode_table(){
    char inode[INODE_SIZE];

    inode[0] = inode[1] = '0';

    for(int i = 2; i < 14; i++){
        inode[i] = '-';
    }

    inode[14] = 'e';

    for(int i = 15; i < 63; i++){
        inode[i] = '9';
    }

    inode[63] = '|';

    char table_buf[SECTOR_SIZE];
    int n = SECTOR_SIZE / INODE_SIZE;
    for(int i = 0; i < n; i++){
        inode[0] = '0' + i / 10;
        inode[1] = '0' + i % 10;

        for(int j = 0; j < INODE_SIZE; j++){
            table_buf[i * INODE_SIZE + j] = inode[j];
        }
    }
    table_buf[SECTOR_SIZE - 1] = '\0';

    set_sector(table_buf, 0);
}

struct inode table[INODE_COUNT];

void read_inode_table(){
    char buf[SECTOR_SIZE];
    get_sector(buf, 0);

    for(int i = 0; i < INODE_COUNT; i++){
        int k = i * INODE_SIZE;

        table[i].num = 10*(buf[k] - '0') + (buf[k + 1] - '0');
        k += 2;

        for(int j = 0; j < MAX_NAME; j++){
            table[i].name[j] = buf[k + j];
        }
        k += MAX_NAME;
        table[i].name[MAX_NAME - 1] = '\0';

        table[i].type = buf[k];
        k += 1;

        for(int j = 0;  j < MAX_ARRAY; j++){
            int num1 = buf[k + 2*j] - '0';
            int num2 = buf[k + 2*j + 1] - '0';
            table[i].array[j] = 10*num1 + num2;
        }
    }
}

void write_inode_table(){
    char buf[SECTOR_SIZE];

    for(int i = 0; i < INODE_COUNT; i++){
        int k = i * INODE_SIZE;

        buf[k] = table[i].num/10 + '0';
        buf[k + 1] = table[i].num%10 + '0';
        k += 2;

        for(int j = 0; j < MAX_NAME; j++){
            buf[k + j] = table[i].name[j];
        }
        k += MAX_NAME;
        buf[k - 1] = '|'; // чтобы не было \0 в MAIN_FILE

        buf[k] = table[i].type;
        k += 1;

        for(int j = 0;  j < MAX_ARRAY; j++){
            int num1 = table[i].array[j] / 10;
            int num2 = table[i].array[j] % 10;
            buf[k + 2 * j] = num1 + '0';
            buf[k + 2 * j + 1] = num2 + '0';
        }
        k += MAX_ARRAY * 2;
        buf[k] = '|';
        if(i == INODE_COUNT - 1){
            buf[k] = '\0';
        }
    }

    set_sector(buf, 0);
}

void print_inode_table(){
    for(int i = 0; i < INODE_COUNT; i++){
        struct inode in = table[i];
        printf("%d |", in.num);
        printf("%s |", in.name);
        printf("%c |", in.type);
        for(int j = 0; j < MAX_ARRAY; j++){
            printf("%d ", in.array[j]);
        }
        printf("\n");
    }
}

#endif FILESYSTEM_INODE_H
