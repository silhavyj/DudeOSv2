#ifndef _PAGING_H_
#define _PAGING_H_

#include "stdlib/stdint.h"

// frames
#define FRAMES_COUNT (128 * 1024)
#define FRAMES_START_ADDR 0x100000
#define FRAME_SIZE 4096

// pages
// all numbers are in frames
#define PAGE_DIRECTORY_START_ADDR 0
#define PAGE_TABLES_START_ADDR 1
#define PAGE_TABLE_COUNT 1024
#define USER_PAGES_START_ADDR (PAGE_TABLES_START_ADDR + PAGE_TABLE_COUNT)

// kernel
#define KERNEL_START_ADDR 0x6400000
#define KERNEL_SOURCE_SIZE 256 // 1MB = 256 frames
#define KERNEL_STACK_START_ADDR (KERNEL_START_ADDR + 0x100000 + FRAME_SIZE) // kernel + 1MB + 1kB
#define KERNEL_STACK_SIZE 4
#define VIDEO_MEMORY_START_ADDR (KERNEL_STACK_START_ADDR + (KERNEL_STACK_SIZE + 1) * FRAME_SIZE) // kernel stack + kernel stack size + 4kB
#define KERNEL_HEAP_START_ADDR (VIDEO_MEMORY_START_ADDR + FRAME_SIZE)
#define KERNEL_HEAP_SIZE (1024 * 3) // 1024 * 3 frames = 12MB

typedef struct {
    uint32_t present    : 1;
    uint32_t rw         : 1;
    uint32_t user_mode  : 1;
    uint32_t reserved1  : 2;
    uint32_t accessed   : 1;
    uint32_t dirty      : 1;
    uint32_t reserved2  : 2;
    uint32_t unused     : 3;
    uint32_t frame_addr : 20;
} __attribute__((packed)) page_table_entry_t;

typedef struct {
    page_table_entry_t entry[PAGE_TABLE_COUNT];
} page_table_t;

typedef struct {
    page_table_entry_t entry[PAGE_TABLE_COUNT];
} page_dir_t;

void init_paging();

uint32_t frame_alloc();
void frame_free(uint32_t frame_number);
void frame_set_usage(uint32_t frame_number, uint32_t usage);

uint32_t inline frame_address(uint32_t frame_number);
uint32_t inline frame_number(uint32_t frame_address);
uint32_t size_in_frames(uint32_t size);

void set_page_table_entry(page_table_entry_t *table_entry, uint32_t frame_addr, uint32_t present, uint32_t rw, uint32_t user_mode);
void map_page(uint32_t virtual_addr, uint32_t physical_addr);
void map_page(page_dir_t *page_dir, uint32_t virtual_addr, uint32_t physical_addr);
void unmap_page(uint32_t virtual_addr);
void pages_refresh();

#endif