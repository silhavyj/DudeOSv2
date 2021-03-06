#ifndef _SYSCALLS_H_
#define _SYSCALLS_H_

#include <idt.h>

#define SYSCALL_EXIT               100
#define SYSCALL_PRINT              101
#define SYSCALL_READLN             102
#define SYSCALL_MALLOC             103
#define SYSCALL_FREE               104
#define SYSCALL_EXEC_PROGRAM       105
#define SYSCALL_TERMINATE_PROCESS  106
#define SYSCALL_PRINT_PROCESSES    107
#define SYSCALL_PRINT_ALL_PROGRAMS 108
#define SYSCALL_CLEAR              109
#define SYSCALL_FORK               110
#define SYSCALL_PIPE               111
#define SYSCALL_PIPE_WRITE         112
#define SYSCALL_PIPE_READ          113
#define SYSCALL_PIPE_RELEASE       114
#define SYSCALL_PIPE_CLOSE         115

void handle_systemcall(int_registers_t *regs);
void syscall_exit();
void syscall_print(int_registers_t *regs);
void syscall_readln();
void syscall_malloc();
void syscall_free();
void syscall_exec_program();
void syscall_terminate_process();
void syscall_clear_screen();
void syscall_fork();
void syscall_pipe();
void syscall_pipe_write();
void syscall_pipe_read();
void syscall_pipe_release();
void syscall_pipe_close();

#endif