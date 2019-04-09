//
// Created by RobertVoropaev on 09.04.19.
//

#ifndef FILESYSTEM_SECTORS_H
#define FILESYSTEM_SECTORS_H

#include <stdio.h>
#include <memory.h>

#define MAIN_FILE "/home/rv/Документы/FileSystem/filesystem.txt"
#define EMPTY_SYMBOL 'a'

const int SECTOR_SIZE = 1024;
const int SECTOR_COUNT = 64;

void filling_main_file(){
    FILE* file = fopen(MAIN_FILE, "w");

    for(int i = 0; i < SECTOR_COUNT; i++){
        for(int j = 0; j < SECTOR_SIZE; j++){
            fprintf(file, "%c", EMPTY_SYMBOL);
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

    fseek(file, sector_num * SECTOR_SIZE, SEEK_SET);
    fgets(buf, SECTOR_SIZE, file);

    fclose(file);
}


/**
 * Запись данных в сектор
 * @param str - строка для записи длиной SECTOR_SIZE с заверщающим нулем
 * @param sector_num - номер сектора с нуля
 */
void set_sector(const char* str, int sector_num){
    if(strlen(str) != SECTOR_SIZE - 1){
        perror("Error argument len in set_sector");
    }

    FILE* file;

    char full[SECTOR_SIZE * SECTOR_COUNT];
    char buf[SECTOR_SIZE];

    file = fopen(MAIN_FILE, "r");

    for(int i = 0; i < SECTOR_COUNT; i++){
        get_sector(buf, i);
        for(int j = 0; j < SECTOR_SIZE; j++){
            full[i * SECTOR_SIZE + j] = buf[j];
        }
    }

    fclose(file);

    file = fopen(MAIN_FILE, "w");

    for(int i = 0; i < SECTOR_COUNT; i++){
        if(i == sector_num){
            for(int j = 0; j < SECTOR_SIZE; j++){
                fprintf(file, "%c", str[j]);
            }
        } else {
            for(int j = 0; j < SECTOR_SIZE; j++){
                fprintf(file, "%c", full[i * SECTOR_SIZE + j]);
            }

        }
    }

    fclose(file);
}


#endif //FILESYSTEM_SECTORS_H
