#ifndef _HEAP_H_
#define _HEAP_H_

#include <stdlib/stdint.h>

typedef struct memory_segment {
    uint32_t size;
    uint8_t free;
    struct memory_segment *next;
    struct memory_segment *prev;
    struct memory_segment *next_free;
    struct memory_segment *prev_free;
} memory_segment_t;

typedef struct {
    memory_segment_t *first_free_seg;
} heap_t;

void init_kernel_heap();
void init_heap(heap_t *heap, uint32_t base_addr, uint32_t size_in_frames);
void heap_free(heap_t *heap, void *addr);
void *heap_malloc(heap_t *heap, uint32_t size);
void *kmalloc(uint32_t size);
void kfree(void *addr);

#endif