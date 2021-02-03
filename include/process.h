#ifndef _PROCESS_H_
#define _PROCESS_H_

#include <idt.h>
#include <heap.h>
#include <stdlib/stdint.h>

//#define SCHEDULER_DEBUG

#define PROCESS_NAME_LEN 32
#define PROCESS_UNUSED_PAGE 0xFFFFFFFF
#define PROCESS_MAX_MEMORY_PAGES 20

#define PROCESS_STATE_NEW     1
#define PROCESS_STATE_RUNNING 2
#define PROCESS_STATE_WAITING 3
#define PROCESS_STATE_READY   4

typedef struct {
    uint32_t EAX, EBX, ECX, EDX, ESP, EBP, ESI, EDI; // general registers
    uint32_t E_FLAGS;
    uint32_t EIP;
    uint32_t CS, SS, ES, DS, FS, GS; //segment registers
} __attribute__((packed)) regs_t;

typedef struct {
    uint32_t pid;
    uint8_t state;
    char name[PROCESS_NAME_LEN];
    regs_t registers;
    uint32_t pages[PROCESS_MAX_MEMORY_PAGES];
    heap_t heap;
} PCB_t;

void init_process_scheduler();
uint32_t load_process(uint32_t *pages, const char *process_name);
PCB_t *create_process(const char *process_name);
void process_save_context(PCB_t *pcb, int_registers_t *regs);
void print_all_processes();
void process_load_context(PCB_t *pcb);
void switch_process();
void set_process_as_ready(PCB_t *pcb);
void set_process_to_run_next(PCB_t *pcb);
void kill_process(PCB_t *pcb);
PCB_t *get_running_process();
uint32_t get_kernel_ESP();
int process_exists(uint32_t addr);

void keyboard_ask_resource();
void keyboard_create_resource(char *input);

#endif