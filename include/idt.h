#ifndef _IDT_H_
#define _IDT_H_

#include <stdlib/stdint.h>

#define UNUSED_PARAM(x) (void)(x)

// maximum number of interrupts in the system
#define IDT_ENTRIES_NUM 256

// define presence of an interrupt
#define IDT_PRESENT     1
#define IDT_NOT_PRESENT 0

// interrupt gate types
// see https://wiki.osdev.org/Interrupt_Descriptor_Table
// for more detailed explanation
#define IDT_32_BIT_TASK_GATE      0x5
#define IDT_16_BIT_INTERRUPT_GATE 0x6
#define IDT_16_BIT_TRAP_GATE      0x7
#define IDT_32_BIT_INTERRUPT_GATE 0xE
#define IDT_32_BIT_TRAP_GATE      0xF

// these structures need to
// be packed so the compiler
// does not put any extra bytes
// in between them - it would break it all up

typedef struct {
    uint16_t offset_low;    // offset bits 0..15 of the interrupt handler
    uint16_t selector;      // a code segment selector in GDT or LDT
    uint8_t zero;           // unused, set to 0
    uint8_t type_attr;      // type and attributes, see https://wiki.osdev.org/Interrupt_Descriptor_Table
    uint16_t offset_high;   // offset bits 16..31 of the interrupt handler
} __attribute__((packed)) IDT_entry_t;

// potner to the interrupt descriptor table
typedef struct {
    uint16_t limit;         // size of the table
    uint32_t base;          // start address
} __attribute__((packed)) IDT_descriptor_t;

// registers passed to an interrupt handler from the CPU
typedef struct {
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t eip, cs, eflags;
} int_registers_t;

// registers passed to an ERROR interrupt handler from the CPU
typedef struct {
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t err_code;
    uint32_t eip, cs, eflags;
} int_with_err_registers_t;

void init_idt();

// set an idt entry (one interrupt handler)
// index           - 0-255 in the table
// isr_addr        - interrupt handler address (function)
// segment         - segment of the interrupt - see gdt
// present         - set to 0 for unused interrupts
// dpl             - descriptor privilege level
// storage_segment - set to 0 for interrupt and trap gates
// get_type        - see above
void idt_set_entry(uint8_t index, uint32_t isr_addr, uint16_t segment, uint8_t present, uint8_t dpl, uint8_t storage_segment, uint8_t get_type);

// these functions are called from
// assembly after we've dealt with stuff
// like storing registers on the stack, swiching the segment, ...
// see interrupts.asm
extern "C" {
    void _int0x80_handler(int_registers_t regs);         // system calls
    void _int0x20_handler(int_registers_t regs);         // PIT (system timer)
    void _int0x21_handler(int_registers_t regs);         // keyboard
    void _int0x8_handler();                              // double fault
    void _int0xB_handler(int_with_err_registers_t regs); // segment not present
    void _int0xC_handler();                              // stack fault
    void _int0xD_handler();                              // general protection fault
    void _int0xE_handler(uint32_t faulting_addr);        // page fault
    void _int0xF_handler();                              // unknown interrupt
}

#endif