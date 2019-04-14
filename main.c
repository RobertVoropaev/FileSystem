#include "inode.h"
#include "blocks_bitmap.h"


int main(int argc, char *argv[]) {
    //filling_main_file();
    //filling_inode_table();
    filling_blocks_bitmap();
    read_blocks_bitmap();
    block_bitmap[0] = 0;
    write_blocks_bitmap();


    return 0;
}
