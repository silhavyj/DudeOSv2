#ifndef _SYSCALLS_H_
#define _SYSCALLS_H_

#include <idt.h>

#define SYSCALL_EXIT              100
#define SYSCALL_PRINT             101
#define SYSCALL_READLN            102
#define SYSCALL_MALLOC            103
#define SYSCALL_FREE              104
#define SYSCALL_EXEC_PROGRAM      105
#define SYSCALL_TERMINATE_PROCESS 106
#define SYSCALL_PRINT_PROCESSES   107

#endif