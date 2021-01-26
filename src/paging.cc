#include <paging.h>
#include <drivers/screen.h>
#include <support.h>
#include <gdt.h>

uint8_t frames[FRAMES_COUNT];
page_dir_t *page_dir;

void init_paging() {
    int i;
    uint32_t addr;

    // set all frames as free (unused)
    for (i = 0; i < FRAMES_COUNT; i++)
        frames[i] = 0;

    // frames for page directory and page tables are set as in use
    frame_set_usage(PAGE_DIRECTORY_START_ADDR, 1);
    for (i = 0; i < PAGE_TABLE_COUNT; i++)
        frame_set_usage(PAGE_TABLES_START_ADDR + i, 1);

    // get page directory
    page_dir = (page_dir_t*)frame_address(PAGE_DIRECTORY_START_ADDR);

    // all pages are set as not present
    for (i = 0; i < PAGE_TABLE_COUNT; i++)
        set_page_table_entry(&page_dir->entry[i], 0, 0, 0, 0); //not present

    // load the page directory into the CPU
    _load_page_dir((uint32_t)page_dir);

    // map kernel source code where virtual addr = physical addr
    for (i = 0; i < KERNEL_SOURCE_SIZE; i++) {
        addr = KERNEL_START_ADDR + i * FRAME_SIZE;
        map_page(addr, addr);
    }

    // map kernel stack where virtual addr = physical addr
    for (i = 0; i < 4; i++) {
        addr = KERNEL_STACK_START_ADDR + i * FRAME_SIZE;
        map_page(addr, addr);
    }
        
    // mapping virtual video memory
    map_page(VIDEO_MEMORY_START_ADDR, 0xB8000); 

     // mapping kernel heap where virtual addr = physical addr
    for (i = 0; i < KERNEL_HEAP_SIZE; i++) {
        addr = KERNEL_HEAP_START_ADDR + i * FRAME_SIZE;
        map_page(addr, addr);
    }

    // map page directory and page tables
    addr = frame_address(PAGE_DIRECTORY_START_ADDR);
    map_page(addr, addr);

    for (i = 0; i < PAGE_TABLE_COUNT; i++) {
        addr = frame_address(PAGE_TABLES_START_ADDR + i);
        map_page(addr, addr);
    }

    _enable_paging();
    set_video_memory_segment(VIRTUAL_VIDEO_MEMORY);
}

uint32_t frame_alloc() {
    int i, j;
    uint8_t byte;
    uint32_t frame_number;

    for (i = 0; i < FRAMES_COUNT; i++) {
        if (frames[i] != 0xFF) {    // all frames occupied 
            byte = frames[i];    // if not there must be at least one free (0)
            for (j = 0; j < 8; j++) {
                if ((byte & 1) == 0) {
                    frame_number = i * 8 + j;
                    frame_set_usage(frame_number, 1);
                    return frame_number; 
                }
                byte >>= 1;
            }
        }
    }
    panic("all frames are busy!");
    return 0;
}

void frame_free(uint32_t frame_number) {
    frame_set_usage(frame_number, 0);
}

void frame_set_usage(uint32_t frame_number, uint32_t usage) {
    uint32_t byte_num = frame_number / 8;
    uint32_t bit_num  = frame_number % 8;
    uint8_t  mask     = 1 << bit_num;

    if (usage == 1)
        frames[byte_num] |= mask;
    else if (usage == 0)
        frames[byte_num] &= ~mask;
}

uint32_t inline frame_address(uint32_t frame_number) {
    return FRAMES_START_ADDR + frame_number * FRAME_SIZE;
}

uint32_t inline frame_number(uint32_t frame_address) {
    return (FRAMES_START_ADDR - frame_address) / FRAME_SIZE;
}

uint32_t inline size_in_frames(uint32_t size) {
    uint32_t frame_count = size / FRAME_SIZE;
    if (size % FRAME_SIZE != 0)
        frame_count++;
    return frame_count;
}

void set_page_table_entry(page_table_entry_t *table_entry, uint32_t frame_addr, uint32_t present, uint32_t rw, uint32_t user_mode) {
    table_entry->frame_addr = frame_addr;
    table_entry->present    = present;
    table_entry->rw         = rw;
    table_entry->user_mode  = user_mode;

    table_entry->dirty      = 0;
    table_entry->reserved1  = 0;
    table_entry->reserved2  = 0;
    table_entry->accessed   = 0;
    table_entry->unused     = 0;
}

void map_page(uint32_t virtual_addr, uint32_t physical_addr) {
    map_page(page_dir, virtual_addr, physical_addr);
}

void map_page(page_dir_t *page_dir, uint32_t virtual_addr, uint32_t physical_addr) {
    uint32_t page_table_number = virtual_addr >> 22;
    uint32_t page_number       = (virtual_addr >> 12) & 0x3FF;
    page_table_t *page_table   = (page_table_t *)frame_address(PAGE_TABLES_START_ADDR + page_table_number);

    set_page_table_entry(&page_dir->entry[page_table_number], (uint32_t)page_table >> 12, 1, 1, 0);
    set_page_table_entry(&page_table->entry[page_number], physical_addr >> 12, 1, 1, 0);
    frame_set_usage(frame_number(physical_addr), 1);
}

void unmap_page(uint32_t virtual_addr) {
    uint32_t page_table_number = virtual_addr >> 22;
    uint32_t page_number       = (virtual_addr >> 12) & 0x3FF;
    page_table_t *page_table   = (page_table_t *)frame_address(PAGE_TABLES_START_ADDR + page_table_number);

    set_page_table_entry(&page_table->entry[page_number], 0, 0, 0, 0);
}