#include <syscalls.h>
#include <heap.h>
#include <process.h>
#include <drivers/screen.h>
#include <stdlib/stdint.h>
#include <filesystem.h>

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
        case SYSCALL_PRINT_FORK:
            syscall_fork();
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

// forks the the process
void syscall_fork() {
    PCB_t *pcb = get_running_process();
    PCB_t *fork_process = create_process(pcb->name);
    
    fork_process->registers.EAX = pcb->registers.EAX;
    fork_process->registers.EBX = pcb->registers.EBX;
    fork_process->registers.ECX = pcb->registers.ECX;
    fork_process->registers.EDX = pcb->registers.EDX;
    fork_process->registers.ESP = pcb->registers.ESP;
    fork_process->registers.EBP = pcb->registers.EBP;
    fork_process->registers.ESI = pcb->registers.ESI;
    fork_process->registers.EDI = pcb->registers.EDI;
    fork_process->registers.E_FLAGS = pcb->registers.E_FLAGS;
    fork_process->registers.EIP = pcb->registers.EIP;
    fork_process->registers.CS = pcb->registers.CS;
    fork_process->registers.SS = pcb->registers.SS;
    fork_process->registers.DS = pcb->registers.DS;
    fork_process->registers.ES = pcb->registers.ES;
    fork_process->registers.FS = pcb->registers.FS;
    fork_process->registers.GS = pcb->registers.GS;

    fork_process->registers.EAX = 0;    // child
    pcb->registers.EAX = 1;             // parent

    //set_process_as_ready(fork_process);
    set_process_as_ready(pcb);
}