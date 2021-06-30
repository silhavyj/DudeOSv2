#include <ipc.h>
#include <heap.h>
#include <stdlib/math.h>
#include <stdlib/list.h>
#include <stdlib/memory.h>
#include <drivers/screen.h>

list_t *pipe_queue = NULL;
int last_operation = -1;

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
    pipe->empty = 1;
    pipe->pcb2 = NULL;
    pipe->current_owner = NULL;

    list_add_last(pipe_queue, (void *)pipe);
    return PIPE_SUCCESS;
}

void delete_pipe_from_memory(void *data) {
    kfree(data);
}

static int delete_pipe(uint32_t id) {
    pipe_t *pipe = get_pipe(id);
    if (pipe == NULL)
        return PIPE_FAILURE;
    list_remove_data(pipe_queue, (void *)pipe, &delete_pipe_from_memory);
    return PIPE_SUCCESS;
}

int remove_pcb_from_pipe(uint32_t id, PCB_t *pcb) {
    pipe_t *pipe = get_pipe(id);
    if (pipe == NULL)
        return PIPE_FAILURE;
    if (pcb == pipe->pcb1) {
        pipe->pcb1 = NULL;
        if (pipe->pcb2 == NULL)
            return delete_pipe(id);
    } 
    if (pcb == pipe->pcb2) {
        pipe->pcb2 = NULL;
        if (pipe->pcb1 == NULL)
            return delete_pipe(id);
    }
    return PIPE_SUCCESS;
}

uint32_t get_max_bytes_to_process(uint32_t bytes, pipe_t *pipe) {
    if (pipe->buff_index + bytes >= PIPE_MAX_BUFF_SIZE) 
        return PIPE_MAX_BUFF_SIZE - pipe->buff_index + 1;
    return bytes;
}

int perform_operation(uint32_t pipe_id, char *buffer, uint32_t bytes, uint8_t read, PCB_t *pcb) {
    pipe_t *pipe = get_pipe(pipe_id);
    if (pipe == NULL)
        return PIPE_FAILURE;
    
    if (pipe->pcb1 != pcb && pipe->pcb2 != pcb)
        return PIPE_UNAUTHORIZED_ACCESS;

    if (pipe->locked == 0) {
        pipe->locked = 1;
        pipe->current_owner = pcb;
    }

    uint32_t max_bytes_to_cpy = get_max_bytes_to_process(bytes, pipe);
    if (read == 1) {
        last_operation = PIPE_READING;
        memcpy(buffer, pipe->buff + pipe->buff_index, max_bytes_to_cpy);
    } else if (read == 0) {
        pipe->empty = 0;
        last_operation = PIPE_WRITING;
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
    return perform_operation(pipe_id, buffer, bytes, PIPE_READING, pcb);
}

int pipe_write(uint32_t pipe_id, char *buffer, uint32_t bytes, PCB_t *pcb) {
    return perform_operation(pipe_id, buffer, bytes, PIPE_WRITING, pcb);
}

int pipe_release(uint32_t id, PCB_t *pcb) {
    pipe_t *pipe = get_pipe(id);
    if (pipe == NULL)
        return PIPE_FAILURE;
    if (pipe->pcb1 != pcb && pipe->pcb2 != pcb)
        return PIPE_UNAUTHORIZED_ACCESS;
    
    if (pipe->current_owner == pipe->pcb1 && pipe->pcb2 != NULL) {
        wake_process_on_pipe(pipe->pcb2);
    } else if (pipe->current_owner == pipe->pcb2 && pipe->pcb1 != NULL) {
        wake_process_on_pipe(pipe->pcb1);
    }

    pipe->locked = 0;
    pipe->buff_index = 0; // so the reader/writer can start from the beginning

    if (last_operation == PIPE_READING)
        pipe->empty = 1;
    return PIPE_SUCCESS;
}

int is_pipe_empty(uint32_t id, PCB_t *pcb) {
    pipe_t *pipe = get_pipe(id);
    if (pipe == NULL)
        return PIPE_FAILURE;
    if (pipe->pcb1 != pcb && pipe->pcb2 != pcb)
        return PIPE_UNAUTHORIZED_ACCESS;
    return pipe->empty;
}

int is_pipe_locked(uint32_t id, PCB_t *pcb) {
    pipe_t *pipe = get_pipe(id);
    if (pipe == NULL)
        return PIPE_FAILURE;
    if (pipe->pcb1 != pcb && pipe->pcb2 != pcb)
        return PIPE_UNAUTHORIZED_ACCESS;
    return pipe->locked;
}

int verify_pipe_access(uint32_t id, PCB_t *pcb) {
    pipe_t *pipe = get_pipe(id);
    if (pipe == NULL)
        return PIPE_FAILURE;
    if (pipe->pcb1 != pcb && pipe->pcb2 != pcb)
        return PIPE_UNAUTHORIZED_ACCESS;
    return PIPE_SUCCESS;
}

PCB_t *get_pipe_holder(uint32_t id) {
    pipe_t *pipe = get_pipe(id);
    if (pipe == NULL)
        return NULL;
    return pipe->current_owner;
}

static void delete_int(void *data) {
    kfree(data);
}

int delete_pcb_from_all_pipes(PCB_t *pcb) {
    if (pcb == NULL)
        return PIPE_FAILURE;
    
    list_node_t *curr = pipe_queue->first;
    list_t *to_del = list_create();
    uint32_t *pipe_id;

    while (curr != NULL) {
        pipe_t *pipe = (pipe_t *)curr->data;
        if (pcb == pipe->pcb1) {
            pipe->pcb1 = NULL;
            if (pipe->pcb2 == NULL) {
                pipe_id = (uint32_t *)kmalloc(sizeof(uint32_t));
                *pipe_id = pipe->id;
                list_add_last(to_del, (void *)pipe_id);
            }
        } else if (pcb == pipe->pcb2) {
            pipe->pcb2 = NULL;
            if (pipe->pcb1 == NULL) {
                pipe_id = (uint32_t *)kmalloc(sizeof(uint32_t));
                *pipe_id = pipe->id;
                list_add_last(to_del, (void *)pipe_id);
            }
        }
        curr = curr->next;
    }

    curr = to_del->first;
    while (curr != NULL) {
        delete_pipe(*(int *)curr->data);
        curr = curr->next;
    }
    list_free(&to_del, &delete_int);
    return PIPE_SUCCESS;    
}