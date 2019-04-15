#include <stdlib.h>
#include "inode.h"
#include "blocks_bitmap.h"
#include "directory.h"


int main(int argc, char *argv[]) {
    struct directory_element directory[MAX_FILE_IN_DIRECTORY];
    int file_count1 = 3;
    directory[0].inodeID = 1;
    directory[1].inodeID = 3;
    directory[2].inodeID = 5;

    char str1[10] = "aaaaaaaaa\0";
    char str2[10] = "bbbbbbbbb\0";
    char str3[10] = "ccccccccc\0";
    strcpy(directory[0].name, str1);
    strcpy(directory[1].name, str2);
    strcpy(directory[2].name, str3);
    write_directory(directory, file_count1, 0);

    int* file_count = malloc(sizeof(int));
    read_directory(directory, file_count, 0);

    printf("%d\n", *file_count);
    print_directory(directory, *file_count);


    return 0;
}
