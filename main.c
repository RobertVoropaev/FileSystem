#include "inode.h"
#include "blocks_bitmap.h"
#include "directory.h"



int main(int argc, char *argv[]) {
    //filling_main_file();
    //filling_inode_table();
    //filling_blocks_bitmap();
    filling_new_directory(40);

    struct directory_element directory[3];
    directory[0].inodeID = 0;
    directory[1].inodeID = 1;
    directory[2].inodeID = 2;

    char name[10] = "ffffffff++";

    strncpy(directory[0].name, name, 10);
    strncpy(directory[1].name, name, 10);
    strncpy(directory[2].name, name, 10);

    read_directory(directory, 3, 40);
    for(int i = 0; i < 3; i++){
        printf("%d %s \n", directory->inodeID, directory->name);
    }

    return 0;
}
