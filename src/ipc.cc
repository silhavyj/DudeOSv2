#include <ipc.h>
#include <heap.h>
#include <stdlib/list.h>

list_t *pipe_queue = NULL;

void init_ipc() {
    pipe_queue = list_create();
}

pipe_t *get_pipe(uint32_t id) {
    list_node_t *curr = pipe_queue->first;
    while (curr != NULL) {
        pipe_t *pipe = (pipe_t *)curr->data;
        if (pipe->id == id)
            return pipe;
        curr = curr->next;
    }
    return NULL;
}

int create_pipe(uint32_t id) {
    if (get_pipe(id) != NULL)
        return PIPE_FAILURE;

    pipe_t *pipe = (pipe_t *)kmalloc(sizeof(pipe_t));
    if (pipe == NULL)
        return PIPE_FAILURE;

    list_add_last(pipe_queue, (void *)pipe);
    return PIPE_SUCCESS;
}

void delete_pipe_from_memory(void *data) {
    kfree(data);
}

int delete_pip(uint32_t id) {
    pipe_t *pipe = get_pipe(id);
    if (pipe == NULL)
        return PIPE_FAILURE;
    list_remove_data(pipe_queue, (void *)pipe, &delete_pipe_from_memory);
    return PIPE_SUCCESS;
}