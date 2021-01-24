#ifndef _GDT_H_
#define _GDT_H_

#include <stdlib/stdint.h>

// define individual segments so 
// we don't have magic number throughout the code
#define CODE_SEGMENT        0x08
#define DATA_SEGMENT        0x10
#define VIDEO_MEMORY        0x18
#define VIRTUAL_VIDE_MEMORY 0x20 // after paging is enabled

// these structures need to
// be packed so the compiler
// does not put any extra bytes
// in between them - it would break it all up

// global descriptor table
// see - https://wiki.osdev.org/Global_Descriptor_Table
typedef struct {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
} __attribute__((packed)) GDT_entry_t;

// gdt descriptor that will be loaded
// to the CPU using assembly
typedef struct {
    uint16_t size;          // limit (size) of the gdt
    uint32_t start_addr;    // address where the gdt starts
} __attribute__((packed)) GDT_descriptor_t;

// init global descriptor table 
void init_gdt();

#endif