cmake_minimum_required(VERSION 3.9)
project(FileSystem C)

set(CMAKE_C_STANDARD 99)

add_executable(FileSystem main.c core/sectors.h core/inode.h settings.h core/inode.h core/bitmap.h core/directory.h commands/base.h commands/run.h commands/main.h commands/str_proc.h commands/initial.h commands/dir_inst.h)