#include <process.h>
#include <heap.h>
#include <stdlib/list.h>
#include <stdlib/string.h>
#include <filesystem.h>
#include <paging.h>
#include <drivers/screen.h>

list_t *all_processes;              // queue
list_t *ready_processes;            // queue
list_t *waiting_processes;          // queue
list_t *waiting_keyboard_processes; // stack

uint32_t kernel_ESP = 0x6504FE0;    // kernel stack pointer

PCB_t *running_process; // currently running task
PCB_t *idle_process;    // can't be removed! (active when there's no ready process)

void print_pcb(void *addr) {
    PCB_t *pcb = (PCB_t *)addr;

    set_color(FOREGROUND_LIGHTGRAY);
    kprintf("PID=");
    reset_color();
    kprintf("%x ", (uint32_t)pcb);

    set_color(FOREGROUND_LIGHTGRAY);
    kprintf("name=");
    reset_color();
    kprintf("%s ", pcb->process_name);

    set_color(FOREGROUND_LIGHTGRAY);
    kprintf("state=");
    reset_color();
    switch(pcb->state) {
        case PROCESS_STATE_NEW:
            kprintf("NEW");
            break;
        case PROCESS_STATE_RUNNING:
            kprintf("RUNNING");
            break;
        case PROCESS_STATE_WAITING:
            kprintf("WAITING");
            break;
        case PROCESS_STATE_READY:
            kprintf("READY");
            break;
        default:
            kprintf("?");
            break;
    }
    kprintf("\n");
}

void print_all_processes() {
    list_print(all_processes, &print_pcb);
}

void init_process_scheduler() {
    all_processes              = list_create(); // create queue of all processes
    ready_processes            = list_create(); // create queue of ready processes
    waiting_processes          = list_create(); // create queue of waiting processes
    waiting_keyboard_processes = list_create(); // create stack of processes waiting for keyboard
}

uint32_t load_process(uint32_t *pages, const char *process_name) {
    int i, j;
    const file_t *program;
    int page_count;
    char *program_txt;

    uint32_t bytes_to_copy;
    uint32_t bytes_copied = 0;

    // get the file (program)
    program = get_file(process_name);
    if (program == NULL)
        return 0;
    
    // calculate how many pages will be needed
    // for the program code
    page_count = size_in_frames(program->size);

    // copy the program code into the process pages
    for (i = 0; i < page_count; i++) {
        pages[i] = frame_address(frame_alloc());
        map_page(pages[i], pages[i]);
        program_txt = (char *)pages[i];

        bytes_to_copy = program->size - bytes_copied;

        // make sure the max amount of bytes
        // we can store into a single frame is FRAME_SIZE
        if (bytes_to_copy > FRAME_SIZE)
            bytes_to_copy = FRAME_SIZE;

        // copy the code into the frame (page)
        for (j = 0; j < bytes_to_copy; j++)
            program_txt[j] = program->data[i * FRAME_SIZE + j];
        
        bytes_copied += bytes_to_copy;
        unmap_page(pages[i]);
    }
    return page_count;
}

PCB_t *create_process(const char *process_name) {
    int i;
    int program_page_count = 0;

    // we don't need to test againts NULL as the kernel would go into panic mode anyway
    PCB_t *pcb = (PCB_t *)kmalloc(sizeof(PCB_t)); 

    strcpy(pcb->process_name, process_name);
    pcb->state = PROCESS_STATE_NEW;
    pcb->pid = (uint32_t)pcb; 

    // initialize process pages
    for (i = 0; i < PROCESS_MAX_MEMORY_PAGES; i++)
        pcb->mem_pages[i] = PROCESS_UNUSED_PAGE;

    program_page_count = load_process(pcb->mem_pages, pcb->process_name);
    if (program_page_count == 0)
        return 0;   // something went wrong

    // initialize process stack (2 FRAMES = 2 * 4kB = 8kB)
    pcb->mem_pages[PROCESS_MAX_MEMORY_PAGES - 2] = frame_address(frame_alloc());

    // initialize process heap
    // [CODE][HEAP][STACK]
    for (i = program_page_count; i < PROCESS_MAX_MEMORY_PAGES - 3; i++)
        pcb->mem_pages[i] = frame_address(frame_alloc());

    init_heap(&pcb->heap, program_page_count * FRAME_SIZE, i - program_page_count);

    // initialize registers
    pcb->registers.EAX = 0;
    pcb->registers.EBX = 0;
    pcb->registers.ECX = 0;
    pcb->registers.EDX = 0;
    pcb->registers.ESP = (PROCESS_MAX_MEMORY_PAGES - 1) * FRAME_SIZE - 4; // set stack pointer
    pcb->registers.EBP = 0;
    pcb->registers.ESI = 0;
    pcb->registers.EDI = 0;
    pcb->registers.E_FLAGS = 1 << 9; // interrupt flag set
    pcb->registers.EIP = 0;
    pcb->registers.CS = 0x8;
    pcb->registers.SS = 0x10;
    pcb->registers.DS = 0x10;
    pcb->registers.ES = 0x10;
    pcb->registers.FS = 0x10;
    pcb->registers.GS = 0x10;

    list_add_last(all_processes, (void *)pcb->pid);

    return pcb;
}

void process_save_context(PCB_t *pid, int_registers_t *regs) {
    pid->registers.EAX = regs->eax;
    pid->registers.EBX = regs->ebx;
    pid->registers.ECX = regs->ecx;
    pid->registers.EDX = regs->edx;
    pid->registers.ESI = regs->esi;
    pid->registers.EDI = regs->edi;
    pid->registers.ESP = regs->esp + 12;
    pid->registers.EBP = regs->ebp;
    pid->registers.E_FLAGS = regs->eflags;
    pid->registers.EIP = regs->eip;
    pid->registers.CS = regs->cs;
}