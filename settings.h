//
// Created by RobertVoropaev on 13.04.19.
//

#ifndef FILESYSTEM_CONSTANTS_H
#define FILESYSTEM_CONSTANTS_H

//Главный файл системы
#define MAIN_FILE "/home/rv/Документы/FileSystem/filesystem.txt"

//Команды
#define INIT "init"
#define EXIT "exit"
#define MKDIR "mkdir"
#define TOUCH "touch"
#define RMDIR "rmdir"
#define RM "rm"
#define LS "ls"
#define HELP "help"
#define CAT "cat"
#define ECHO "echo"

//Настроки блоков
const int BLOCK_SIZE = 2048; //Размер одного блока в байтах
const int BLOCK_COUNT = 1024; //Количество блоков
const int FILE_SYSTEM_SIZE = BLOCK_SIZE * BLOCK_COUNT; //Общий размер файловой системы
const char EMPTY_SYMBOL = '+'; //Символ, которые записывается в неиспользуемые ячейки файла
const int STATUS_BLOCK = BLOCK_COUNT - 1;

//Настройки INODE
const int INODE_TABLE_START_BLOCK = 1; //Блок, с которого начинаются INODE
const int INODE_SIZE = 64; //Размер INODE в байтах
const int IBLOCK_ARRAY_SIZE = INODE_SIZE / 4 - 2; //Размер IBLOCK_ARRAY в элементах
const int INODE_TABLE_SIZE = BLOCK_COUNT; //Количеств INODE
const int INODE_TABLE_IN_BLOCK = BLOCK_SIZE / INODE_SIZE; //Количество INODE в одном блоке
const int INODE_TABLE_BLOCK_COUNT = INODE_TABLE_SIZE / INODE_TABLE_IN_BLOCK; //Количество блоков с INODE

//Настройки BITMAP
const int BITMAP_BLOCK = 0; //Блок, в котором хранится BITMAP
const int BITMAP_SIZE = BLOCK_COUNT; //Размер BITMAP

//Настроки папок
const int DIRECTORY_ELEMENT_SIZE = 16; //Размер записи в файле папки
const int FILE_NAME_SIZE = DIRECTORY_ELEMENT_SIZE - 4 - 2; //Размер имени файла
const int MAX_FILE_IN_DIRECTORY = BLOCK_SIZE / DIRECTORY_ELEMENT_SIZE; // Максимальное количество файлов в папке
const int ROOT_DIRECTORY_BLOCK = INODE_TABLE_START_BLOCK + INODE_TABLE_BLOCK_COUNT;
const int ROOT_INODE_ID = 0;

//Настрока команд
const int MAX_PATH_LEN = 64;
const int COMMAND_SIZE = 6;
const int FULL_COMMAND_SIZE = MAX_PATH_LEN + COMMAND_SIZE ;
const int MAX_RESPONSE_LEN = 256;

#endif //FILESYSTEM_CONSTANTS_H
