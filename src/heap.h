#ifndef _HEAP_H_
#define _HEAP_H_

#include "stdlib/stdint.h"

typedef struct heap_element {
    uint8_t free;
    struct heap_element *next;
    struct heap_element *prev;
    uint32_t data_size;
    void *data;
} heap_element_t;

typedef struct {
    uint32_t base_addr;
    uint32_t free_mem_addr;
    uint32_t size;
    heap_element_t *head;
    heap_element_t *tail;
} heap_t;

void init_heap(heap_t *heap, uint32_t base_addr, uint32_t size_in_frames);
void heap_insert_element(heap_t *heap, heap_element_t *element);
void heap_remove_element(heap_t *heap);
void *heap_malloc(heap_t *heap, uint32_t size);
void heap_free(heap_t *heap, void *addr);

void init_kernel_heap();
void *kmalloc(uint32_t size);
void kfree(void *addr);

#endif