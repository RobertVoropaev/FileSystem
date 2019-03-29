#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#define MAIN_FILE "/home/rv/Документы/FileSystem/filesystem.txt"
#define SECTOR_SIZE 4
#define SECTOR_COUNT 16
#define EMPTY_SYMBOL 'a'


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
 * @param str - строка для записи с заверщающим нулем
 * @param sector_num - номер сектора с нуля
 */
void set_sector(const char* str, int sector_num){
    FILE* file;

    int  m = (SECTOR_COUNT - sector_num) * SECTOR_SIZE;
    char buf[m];
    strncpy(buf, str, SECTOR_SIZE);
    printf("buf: %s\n", buf);

    file = fopen(MAIN_FILE, "rb");

    char next_sectors[m - SECTOR_SIZE];
    fseek(file, (sector_num + 1) * SECTOR_SIZE, SEEK_SET);
    fgets(next_sectors, m - SECTOR_SIZE, file);
    printf("next_sectors: %s\n", next_sectors);

    fclose(file);

    strstr(buf, next_sectors);
    printf("buf: %s\n", buf);

    file = fopen(MAIN_FILE, "a");

    fseek(file, sector_num * SECTOR_SIZE, SEEK_SET);
    fputs(buf, file);

    fclose(file);
}

int main(int argc, char *argv[]) {
    filling_main_file();

    char buf[SECTOR_SIZE] = "bbb\0";
    set_sector(buf, 1);
    printf("%s", buf);

    return 0;
}
