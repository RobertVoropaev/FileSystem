#include <stdlib.h>
#include "command_base.h"
#include "run.h";

int main(int argc, char *argv[]) {
    const char path[] = "/dir";
    char dir[MAX_PATH_LEN];
    char name[FILE_NAME_SIZE];
    get_dir_and_name_in_path(path, dir, name);
    printf("%d %s \n", strlen(dir), name);

    int inodeDir = find_inode_directory(dir);
    printf("%d", inodeDir);
    return 0;
}
