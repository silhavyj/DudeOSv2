#include <process.h>
#include <heap.h>
#include <stdlib/list.h>
#include <stdlib/string.h>
#include <filesystem.h>
#include <paging.h>
#include <drivers/screen.h>
#include <stdlib/memory.h>

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
    kprintf("%d ", (uint32_t)pcb);

    set_color(FOREGROUND_LIGHTGRAY);
    kprintf("name=");
    reset_color();
    kprintf("%s ", pcb->name);

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
    
    idle_process = create_process("idle.exe");  // load the idle process
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

void switch_process() {
    running_process = idle_process;
    while (ready_processes->first != NULL && running_process == idle_process) {
        running_process = (PCB_t *)ready_processes->first->data;
        list_remove(ready_processes, 0, NULL);
    }
    #ifdef SCHEDULER_DEBUG
        set_color(FOREGROUND_YELLOW);
        kprintf("scheduled: pid=%x name=%s\n", running_process->pid, running_process->name);
        reset_color();
    #endif
    process_load_context(running_process);
}

void set_process_as_ready(PCB_t *pcb) {
    pcb->state = PROCESS_STATE_READY;
    list_add_last(ready_processes, pcb);
}

void set_process_to_run_next(PCB_t *pcb) {
    pcb->state = PROCESS_STATE_READY;
    list_add_first(ready_processes, pcb);
}

void copy_process(PCB_t *dest, PCB_t *src) {
    int i;
    for (i = 0; i < PROCESS_MAX_MEMORY_PAGES; i++)
        if (src->pages[i] != PROCESS_UNUSED_PAGE)
            memcpy((char *)&dest->pages[i], (char *)&src->pages[i], FRAME_SIZE);

    dest->registers.EAX = src->registers.EAX;
    dest->registers.EBX = src->registers.EBX;
    dest->registers.ECX = src->registers.ECX;
    dest->registers.EDX = src->registers.EDX;
    dest->registers.ESP = src->registers.ESP;
    dest->registers.EBP = src->registers.EBP;
    dest->registers.ESI = src->registers.ESI;
    dest->registers.EDI = src->registers.EDI;
    dest->registers.E_FLAGS = src->registers.E_FLAGS;
    dest->registers.EIP = src->registers.EIP;
    dest->registers.CS = src->registers.CS;
    dest->registers.SS = src->registers.SS;
    dest->registers.DS = src->registers.DS;
    dest->registers.ES = src->registers.ES;
    dest->registers.FS = src->registers.FS;
    dest->registers.GS = src->registers.GS;
}

