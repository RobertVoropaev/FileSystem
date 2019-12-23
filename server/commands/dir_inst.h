//
// Created by RobertVoropaev on 18.04.19.
//

#ifndef FILESYSTEM_DIR_INST_H
#define FILESYSTEM_DIR_INST_H

#include <stdlib.h>
#include "../settings.h"
#include "../core/inode.h"
#include "../core/directory.h"


/**
 * Ищет inode по имени в папке
 * @param name имя файла с \0
 * @param dir_inode inode папки
 * @return inodeID - если файл в папке есть, -1 - если нет
 */
int find_inode_in_directory(const char name[FILE_NAME_SIZE],
                            int dir_inode){
    if(inode_table[dir_inode].type != 'd'){
        fprintf(stderr, "It's not a directory\n");
        return -1;
    }
    int dir_block = inode_table[dir_inode].iblock[0];

    struct directory_element directory[MAX_FILE_IN_DIRECTORY];
    int* file_count = malloc(sizeof(int));
    read_directory(directory, file_count, dir_block);

    for(int i = 0; i < *file_count; i++){
        if(!strcmp(directory[i].name, name)){
            return directory[i].inodeID;
        }
    }
    return -1;
}


/**
 * Находит inode последней папки в пути
 * ex. /dir1/dir2 -> inode dir2
 * "\0" -> ROOT_INODE_ID
 * @param path полный путь к папке
 * @return inode или -1 в случае ошибки
 */
int find_inode_directory(const char path[MAX_PATH_LEN]){
    if(path[0] == '\0'){
        return ROOT_INODE_ID;
    }
    if(path[0] != '/'){
        fprintf(stderr, "Incorrect path\n");
        return -1;
    }

    int i_path = 1;
    int i_name = 0;
    char name[FILE_NAME_SIZE];
    int current_inodeID = ROOT_INODE_ID;
    while(1){
        char c = path[i_path++];
        if(c == '/'){
            name[i_name] = '\0';
            current_inodeID = find_inode_in_directory(name, current_inodeID);
            if(current_inodeID == -1){
                fprintf(stderr, "This file was not found in the directory\n");
                return -1;
            }
            i_name = 0;
        }
        else if(c == '\0'){
            name[i_name] = c;
            current_inodeID = find_inode_in_directory(name, current_inodeID);
            if(current_inodeID == -1){
                fprintf(stderr, "This file was not found in the directory\n");
                return -1;
            }
            return current_inodeID;
        }
        else {
            name[i_name++] = c;
        }
    }
}


/**
 * Удаляет элемет из переданной директории
 * Warning: функция только удаляет файл из блока директории - она не удаляет файл
 * из inode и bitmap.
 * @param directory ссылка на папку
 * @param file_count количество файлов в папке
 * @param inodeID inodeID удаляемого элемента
 */
void delete_inode_in_directory_by_structure(struct directory_element *directory,
                                            int file_count,
                                            int inodeID){
    int index = -1;
    for(int i = 0; i < file_count; i++){
        if(directory[i].inodeID == inodeID){
            index = i;
            break;
        }
    }
    if(index == -1){
        fprintf(stderr, "This file was not found in the directory\n");
        return;
    }
    for(int i = index + 1; i < file_count; i++){
        directory[i - 1].inodeID = directory[i].inodeID;
        strcpy(directory[i - 1].name, directory[i].name);
    }
}


#endif //FILESYSTEM_DIR_INST_H
