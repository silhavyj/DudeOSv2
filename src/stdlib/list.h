#ifndef _LIST_H_
#define _LIST_H_

#include "stdint.h"

typedef struct list_node {
    struct list_node *next;
    struct list_node *prev;
    void *data;
} list_node_t;

typedef struct {
    list_node_t *first;
    list_node_t *last;
    uint32_t size;
} list_t;

list_t *list_create();
void list_free(list_t **list, void(*remove_elem_func)(void *));
void list_add_first(list_t *list, void *data);
void list_add_last(list_t *list, void *data);
void list_print(list_t *list, void(*print_elem_fce)(void *));
void list_remove(list_t *list, uint32_t index, void(*remove_elem_func)(void *));
void list_remove_data(list_t *list, void *data, void(*remove_elem_func)(void *));
uint8_t list_contains(list_t *list, void *data, uint8_t(*cmp_fce)(void *x, void *y));
void *list_get(list_t *list, int32_t index);

#endif