PCB_t *create_process(const char *process_name) {
    int i;
    int program_page_count = 0;

    // we don't need to test againts NULL as the kernel would go into panic mode anyway
    PCB_t *pcb = (PCB_t *)kmalloc(sizeof(PCB_t)); 

    strcpy(pcb->name, process_name);
    pcb->state = PROCESS_STATE_NEW;
    pcb->pid = (uint32_t)pcb; 

    // initialize process pages
    for (i = 0; i < PROCESS_MAX_MEMORY_PAGES; i++)
        pcb->pages[i] = PROCESS_UNUSED_PAGE;

    program_page_count = load_process(pcb->pages, pcb->name);
    if (program_page_count == 0)
        return 0;   // something went wrong

    // initialize process stack (2 FRAMES = 2 * 4kB = 8kB)
    pcb->pages[PROCESS_MAX_MEMORY_PAGES - 2] = frame_address(frame_alloc());

    // initialize process heap
    // [CODE][HEAP][STACK]
    for (i = program_page_count; i < PROCESS_MAX_MEMORY_PAGES - 3; i++)
        pcb->pages[i] = frame_address(frame_alloc());

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

void process_save_context(PCB_t *pcb, int_registers_t *regs) {
    pcb->registers.EAX     = regs->eax;
    pcb->registers.EBX     = regs->ebx;
    pcb->registers.ECX     = regs->ecx;
    pcb->registers.EDX     = regs->edx;
    pcb->registers.ESI     = regs->esi;
    pcb->registers.EDI     = regs->edi;
    pcb->registers.ESP     = regs->esp + 12;
    pcb->registers.EBP     = regs->ebp;
    pcb->registers.E_FLAGS = regs->eflags;
    pcb->registers.EIP     = regs->eip;
    pcb->registers.CS      = regs->cs;
}

void process_load_context(PCB_t *pcb) {
    int i;
    pcb->state = PROCESS_STATE_RUNNING;

    // memory switch
    for (i = 0; i < PROCESS_MAX_MEMORY_PAGES; i++)
        if (pcb->pages[i] != PROCESS_UNUSED_PAGE) {
            unmap_page(i * FRAME_SIZE);
            map_page(i * FRAME_SIZE, pcb->pages[i]);
        }

    pages_refresh();

    asm (
        "push %0;"
        "pop %%esp;"
        "push %1;"
        "push %2;"
        "push %3;"
        : : "g" (pcb->registers.ESP), "g" (pcb->registers.E_FLAGS), "g" (pcb->registers.CS),
            "g" (pcb->registers.EIP)
    );
    asm(
        "push %0;"
        "push %1;"
        "push %2;"
        "push %3;"
        : :  "g" (pcb->registers.DS), "g" (pcb->registers.ES), "g" (pcb->registers.FS),
             "g" (pcb->registers.GS)
    );
    asm
    (
        "push %0;"
        "push %1;"
        "push %2;"
        "push %3;"
        "push %4;"
        "push %5;"
        "push %6;"
        "push %7;"
        : : "g" (pcb->registers.EAX), "g" (pcb->registers.ECX), "g" (pcb->registers.EDX),
            "g" (pcb->registers.EBX), "g" (pcb->registers.ESP), "g" (pcb->registers.EBP),
            "g" (pcb->registers.ESI), "g" (pcb->registers.EDI)
    );
    asm
    (
        "popa;"
        "pop %%gs;"
        "pop %%fs;"
        "pop %%es;"
        "pop %%ds;"
        "iret;"
        : :
    );
}

void kill_process(PCB_t *pcb) {
    int i;

    // free all pages used by the process
    for (i = 0; i < PROCESS_MAX_MEMORY_PAGES; i++)
        if (pcb->pages[i] != PROCESS_UNUSED_PAGE) {
            frame_free(frame_number(pcb->pages[i]));
            unmap_page(pcb->pages[i]);
        }

    // remove pcb from queues and stack
    list_remove_data(all_processes, (void *)pcb->pid, NULL);
    list_remove_data(ready_processes, (void *)pcb->pid, NULL);
    list_remove_data(waiting_processes, (void *)pcb->pid, NULL);
    list_remove_data(waiting_keyboard_processes, (void *)pcb->pid, NULL);

    // clean memory
    kfree(pcb);
}

PCB_t *get_running_process() {
    return running_process;
}

uint32_t get_kernel_ESP() {
    return kernel_ESP;
}

void keyboard_ask_resource() {
    running_process->state = PROCESS_STATE_WAITING;
    list_remove_data(ready_processes, (void *)running_process->pid, NULL);
    list_add_last(waiting_processes, (void *)running_process->pid);
    list_add_first(waiting_keyboard_processes, (void *)running_process->pid);
}

void keyboard_create_resource(char *input) {
    int i;
    PCB_t *pcb = (PCB_t *)list_get(waiting_keyboard_processes, 0);
    if (pcb == NULL)
        return;
    
    // remove process from waiting queues
    list_remove(waiting_keyboard_processes, 0, NULL);
    list_remove_data(waiting_processes, (void *)pcb->pid, NULL);

    pcb->state = PROCESS_STATE_READY;

    // copy input buffer to process memory - address is in EDI
     for (i = 0; i < PROCESS_MAX_MEMORY_PAGES; i++)
        if (pcb->pages[i] != PROCESS_UNUSED_PAGE)
            map_page(i * FRAME_SIZE, pcb->pages[i]);
    
    strcpy((char *)pcb->registers.EDI, input);

    // add process to ready queue
    list_add_first(ready_processes, (void *)pcb->pid);

    set_process_as_ready(running_process);
}

uint8_t pcb_comapre(void *pcb1, void *pcb2) {
    return (uint32_t)pcb1 == (uint32_t)pcb2;
}

int process_exists(uint32_t addr) {
    return list_contains(all_processes, (void *)addr, &pcb_comapre) == 1;
}