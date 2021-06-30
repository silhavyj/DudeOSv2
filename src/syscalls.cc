#include <syscalls.h>
#include <heap.h>
#include <process.h>
#include <drivers/screen.h>
#include <stdlib/stdint.h>
#include <app.h>
#include <paging.h>
#include <support.h>
#include <stdlib/memory.h>
#include <ipc.h>

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
            list_all_programs();
            set_process_as_ready(get_running_process());
            break;
        case SYSCALL_CLEAR:
            syscall_clear_screen();
            break;
        case SYSCALL_FORK:
            syscall_fork();
            break;
        case SYSCALL_PIPE:
            syscall_pipe();
            break;
        case SYSCALL_PIPE_WRITE:
            syscall_pipe_write();
            break;
        case SYSCALL_PIPE_READ:
            syscall_pipe_read();
            break;
        case SYSCALL_PIPE_RELEASE:
            syscall_pipe_release();
            break;
        default:
            kprintf("@---KERNEL--- unknown syscall\n");
            break;
    }
}

void syscall_exit() {
    PCB_t *pcb = get_running_process();
    kill_process(pcb);
    delete_pcb_from_all_pipes(pcb);
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
    set_process_to_run_next(pcb);
    if (process_exists(pcb->registers.EBX) != 1)
        pcb->registers.EAX = 1;
    else {
        pcb->registers.EAX = 0;
        kill_process((PCB_t *)pcb->registers.EBX);
    }
}

// clears up the screen
void syscall_clear_screen() {
    PCB_t *pcb = get_running_process();
    clear_screen();
    set_process_to_run_next(pcb);
}

void syscall_fork() {
    PCB_t *pcb = get_running_process();

    // create a new process of the same program
    PCB_t *child = create_process(pcb->name);

    // disable paging otherwise it's gonna be a page fault
    _disable_paging(); 
   
    // copy the content of the stack
    memcpy((char *)child->pages[PROCESS_MAX_MEMORY_PAGES - 2], (char *)pcb->pages[PROCESS_MAX_MEMORY_PAGES - 2], FRAME_SIZE);

    // copy the content of the heap
    memcpy((char *)child->pages[pcb->program_page_count], (char *)pcb->pages[pcb->program_page_count], (PROCESS_MAX_MEMORY_PAGES - 3 - pcb->program_page_count) * FRAME_SIZE);
    child->heap = pcb->heap;

    // copy the content of the registers
    memcpy((char *)&child->registers, (char *)&pcb->registers, sizeof(regs_t));

    pcb->registers.EAX = 1;   // you're the parent
    child->registers.EAX = 0; // you're the child

    // once we're done - enable paging again
    _enable_paging();

    set_process_to_run_next(pcb);
    set_process_to_run_next(child);
}

void syscall_pipe() {
    PCB_t *pcb = get_running_process();
    pcb->registers.EAX = create_pipe(pcb->registers.EBX, pcb);
    set_process_to_run_next(pcb);
}

void syscall_pipe_write() {
    PCB_t *pcb = get_running_process();
    int pipe_id = pcb->registers.EBX;
    int pipe_state = verify_pipe_access(pipe_id, pcb);
    if (pipe_state != PIPE_SUCCESS) {
        syscall_exit(); // kill the process if it's trying to access someone else's pipe 
        switch_process();
        return;
    } else {
        if (!is_pipe_empty(pipe_id, pcb) || is_pipe_locked(pipe_id, pcb)) {
            pcb->registers.EAX = PIPE_BLOCKED;
            block_process_on_pipe();
            switch_process();
        } else {
            pcb->registers.EAX = pipe_write(pcb->registers.EBX, (char *)pcb->registers.ECX, pcb->registers.EDX, pcb);
            set_process_to_run_next(pcb);
        }
    }
}

void syscall_pipe_read() {
    PCB_t *pcb = get_running_process();
    int pipe_id = pcb->registers.EBX;
    int pipe_state = verify_pipe_access(pipe_id, pcb);
    if (pipe_state != PIPE_SUCCESS) {
        syscall_exit(); // kill the process if it's trying to access someone else's pipe 
        switch_process();
        return;
    } else {
        if (is_pipe_empty(pipe_id, pcb) || is_pipe_locked(pipe_id, pcb)) {
            pcb->registers.EAX = PIPE_BLOCKED;
            block_process_on_pipe();
            switch_process();
        } else {
            pcb->registers.EAX = pipe_read(pipe_id, (char *)pcb->registers.ECX, pcb->registers.EDX, pcb);
            set_process_to_run_next(pcb);
        }
    }
}

void syscall_pipe_release() {
    PCB_t *pcb = get_running_process();
    int pipe_id = pcb->registers.EBX;
    if (verify_pipe_access(pipe_id, pcb) != PIPE_SUCCESS || get_pipe_holder(pipe_id) != pcb) {
        syscall_exit(); // kill the process if it's trying to access someone else's pipe 
        switch_process();
        return;
    }
    pcb->registers.EAX = pipe_release(pcb->registers.EBX, pcb);
    set_process_to_run_next(pcb);
}