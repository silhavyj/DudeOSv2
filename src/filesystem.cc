#include <filesystem.h>
#include <drivers/screen.h>
#include <stdlib/string.h>

#include <user-programs/idle.bin.h>
#include <user-programs/shell.bin.h>
#include <user-programs/error.bin.h>

const file_t files[] {
    { "idle.exe", sizeof(idle_bin), idle_bin },
    { "shell.exe", sizeof(shell_bin), shell_bin },
    { "error.exe", sizeof(error_bin), error_bin }
};

uint32_t files_count = 0;

const file_t *get_file(const char *filename) {
    uint32_t i;
    for (i = 0; i < files_count; i++)
        if (strcmp(filename, files[i].name) == 0)
            return &files[i];
    return NULL;
}

void list_all_files() {
    uint32_t i;
    for (i = 0; i < files_count; i++)
        kprintf("%s [%d B]\n", files[i].name, files[i].size);
}

void init_filesystem() {
   files_count = sizeof(files) / sizeof(file_t);
}