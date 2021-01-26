#ifndef _PAGING_H_
#define _PAGING_H_

#include <stdlib/stdint.h>

// frames
#define FRAMES_COUNT      (128 * 1024)  // number of physical frames in the memory
#define FRAME_SIZE        4096          // size of one frame 4kB
#define FRAMES_START_ADDR 0x100000      // start address of the frames

// pages
// all values are in frames
#define PAGE_DIRECTORY_START_ADDR 0     // page directory start addr (0 * 4kB)
#define PAGE_TABLES_START_ADDR    1     // page tables start addr (1 * 4kB)
#define PAGE_TABLE_COUNT       1024     // total number of pages
#define USER_PAGES_START_ADDR (PAGE_DIRECTORY_START_ADDR + PAGE_TABLE_COUNT)

// kernel
#define KERNEL_START_ADDR  0x6400000    // kernel start address
#define KERNEL_SOURCE_SIZE 256          // kernel code size 256 * 4kB = 1MB
#define KERNEL_STACK_START_ADDR (KERNEL_START_ADDR + KERNEL_SOURCE_SIZE + FRAME_SIZE) // kernel + kernel size + 1KB
#define KERNEL_STACK_SIZE 4             // kernel stack size 4 * 4kB = 8kB
#define VIDEO_MEMORY_START (KERNEL_STACK_START_ADDR + KERNEL_STACK_SIZE * FRAME_SIZE + FRAME_SIZE) // kernel stack + kernel stack size + 4kB
#define KERNEL_HEAP_START_ADDR (VIDEO_MEMORY_START + FRAME_SIZE)
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
    page_table_entry_t entries[PAGE_TABLE_COUNT];
} page_table_t;

typedef struct {
    page_table_entry_t entries[PAGE_TABLE_COUNT];
} page_dir_t;

void init_paging();

#endif