#include <heap.h>
#include <paging.h>
#include <drivers/screen.h>

heap_t kernel_heap;

static void merge(memory_segment_t *a, memory_segment_t *b);

void init_heap(heap_t *heap, uint32_t base_addr, uint32_t size_in_frames) {
    heap->first_free_seg = (memory_segment_t *)base_addr;
    heap->first_free_seg->size = size_in_frames * FRAME_SIZE - sizeof(memory_segment_t);
    heap->first_free_seg->free = 1;
    heap->first_free_seg->next = NULL;
    heap->first_free_seg->prev = NULL;
    heap->first_free_seg->next_free = NULL;
    heap->first_free_seg->prev_free = NULL;
}

void heap_free(heap_t *heap, void *addr) {
    memory_segment_t *curr_mem_seg = (memory_segment_t *)addr - 1;
    curr_mem_seg->free = 1;

    if (curr_mem_seg < heap->first_free_seg)
        heap->first_free_seg = curr_mem_seg;

    if (curr_mem_seg->next_free != NULL) {
        if (curr_mem_seg->next_free->prev_free < curr_mem_seg)
            curr_mem_seg->next_free->prev_free = curr_mem_seg;
    }
    if (curr_mem_seg->prev_free != NULL) {
        if (curr_mem_seg->prev_free->next_free > curr_mem_seg) {
            curr_mem_seg->prev_free->next_free = curr_mem_seg;
        }
    }
    if (curr_mem_seg->next != NULL) {
        curr_mem_seg->next->prev = curr_mem_seg;
        if (curr_mem_seg->next->free)
            merge(curr_mem_seg, curr_mem_seg->next);
    }
    if (curr_mem_seg->prev != NULL) {
        curr_mem_seg->prev->next = curr_mem_seg;
        if (curr_mem_seg->prev->free)
            merge(curr_mem_seg, curr_mem_seg->prev);
    }
}

void *heap_malloc(heap_t *heap, uint32_t size) {
    memory_segment_t *curr_mem_seg = heap->first_free_seg; 
    
    while (1) {
        if (curr_mem_seg->size >= size) {
            if (curr_mem_seg->size > size + sizeof(memory_segment_t)) {
                memory_segment_t *new_mem_seg = (memory_segment_t *)((uint32_t)curr_mem_seg + sizeof(memory_segment_t) + size);
                new_mem_seg->size = (uint32_t)curr_mem_seg->size - sizeof(memory_segment_t) - size;

                new_mem_seg->next_free = curr_mem_seg->next_free;
                new_mem_seg->prev_free = curr_mem_seg->prev_free;

                new_mem_seg->next = curr_mem_seg->next;
                new_mem_seg->prev = curr_mem_seg;
                new_mem_seg->free = 1;
                curr_mem_seg->size = size;

                curr_mem_seg->next_free = new_mem_seg;
                curr_mem_seg->next = new_mem_seg;
            }
            if (curr_mem_seg == heap->first_free_seg)
                heap->first_free_seg = curr_mem_seg->next_free;

            if (curr_mem_seg->prev_free != NULL)
                curr_mem_seg->prev_free->next_free = curr_mem_seg->next_free;

            if (curr_mem_seg->next_free != NULL)
                curr_mem_seg->next_free->prev_free = curr_mem_seg->prev_free;

            if (curr_mem_seg->prev != NULL)
                curr_mem_seg->prev->next_free = curr_mem_seg->next_free;

            if (curr_mem_seg->next != NULL)
                curr_mem_seg->next->prev_free = curr_mem_seg->prev_free;

            curr_mem_seg->free = 0;
            return curr_mem_seg + 1;
        }
        if (curr_mem_seg->next_free == NULL)
            break; // ran out of memory

        curr_mem_seg = curr_mem_seg->next_free;
    }
    return NULL;
}

static void merge(memory_segment_t *a, memory_segment_t *b) {
    if (a == NULL || b == NULL)
        return;

    if (a < b) {
        memory_segment_t *c = a;
        a = b;
        b = c;
    }

    a->size += b->size + sizeof(memory_segment_t);
    a->next = b->next;
    a->next_free = b->next_free;

    b->next->prev = a;
    b->next->prev_free = a;
    b->next_free->prev_free = a;
}

void *kmalloc(uint32_t size) {
    void *data = heap_malloc(&kernel_heap, size);
    if (data == NULL)
        panic("Kernel ran out of heap memory!");
    return data;
}

void kfree(void *addr) {
    heap_free(&kernel_heap, addr);
}

void init_kernel_heap() {
    init_heap(&kernel_heap, KERNEL_HEAP_START_ADDR, KERNEL_HEAP_SIZE);
}