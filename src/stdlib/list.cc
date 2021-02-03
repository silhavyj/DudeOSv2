#include <stdlib/list.h>
#include <heap.h>

static list_node_t *list_node_create(void *data);

list_t *list_create() {
    list_t *list = (list_t *)kmalloc(sizeof(list_t));
    if (list == NULL)
        return NULL;

    list->first = NULL;
    list->last = NULL;
    list->size = 0;
    return list;
}

void list_free(list_t **list, void(*remove_elem_func)(void *)) {
    list_node_t *tmp;
    list_node_t *curr = (*list)->first;

    while (curr != NULL) {
        tmp = curr;
        curr = curr->next;
        if (remove_elem_func != NULL)
            remove_elem_func(tmp->data);
        kfree(tmp);
    }
    kfree(*list);
    *list = NULL;
}

list_node_t *list_node_create(void *data) {
    list_node_t *node = (list_node_t *)kmalloc(sizeof(list_node_t));
    if (node == NULL)
        return NULL;
    node->data = data;
    node->prev = NULL;
    node->next = NULL;
    return node;
}

void list_add_first(list_t *list, void *data) {
    if (list == NULL)
        return;
    
    list_node_t *node = list_node_create(data);
    if (node == NULL)
        return;

    if (list->first == NULL) {
        list->first = node;
        list->last = node;
    } else {
        node->next = list->first;
        list->first->prev = node;
        list->first = node;
    }
    list->size++;
}

void list_add_last(list_t *list, void *data) {
    if (list == NULL)
        return;

    list_node_t *node = list_node_create(data);
    if (node == NULL)
        return;

    if (list->first == NULL) {
        list->first = node;
        list->last = node;
    } else {
        node->prev = list->last;
        list->last->next = node;
        list->last = node;
    }
    list->size++;
}

void list_print(list_t *list, void(*print_elem_fce)(void *)) {
    if (list == NULL)
        return;
    
    list_node_t *curr = list->first;
    while (curr != NULL) {
        if (print_elem_fce != NULL)
            print_elem_fce(curr->data);
        curr = curr->next;
    }
}

void list_remove(list_t *list, uint32_t index, void(*remove_elem_func)(void *)) {
    if (index >= list->size)
        return;

    uint32_t i;
    list_node_t *curr = list->first;

    for (i = 0; i < index; i++)
        curr = curr->next;

    if (curr == list->first) {
        list->first = list->first->next;
        if (list->first != NULL)
            list->first->prev = NULL;
    } else if (curr == list->last) {
        list->last = list->last->prev;
        if (list->last != NULL)
            list->last->next = NULL;
    } else {
        curr->prev->next = curr->next;
        curr->next->prev = curr->prev;
    }
    if (remove_elem_func != NULL)
        remove_elem_func(curr->data);
    kfree(curr);
    list->size--;
}

void list_remove_data(list_t *list, void *data, void(*remove_elem_func)(void *)) {
    if (list == NULL)
        return;

    uint32_t index = 0;
    list_node_t *curr = list->first;
    
    while (curr != NULL) {
        if (curr->data == data)
            list_remove(list, index, remove_elem_func);
        curr = curr->next;
        index++;
    }
}

uint8_t list_contains(list_t *list, void *data, uint8_t(*cmp_fce)(void *x, void *y)) {
    list_node_t *curr = list->first;
    while (curr != NULL) {
        if (cmp_fce(data, curr->data) == 1)
            return 1;
        curr = curr->next;
    }
    return 0;
}

void *list_get(list_t *list, int32_t index) {
    if (index < 0 || (uint32_t)index >= list->size)
        return NULL;
    
    list_node_t *curr = list->first;
    while (index-- > 0)
        curr = curr->next;
    return curr->data;
}