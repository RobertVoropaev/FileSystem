#include <stdlib.h>
#include "command.h"
#include "run.h";

int main(int argc, char *argv[]) {
//    init_file_system();
    load_file_system_structure();
//
//    create("/new\0", 'f');
//    create("/new1\0", 'f');
//    create("/dir\0", 'f');
//
    create("/dir/file", 'd');

    struct directory_element dir[MAX_FILE_IN_DIRECTORY];
    int* file_count = malloc(sizeof(int));
    read_directory(dir, file_count, ROOT_DIRECTORY_BLOCK);

    print_directory(dir, *file_count);

    return 0;
}
