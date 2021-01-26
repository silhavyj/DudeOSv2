#ifndef _HEAP_H_
#define _HEAP_H_

#include <stdlib/stdint.h>

#define ALIGN 8

typedef struct heap {
    uint32_t size;
    uint8_t free;
    struct heap *next;
    struct heap *prev;
    struct heap *next_free;
    struct heap *prev_free;
} heap_t;

void init_kernel_heap();
void init_heap(heap_t *heap, uint32_t base_addr, uint32_t size_in_frames);
void heap_free(heap_t *heap, void *addr);
void *heap_malloc(heap_t *heap, uint32_t size);
void *kmalloc(uint32_t size);
void kfree(void *addr);

#endif