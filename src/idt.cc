#include <idt.h>
#include <irq.h>
#include <gdt.h>
#include <support.h>
#include <process.h>
#include <syscalls.h>
#include <drivers/screen.h>
#include <drivers/keyboard.h>

IDT_descriptor_t idt_desc;
IDT_entry_t idt[IDT_ENTRIES_NUM];

// procedures for individual interrupts
// these are written in assembly - interrupts.asm
extern "C" {
    void _isr0x80(void); // system calls
    void _isr0x20(void); // PIT (system timer)
    void _isr0x21(void); // keyboard
    void _isr0x8(void);  // double fault
    void _isr0xB(void);  // segment not present
    void _isr0xC(void);  // stack fault
    void _isr0xD(void);  // general protection fault
    void _isr0xE(void);  // page fault
    void _isr0xF(void);  // unknown interrupt
}

void init_idt() {
    int i;

    // set all interrupts as not preset
    // we'll set up them individually further down below
    for (i = 0; i < IDT_ENTRIES_NUM; i++)
        idt_set_entry(i, 0, 0, IDT_NOT_PRESENT, 0, 0, IDT_32_BIT_TRAP_GATE);

    idt_set_entry(0x8, (uint32_t)_isr0x8, CODE_SEGMENT, IDT_PRESENT, 0, 0, IDT_32_BIT_INTERRUPT_GATE);
    idt_set_entry(0xB, (uint32_t)_isr0xB, CODE_SEGMENT, IDT_PRESENT, 0, 0, IDT_32_BIT_INTERRUPT_GATE);
    idt_set_entry(0xC, (uint32_t)_isr0xC, CODE_SEGMENT, IDT_PRESENT, 0, 0, IDT_32_BIT_INTERRUPT_GATE);
    idt_set_entry(0xD, (uint32_t)_isr0xD, CODE_SEGMENT, IDT_PRESENT, 0, 0, IDT_32_BIT_INTERRUPT_GATE);
    idt_set_entry(0xE, (uint32_t)_isr0xE, CODE_SEGMENT, IDT_PRESENT, 0, 0, IDT_32_BIT_INTERRUPT_GATE);
    idt_set_entry(0xF, (uint32_t)_isr0xF, CODE_SEGMENT, IDT_PRESENT, 0, 0, IDT_32_BIT_INTERRUPT_GATE);

    idt_set_entry(0x80, (uint32_t)_isr0x80, CODE_SEGMENT, IDT_PRESENT, 0, 0, IDT_32_BIT_INTERRUPT_GATE); // system calls
    idt_set_entry(0x20, (uint32_t)_isr0x20, CODE_SEGMENT, IDT_PRESENT, 0, 0, IDT_32_BIT_INTERRUPT_GATE); // PIT - times
    idt_set_entry(0x21, (uint32_t)_isr0x21, CODE_SEGMENT, IDT_PRESENT, 0, 0, IDT_32_BIT_INTERRUPT_GATE); // keyboard

    // initialize idt desctiptor (size and address)
    idt_desc.limit = sizeof(idt);
    idt_desc.base  = (uint32_t)&idt;

    // load desctiptor to the CPU
    _load_idt((uint32_t)&idt_desc);
}

void idt_set_entry(uint8_t index, uint32_t isr_addr, uint16_t segment, uint8_t present, uint8_t dpl, uint8_t storage_segment, uint8_t get_type) {
    idt[index].offset_low  = isr_addr & 0xFFFF;
    idt[index].offset_high = isr_addr >> 16;
    idt[index].selector    = segment;
    idt[index].zero        = 0;
    idt[index].type_attr   = ((present & 0x1) << 7) | 
                             ((dpl & 0x3) << 5) | 
                             ((storage_segment & 0x1) << 4) | 
                             (get_type & 0xF);
}

// system calls handler
void _int0x80_handler(int_registers_t regs) {
    asm("mov %0, %%esp" : : "r" (get_kernel_ESP()));
    PCB_t *running_process = get_running_process();
    process_save_context(running_process, &regs);
    handle_systemcall(&regs);
    switch_process();
}

// PIT (system timer) handler
void _int0x20_handler(int_registers_t regs) {
 asm("mov %0, %%esp" : : "r" (get_kernel_ESP()));
    PCB_t *running_process = get_running_process();
    process_save_context(running_process, &regs);
    set_process_as_ready(running_process);

    _outb(PIC_MASTER_CMD_PORT, PIC_ACK);
    switch_process();
}

// keyboard handler
void _int0x21_handler(int_registers_t regs) {
    UNUSED_PARAM(regs);
    uint8_t scancode = _inb(KEYBOARD_DATA_PORT);
    process_key(scancode);
    _outb(PIC_MASTER_CMD_PORT, PIC_ACK);
}

 // double fault
void _int0x8_handler() {
    PCB_t *pcb = get_running_process();
    asm ("mov %0, %%esp" : : "r" (get_kernel_ESP()));
    set_color(FOREGROUND_LIGHTRED);
    kprintf("#---ERROR--- Double fault : %s pid=0x%x\n", pcb->name, pcb->pid);
    reset_color();
    kill_process(pcb);
    switch_process();
}   

// stack fault
void _int0xC_handler() {
    PCB_t *pcb = get_running_process();
    asm ("mov %0, %%esp" : : "r" (get_kernel_ESP()));
    set_color(FOREGROUND_LIGHTRED);
    kprintf("#---ERROR--- Stack fault : %s pid=0x%x\n", pcb->name, pcb->pid);
    reset_color();
    kill_process(pcb);
    switch_process();
}   

// general protection fault
void _int0xD_handler() {
    PCB_t *pcb = get_running_process();
    asm ("mov %0, %%esp" : : "r" (get_kernel_ESP()));
    set_color(FOREGROUND_LIGHTRED);
    kprintf("#---ERROR--- General protection fault : %s pid=0x%x\n", pcb->name, pcb->pid);
    reset_color();
    kill_process(pcb);
    switch_process();
}                     

// page fault
void _int0xE_handler(uint32_t faulting_addr) {
    PCB_t *pcb = get_running_process();
    asm ("mov %0, %%esp" : : "r" (get_kernel_ESP()));
    set_color(FOREGROUND_LIGHTRED);
    kprintf("\n");
    kprintf("#---ERROR--- Page fault %x: %s pid=%d\n", faulting_addr, pcb->name, pcb->pid);
    reset_color();
    kill_process(pcb);
    switch_process();
}   

// unknown interrupt
void _int0xF_handler() {
    // TODO
}                            

// segment not present
void _int0xB_handler(int_with_err_registers_t regs) {
    UNUSED_PARAM(regs);
    // TODO
}