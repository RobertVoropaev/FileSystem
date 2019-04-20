//
// Created by RobertVoropaev on 09.04.19.
//

#ifndef FILESYSTEM_SECTORS_H
#define FILESYSTEM_SECTORS_H


#include <stdio.h>
#include <memory.h>
#include "../settings.h"


/**
 * Первичное заполнение всего файла
 */
void filling_main_file(){
    FILE* file = fopen(MAIN_FILE, "w");

    for(int i = 0; i < BLOCK_COUNT; i++){
        for(int j = 0; j < BLOCK_SIZE; j++){
            if(j == BLOCK_SIZE - 1){
                fprintf(file,"\0");
            } else {
                fprintf(file, "%c", EMPTY_SYMBOL);
            }
        }
    }

    fclose(file);
}


/**
 * Считывание данных из сектора
 * @param buf - строка, в которую будут считаны данные длинной SECTOR_SIZE c завершающим нулем
 * @param sector_num - номер сектора с нуля
 */
void get_sector(char* buf, int sector_num){
    FILE* file = fopen(MAIN_FILE, "r");

    fseek(file, sector_num * BLOCK_SIZE, SEEK_SET);
    fgets(buf, BLOCK_SIZE, file);

    fclose(file);
}


/**
 * Запись данных в сектор
 * @param str - строка для записи длиной SECTOR_SIZE с заверщающим нулем
 * @param sector_num - номер сектора с нуля
 */
void set_sector(const char* str, int sector_num){
    if(strlen(str) != BLOCK_SIZE - 1){
        perror("Error argument len in set_sector");
        printf("%d", strlen(str));
    }

    FILE* file;

    char full[BLOCK_SIZE * BLOCK_COUNT];
    char buf[BLOCK_SIZE];

    file = fopen(MAIN_FILE, "r");

    for(int i = 0; i < BLOCK_COUNT; i++){
        get_sector(buf, i);
        for(int j = 0; j < BLOCK_SIZE; j++){
            full[i * BLOCK_SIZE + j] = buf[j];
        }
    }

    fclose(file);

    file = fopen(MAIN_FILE, "w");

    for(int i = 0; i < BLOCK_COUNT; i++){
        if(i == sector_num){
            for(int j = 0; j < BLOCK_SIZE; j++){
                fprintf(file, "%c", str[j]);
            }
        } else {
            for(int j = 0; j < BLOCK_SIZE; j++){
                fprintf(file, "%c", full[i * BLOCK_SIZE + j]);
            }

        }
    }

    fclose(file);
}

void clear_sector(int sector_num){
    char buf[BLOCK_SIZE];
    for(int i = 0; i < BLOCK_SIZE; i++){
        buf[i] = EMPTY_SYMBOL;
    }
    buf[BLOCK_SIZE - 1] = '\0';

    set_sector(buf, sector_num);
}


#endif //FILESYSTEM_SECTORS_H
