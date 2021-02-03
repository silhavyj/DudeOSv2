#include <filesystem.h>
#include <drivers/screen.h>
#include <stdlib/string.h>

#include <user-programs/exe/idle.bin.h>
#include <user-programs/exe/shell.bin.h>
#include <user-programs/exe/error.bin.h>
#include <user-programs/exe/test_malloc.bin.h>
#include <user-programs/exe/calc.bin.h>
#include <user-programs/exe/test_fork.bin.h>

const file_t files[] {
    { "idle.exe",  sizeof(exe_idle_bin),  exe_idle_bin  },
    { "shell.exe", sizeof(exe_shell_bin), exe_shell_bin },
    { "error.exe", sizeof(exe_error_bin), exe_error_bin },
    { "test_malloc.exe", sizeof(exe_test_malloc_bin), exe_test_malloc_bin },
    { "calc.exe", sizeof(exe_calc_bin), exe_calc_bin },
    { "test_fork.exe", sizeof(exe_test_fork_bin), exe_test_fork_bin }
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