//
// Created by RobertVoropaev on 14.04.19.
//

#ifndef FILESYSTEM_DIRECTORY_H
#define FILESYSTEM_DIRECTORY_H

#include "constants.h"
#include "sectors.h"

struct directory_element{
    int inodeID;
    char name[FILE_NAME_SIZE];
};

/*
 * Вид записи
 *  0001:name00000000;0012:name200000000|
 */

void filling_new_directory(int block){
    char buf[BLOCK_SIZE];
    for(int i = 0; i < BLOCK_SIZE; i++){
        buf[i] = EMPTY_SYMBOL;
    }

    buf[0] = '|';

    buf[BLOCK_SIZE - 1] = '\0';
    set_sector(buf, block);
}

void read_directory(struct directory_element directory[MAX_FILE_IN_DIRECTORY], int* file_count, int block){
    char buf[BLOCK_SIZE];
    get_sector(buf, block);

    if(buf[0] == '|'){
        *file_count = 0;
        return;
    }

    int flag = 1;
    for(int i = 0; i < MAX_FILE_IN_DIRECTORY; i++){
        for(int j = 0; j < DIRECTORY_ELEMENT_SIZE; j++){
            int s = DIRECTORY_ELEMENT_SIZE * i  + j; // позиция в строке

            int num1 = buf[s++] - '0';
            int num2 = buf[s++] - '0';
            int num3 = buf[s++] - '0';
            int num4 = buf[s++] - '0';
            directory[i].inodeID = num1 * 1000 + num2 * 100 + num3 * 10 + num4;
            s += 1;

            for(int t = 0; t < FILE_NAME_SIZE; t++){
                directory[i].name[t] = buf[s++];
            }
            directory[i].name[FILE_NAME_SIZE - 1] = '\0'; //затирает последний символ имени

            if(buf[s] == '|'){
                flag = 0;
            }
        }
        if(!flag){
            *file_count = i + 1;
        }
    }
}

void write_directory(struct directory_element directory[MAX_FILE_IN_DIRECTORY], int file_count, int block){
    char buf[BLOCK_SIZE];
    for(int i = 0; i < BLOCK_SIZE; i++){
        buf[i] = EMPTY_SYMBOL;
    }

    int s = 0;

    for(int i = 0; i < file_count; i++){
        int num = directory[i].inodeID;
        int num1 = num / 1000;
        int num2 = num / 100 % 10;
        int num3 = num / 10 % 10;
        int num4 = num % 10;

        buf[s++] = num1 + '0';
        buf[s++] = num2 + '0';
        buf[s++] = num3 + '0';
        buf[s++] = num4 + '0';

        buf[s++] = ':';

        for(int t = 0; t < FILE_NAME_SIZE; t++){
            buf[s++] = directory[i].name[t];
        }
        buf[s - 1] = EMPTY_SYMBOL; //затирает последний \0

        buf[s] = ';';
    }

    buf[s] = '|';
    buf[BLOCK_SIZE - 1] = '\0';
    set_sector(buf, block);
}


#endif //FILESYSTEM_DIRECTORY_H
