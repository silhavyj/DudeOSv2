#ifndef _GDT_H_
#define _GDT_H_

#include <stdlib/stdint.h>

typedef struct {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
} __attribute__((packed)) GDT_entry_t;

typedef struct {
    uint16_t size;
    uint32_t start_addr;
} __attribute__((packed)) GDT_descriptor_t;

void init_gdt();

#endif