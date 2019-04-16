#include <stdlib.h>
#include "command_base.h"
#include "run.h";

int main(int argc, char *argv[]) {
    init_file_system();
    load_file_system_structure();

    create_file_in_directory(ROOT_INODE_ID, "file0\0", 'f');
    create_file_in_directory(ROOT_INODE_ID, "file1\0", 'f');
    create_file_in_directory(ROOT_INODE_ID, "dir0\0", 'd');
    delete_file_in_directory(ROOT_INODE_ID, "file1\0");
    delete_file_in_directory(ROOT_INODE_ID, "dir0\0");

    struct directory_element directory[MAX_FILE_IN_DIRECTORY];
    int* file_count = malloc(sizeof(int));
    read_directory(directory, file_count, ROOT_DIRECTORY_BLOCK);


    print_directory(directory, *file_count);

    return 0;
}
