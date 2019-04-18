//
// Created by RobertVoropaev on 14.04.19.
//

#ifndef FILESYSTEM_INODE_H
#define FILESYSTEM_INODE_H


#include "../settings.h"
#include "sectors.h"


/**
 * type: e - empty, f - file, d - directory
 */
struct inode {
    char type;
    int len_iblock;
    int iblock[IBLOCK_ARRAY_SIZE];
};


struct inode inode_table[INODE_TABLE_SIZE];


/**
 *  Первичное заполнение блоков inode таблицы
 */
void filling_inode_table(){
    char inode_str[INODE_SIZE];

    inode_str[0] = 'e';
    inode_str[1] = ';';

    for(int i = 2; i < 6; i++){
        inode_str[i] = '0';
    }
    inode_str[6] = ';';

    int s = 7;
    for(int i = 0; i < IBLOCK_ARRAY_SIZE; i++){
        inode_str[s++] = '0';
        inode_str[s++] = '0';
        inode_str[s++] = '0';
        inode_str[s++] = '0';
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


/**
 * Чтение inode таблицы
 */
void read_inode_table(){
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
            inode_table[t].len_iblock = 1000 * num1 + 100 * num2 + 10 * num3 + num4;
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


/**
 * Запись inode таблицы в блоки
 */
void write_inode_table(){
    char buf[BLOCK_SIZE];
    for(int table_block = 0; table_block < INODE_TABLE_BLOCK_COUNT; table_block++) {
        for (int i = 0; i < INODE_TABLE_IN_BLOCK; i++) {
            int j = i * INODE_SIZE; //номер в буфере
            int t = INODE_TABLE_IN_BLOCK * table_block + i; //номер в таблице

            buf[j++] = inode_table[t].type;
            buf[j++] = ';';


            int num = inode_table[t].len_iblock;
            int num1 = num / 1000;
            int num2 = num / 100 % 10;
            int num3 = num / 10 % 10;
            int num4 = num % 10;
            buf[j++] = num1 + '0';
            buf[j++] = num2 + '0';
            buf[j++] = num3 + '0';
            buf[j++] = num4 + '0';
            buf[j++] = ';';


            for (int s = 0; s < IBLOCK_ARRAY_SIZE; s++) {
                int num = inode_table[t].iblock[s];
                int num1 = num / 1000;
                int num2 = num / 100 % 10;
                int num3 = num / 10 % 10;
                int num4 = num % 10;
                buf[j++] = num1 + '0';
                buf[j++] = num2 + '0';
                buf[j++] = num3 + '0';
                buf[j++] = num4 + '0';
            }
        }

        set_sector(buf, table_block + INODE_TABLE_START_BLOCK);
    }
}


void print_inode_table(){
    for(int i = 0; i < INODE_TABLE_SIZE; i++){
        printf("%c;%d;", inode_table[i].type, inode_table[i].len_iblock);
        for(int j = 0; j < IBLOCK_ARRAY_SIZE; j++){
            printf("%d:", inode_table[i].iblock[j]);
        }
        printf("|\n");
    }
}


/**
 * Получение первого блока файла по inode
 * @param inodeID inode файла
 * @return первый блок
 */
int get_block(int inodeID){
    return inode_table[inodeID].iblock[0];
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


#endif //FILESYSTEM_INODE_H
