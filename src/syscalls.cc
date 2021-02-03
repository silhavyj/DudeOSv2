#include <syscalls.h>
#include <heap.h>
#include <process.h>
#include <drivers/screen.h>
#include <stdlib/stdint.h>
#include <filesystem.h>
#include <paging.h>

void handle_systemcall(int_registers_t *regs) {
    switch (regs->eax) {
        case SYSCALL_EXIT:
            syscall_exit();
            break;
        case SYSCALL_PRINT:
            syscall_print(regs);
            break;
        case SYSCALL_READLN:
            syscall_readln();
            break;
        case SYSCALL_MALLOC:
            syscall_malloc();
            break;
        case SYSCALL_FREE:
            syscall_free();
            break;
        case SYSCALL_EXEC_PROGRAM:
            syscall_exec_program();
            break;
        case SYSCALL_TERMINATE_PROCESS:
            syscall_terminate_process();
            break;
        case SYSCALL_PRINT_PROCESSES:
            print_all_processes();
            set_process_as_ready(get_running_process());
            break;
        case SYSCALL_PRINT_ALL_PROGRAMS:
            list_all_files();
            set_process_as_ready(get_running_process());
            break;
        default:
            kprintf("@---KERNEL--- unknown syscall\n");
            break;
    }
}

void syscall_exit() {
    PCB_t *pcb = get_running_process();
    kill_process(pcb);
}

// prints string which address is in esi
void syscall_print(int_registers_t *regs) {
    PCB_t *pcb = get_running_process();
    if (*(char *)regs->esi != '\n') {
        set_color(FOREGROUND_DARKGRAY);
        kprintf("%s (%d): ", pcb->name, pcb->pid);
        reset_color();
        kprintf("%s", (char *)regs->esi);
    }
    set_process_to_run_next(pcb);
}

// reads line from console and copies it in [edi]
void syscall_readln() {
    keyboard_ask_resource();
}

// allocates memory in process heap, size is in EBX, returns address in EAX
void syscall_malloc() {
    PCB_t *pcb = get_running_process();
    pcb->registers.EAX = (uint32_t)heap_malloc(&pcb->heap, pcb->registers.EBX);
    set_process_to_run_next(pcb);
}

// frees memory in process heap, address is in EBX
void syscall_free() {
    PCB_t *pcb = get_running_process();
    heap_free(&pcb->heap, (void *)pcb->registers.EBX);
    set_process_to_run_next(pcb);
}

// creates new process and puts it in ready queue, process name string address is in EBX, process id is returned in EAX
void syscall_exec_program() {
    PCB_t *pcb = get_running_process();
    pcb->registers.EAX = (uint32_t)create_process((char *)pcb->registers.EBX);

    if (pcb->registers.EAX != NULL)
        set_process_as_ready((PCB_t *)pcb->registers.EAX);
    set_process_as_ready(pcb);
}

// terminates process that's pcb is in EBX
void syscall_terminate_process() {
    PCB_t *pcb = get_running_process();
    set_process_as_ready(pcb);
    if (process_exists(pcb->registers.EBX) != 1)
        pcb->registers.EAX = 1;
    else {
        pcb->registers.EAX = 0;
        kill_process((PCB_t *)pcb->registers.EBX);
    }
}