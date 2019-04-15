//
// Created by RobertVoropaev on 15.04.19.
//

#ifndef FILESYSTEM_COMMAND_H
#define FILESYSTEM_COMMAND_H

#include "sectors.h"
#include "settings.h"
#include "inode.h"
#include "directory.h"
#include "blocks_bitmap.h"

void init_file_system(){
    filling_main_file();
    filling_blocks_bitmap();
    filling_inode_table();
}

void create_directory(){
    
}

#endif //FILESYSTEM_COMMAND_H
