//
// Created by RobertVoropaev on 14.04.19.
//

#ifndef FILESYSTEM_DIRECTORY_H
#define FILESYSTEM_DIRECTORY_H


#include <stdlib.h>
#include "../settings.h"
#include "sectors.h"
#include "../commands/str_proc.h"


struct directory_element{
    int inodeID;
    char name[FILE_NAME_SIZE];
};


/**
 * Первичная запись в блок пустой папки
 * @param block номер блока
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


/**
 * Чтение папки из блока
 * @param directory массив directory_element длиной MAX_FILE_IN_DIRECTORY
 * @param file_count ссылка int*, в которой будет размер массива
 * @param block номер блока
 */
void read_directory(struct directory_element directory[MAX_FILE_IN_DIRECTORY],
                    int* file_count, int block){
    char buf[BLOCK_SIZE];
    get_sector(buf, block);

    if(buf[0] == '|'){
        *file_count = 0;
        return;
    }

    for(int i = 0; i < MAX_FILE_IN_DIRECTORY; i++){
        int s = DIRECTORY_ELEMENT_SIZE * i; // позиция в строке

        int num1 = buf[s++] - '0';
        int num2 = buf[s++] - '0';
        int num3 = buf[s++] - '0';
        int num4 = buf[s++] - '0';
        directory[i].inodeID = num1 * 1000 + num2 * 100 + num3 * 10 + num4;
        s += 1;

        for(int t = 0; t < FILE_NAME_SIZE; t++){
            directory[i].name[t] = buf[s++];
        }

        valid_name(directory[i].name);

        if(buf[s] == '|'){
            *file_count = i + 1;
            return;
        }


    }
}


/**
 * Запись папки в блок
 * @param directory directory массив directory_element длиной file_count
 * @param file_count размер массива
 * @param block номер блока
 */
void write_directory(struct directory_element directory[MAX_FILE_IN_DIRECTORY],
                     int file_count, int block){
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

        char name[FILE_NAME_SIZE];
        strcpy(name, directory[i].name);
        stored_name(name);

        for(int t = 0; t < FILE_NAME_SIZE; t++){
            buf[s++] = name[t];
        }

        buf[s++] = ';';
    }

    if(file_count == 0){
        buf[s] = '|';
    }
    else {
        buf[s - 1] = '|';
    }
    buf[BLOCK_SIZE - 1] = '\0';
    set_sector(buf, block);
}


void print_dir_by_structure(struct directory_element *directory, int len){
    for(int i = 0; i < len; i++){
        printf("%d -> ", i);
        printf("inodeID:%d name:%s \n", directory[i].inodeID, directory[i].name);
    }
}

void print_dir_by_inode(int inode_ID){
    int dir_block = get_block(inode_ID);

    struct directory_element directory[MAX_FILE_IN_DIRECTORY];
    int* file_count = malloc(sizeof(int));
    read_directory(directory, file_count, dir_block);

    print_dir_by_structure(directory, *file_count);
}




#endif //FILESYSTEM_DIRECTORY_H
