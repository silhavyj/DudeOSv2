#include <ipc.h>
#include <heap.h>
#include <stdlib/math.h>
#include <stdlib/list.h>
#include <stdlib/memory.h>
#include <drivers/screen.h>

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

int create_pipe(uint32_t id, PCB_t *pcb) {
    pipe_t *pipe = get_pipe(id);
    if (pipe != NULL) {
        if (pipe->pcb2 != NULL)
            return PIPE_FAILURE;

        pipe->pcb2 = pcb;
        return PIPE_SUCCESS;
    }

    pipe = (pipe_t *)kmalloc(sizeof(pipe_t));
    if (pipe == NULL)
        return PIPE_FAILURE;

    pipe->pcb1 = pcb;
    pipe->id = id;
    pipe->buff_index = 0;
    pipe->locked = 0;
    pipe->pcb2 = NULL;

    list_add_last(pipe_queue, (void *)pipe);
    return PIPE_SUCCESS;
}

void delete_pipe_from_memory(void *data) {
    kfree(data);
}

int delete_pipe(uint32_t id) {
    pipe_t *pipe = get_pipe(id);
    if (pipe == NULL)
        return PIPE_FAILURE;
    list_remove_data(pipe_queue, (void *)pipe, &delete_pipe_from_memory);
    return PIPE_SUCCESS;
}

uint32_t get_max_bytes_to_process(uint32_t bytes, pipe_t *pipe) {
    if (pipe->buff_index + bytes >= PIPE_MAX_BUFF_SIZE) 
        return pipe->buff_index + bytes - PIPE_MAX_BUFF_SIZE;
    return bytes;
}

int perform_operation(uint32_t pipe_id, char *buffer, uint32_t bytes, uint8_t read, PCB_t *pcb) {
    pipe_t *pipe = get_pipe(pipe_id);
    if (pipe == NULL)
        return PIPE_FAILURE;
    
    if (pipe->pcb1 != pcb && pipe->pcb2 != pcb)
        return PIPE_UNAUTHORIZED_ACCESS;

    pipe->locked = 1;

    uint32_t max_bytes_to_cpy = get_max_bytes_to_process(bytes, pipe);
    if (read == 1) {
        memcpy(buffer, pipe->buff + pipe->buff_index, max_bytes_to_cpy);
    } else if (read == 0) {
        memcpy(pipe->buff + pipe->buff_index, buffer, max_bytes_to_cpy);
    } else {
        return PIPE_BUFF_FAILURE;
    }
    pipe->buff_index += max_bytes_to_cpy;

    if (max_bytes_to_cpy == bytes)
        return PIPE_SUCCESS;
    return PIPE_BUFF_FAILURE; 
}

int pipe_read(uint32_t pipe_id, char *buffer, uint32_t bytes, PCB_t *pcb) {
    return perform_operation(pipe_id, buffer, bytes, 1, pcb);
}

int pipe_write(uint32_t pipe_id, char *buffer, uint32_t bytes, PCB_t *pcb) {
    return perform_operation(pipe_id, buffer, bytes, 0, pcb);
}

int pipe_release(uint32_t id, PCB_t *pcb) {
    pipe_t *pipe = get_pipe(id);
    if (pipe == NULL)
        return PIPE_FAILURE;
    if (pipe->pcb1 != pcb && pipe->pcb2 != pcb)
        return PIPE_UNAUTHORIZED_ACCESS;

    pipe->locked = 0;
    pipe->buff_index = 0; // so the reader/writer can start from the beginning
    return PIPE_SUCCESS;
}