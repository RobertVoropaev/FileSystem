//
// Created by RobertVoropaev on 17.04.19.
//

#ifndef FILESYSTEM_STRING_PROC_H
#define FILESYSTEM_STRING_PROC_H

#include <memory.h>
#include <stdio.h>
#include "../settings.h"

/**
 * Преобразует строку для хранения в структуре
 * ex. "name++++++" -> "name\0"
 * @param name
 */
void valid_name(char name[FILE_NAME_SIZE]){
    for(int i = 0; i < FILE_NAME_SIZE; i++){
        if(name[i] == EMPTY_SYMBOL){
            name[i] = '\0';
            break;
        }
    }
}

/**
 * Преобразует строку для хранения в блоке
 * ex. "name\0" -> "name++++++"
 * @param name
 */
void stored_name(char name[FILE_NAME_SIZE]){
    int s = 0;
    for(int i = 0; i < FILE_NAME_SIZE; i++){
        if(name[i] == '\0'){
            s = i;
            break;
        }
    }
    for(int i = s; i < FILE_NAME_SIZE; i++){
        name[i] = EMPTY_SYMBOL;
    }
}

/**
 * Разделяет входную строку на комнду и путь
 * ex. mkdir /dir/dir1 -> mkdir and /dir/dir1
 * @param full_command
 * @param command
 * @param path
 */
void get_command_and_path(const char full_command[FULL_COMMAND_SIZE],
                          char command[COMMAND_SIZE],
                          char path[MAX_PATH_LEN]){
    int s = 0;
    int is_command = 1;
    for(int i = 0; i < strlen(full_command); i++){
        char c = full_command[i];
        if(c == ' '){
            s = i + 1;
            command[i] = '\0';
            is_command = 0;
        }
        else {
            if(is_command){
                command[i] = c;
            }
            else {
                path[i - s] = c;
            }
        }
    }
}

/**
 * Получение каталоговой и файловой части пути
 * ex. /dir1/dir2/file - > /dir1/dir2 and file
 * ex. /file -> "\0" and file
 * @param path полный путь с \0
 * @param dir каталоговая часть
 */
int get_dir_and_name_in_path(const char path[MAX_PATH_LEN],
                              char dir[MAX_PATH_LEN],
                              char name[FILE_NAME_SIZE]){
    if(path[0] != '/'){
        fprintf(stderr, "Incorrect path\n");
        return -1;
    }
    dir[0] = '\0';

    int i_path = 1;
    int i_dir = 0;
    int i_name = 0;
    int i_cur = 0;
    char cur_name[FILE_NAME_SIZE];
    cur_name[i_cur++] = '/';
    int is_first = 1;
    while(1){
        char c = path[i_path++];
        if(c == '/'){
            cur_name[i_cur++] = c;
            for(int i = 0; i < i_cur; i++){
                dir[i + i_dir] = cur_name[i];
            }
            i_dir += i_cur;
            i_cur = 0;
            is_first = 0;
        }
        else if(c == '\0'){
            int def = 0;
            for(int i = 0; i < i_cur; i++){
                if(is_first){
                    def = 1;
                    is_first = 0;
                    continue;
                }
                name[i - def] = cur_name[i];
                i_name += i_cur;
            }
            name[i_cur] = '\0';
            dir[i_dir - 1] = '\0';
            break;
        }
        else {
            cur_name[i_cur++] = c;
        }
    }
    return 0;
}

#endif //FILESYSTEM_STRING_PROC_H
