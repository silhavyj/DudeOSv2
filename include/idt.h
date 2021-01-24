#ifndef _IDT_H_
#define _IDT_H_

#include <stdlib/stdint.h>

#define IDT_ENTRIES_NUM 256

typedef struct {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t zero;
    uint8_t type_attr;
    uint16_t offset_high;
} __attribute__((packed)) IDT_entry_t;

typedef struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) IDT_descriptor_t;

typedef struct {
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t eip, cs, eflags;
} int_registers_t;

typedef struct {
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t err_code;
    uint32_t eip, cs, eflags;
} int_w_err_registers_t;

void init_idt();
void idt_set_entry(uint8_t index, uint32_t isr_addr, uint16_t segment, uint8_t present, uint8_t dpl, uint8_t storage_segment, uint8_t get_type);

#endif