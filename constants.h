//
// Created by RobertVoropaev on 13.04.19.
//

#ifndef FILESYSTEM_CONSTANTS_H
#define FILESYSTEM_CONSTANTS_H

#define MAIN_FILE "/home/rv/Документы/FileSystem/filesystem.txt"

const int BLOCK_SIZE = 2048;
const int BLOCK_COUNT = 1024;
const int FILE_SYSTEM_SIZE = BLOCK_SIZE * BLOCK_COUNT;

const char EMPTY_SYMBOL = 0;

const int INODE_SIZE = 64;
const int IBLOCK_ARRAY_SIZE = INODE_SIZE / 4 - 2;
const int INODE_TABLE_SIZE = BLOCK_COUNT;
const int INODE_TABLE_IN_BLOCK = BLOCK_SIZE / INODE_SIZE;
const int INODE_TABLE_BLOCK_COUNT = INODE_TABLE_SIZE / INODE_TABLE_IN_BLOCK;
const int INODE_TABLE_START_BLOCK = 1;

const int BITMAP_BLOCK = 0;

#endif //FILESYSTEM_CONSTANTS_H
