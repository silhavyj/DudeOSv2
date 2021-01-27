#include <heap.h>
#include <paging.h>
#include <drivers/screen.h>

heap_t kernel_heap;

void init_heap(heap_t *heap, uint32_t base_addr, uint32_t size_in_frames) {
    heap->base_addr = base_addr;
    heap->free_mem_addr = heap->base_addr;
    heap->size = size_in_frames * FRAME_SIZE;
    heap->head = NULL;
    heap->tail = NULL;
}

void heap_insert_element(heap_t *heap, heap_element_t *element) {
    if (heap->head == NULL) {
        element->next = NULL;
        element->prev = NULL;
        heap->head = element;
        heap->tail = element;
    } else {
        element->prev = heap->tail;
        heap->tail->next = element;
        heap->tail = element;
    }
}

void heap_remove_element(heap_t *heap) {
    if (heap->tail == heap->head) {
        heap->tail = NULL;
        heap->head = NULL;
    } else {
        heap->tail = heap->tail->prev;
        heap->tail->next = NULL;
    }
}

void *heap_malloc(heap_t *heap, uint32_t size) {
    uint32_t physical_size;
    heap_element_t *element;

    physical_size = size + sizeof(heap_element_t);
    if (heap->free_mem_addr + physical_size <= heap->base_addr + heap->size) {
        element = (heap_element_t *)heap->free_mem_addr;
        element->free = 0;
        element->data_size = size;
        element->data = (void *)((uint32_t)element + sizeof(heap_element_t));

        heap_insert_element(heap, element);
        heap->free_mem_addr += physical_size;
        return element->data;
    } else {
        element = heap->head;
        while (element != NULL) {
            if (element->free == 1 && element->next != NULL && element->free == 1) {
                element->data_size += element->next->data_size;
                element->next = element->next->next;
            }
            if (element->free == 1 && element->data_size >= size) {
                element->free = 0;
                return element->data;
            }
            element = element->next;
        }
    }
    return NULL;
}

void heap_free(heap_t *heap, void *addr) {
    if ((uint32_t)addr < heap->base_addr || (uint32_t)addr > (heap->base_addr + heap->size * FRAME_SIZE))
        return;
    
    heap_element_t *element = (heap_element_t *)((uint32_t)addr - sizeof(heap_element_t));
    if (element == heap->tail) {
        heap_remove_element(heap);
        heap->free_mem_addr = (uint32_t)element;
    }
    else 
        element->free = 1;
}

void init_kernel_heap() {
    init_heap(&kernel_heap, KERNEL_HEAP_START_ADDR, KERNEL_HEAP_SIZE);
}

void *kmalloc(uint32_t size) {
    return heap_malloc(&kernel_heap, size);
}

void kfree(void *addr) {
    heap_free(&kernel_heap, addr);
}