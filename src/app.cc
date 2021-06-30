#include <app.h>
#include <drivers/screen.h>
#include <stdlib/string.h>

#include <user-programs/exe/idle.bin.h>
#include <user-programs/exe/shell.bin.h>
//#include <user-programs/exe/error.bin.h>
//#include <user-programs/exe/test_malloc.bin.h>
//#include <user-programs/exe/calc.bin.h>
//#include <user-programs/exe/test_fork.bin.h>
//#include <user-programs/exe/test_fork2.bin.h>
//#include <user-programs/exe/test_pipe.bin.h>
#include <user-programs/exe/pipe1.bin.h>
#include <user-programs/exe/pipe2.bin.h>


const program_t programs[] {
    { "idle.exe",  sizeof(exe_idle_bin),  exe_idle_bin  },
    { "shell.exe", sizeof(exe_shell_bin), exe_shell_bin },
    //{ "error.exe", sizeof(exe_error_bin), exe_error_bin },
    //{ "test_malloc.exe", sizeof(exe_test_malloc_bin), exe_test_malloc_bin },
    //{ "calc.exe", sizeof(exe_calc_bin), exe_calc_bin },
    //{ "test_fork.exe", sizeof(exe_test_fork_bin), exe_test_fork_bin },
    //{ "test_fork2.exe", sizeof(exe_test_fork2_bin), exe_test_fork2_bin },
    //{ "test_pipe.exe", sizeof(exe_test_pipe_bin), exe_test_pipe_bin },
    { "pipe1.exe", sizeof(exe_pipe1_bin), exe_pipe1_bin },
    { "pipe2.exe", sizeof(exe_pipe2_bin), exe_pipe2_bin }
};

uint32_t programs_count = 0;

const program_t *get_program(const char *filename) {
    uint32_t i;
    for (i = 0; i < programs_count; i++)
        if (strcmp(filename, programs[i].name) == 0)
            return &programs[i];
    return NULL;
}

void list_all_programs() {
    uint32_t i;
    for (i = 0; i < programs_count; i++)
        kprintf("%s [%d B]\n", programs[i].name, programs[i].size);
}

void init_programs_list() {
   programs_count = sizeof(programs) / sizeof(program_t);
}