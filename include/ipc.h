#ifndef IPC_H_
#define IPC_H_

#include <stdlib/stdint.h>
#include <process.h>

#define PIPE_MAX_BUFF_SIZE 256
#define PIPE_SUCCESS 2
#define PIPE_FAILURE 3
#define PIPE_BUFF_FAILURE 4
#define PIPE_UNAUTHORIZED_ACCESS 5
#define PIPE_READING 1
#define PIPE_WRITING 0
#define PIPE_BLOCKED 6

typedef struct {
    uint32_t id;                    // id of the pipe
    uint32_t buff_index;            // current index within the buffer
    char buff[PIPE_MAX_BUFF_SIZE];  // buffer 256B
    PCB_t *current_owner;           // who's currently holding the pipe
    PCB_t *pcb1;                    // first process
    PCB_t *pcb2;                    // second process
    uint8_t locked;                 // flag if the process is done
                                    // writing/reading so the other one
                                    // can continue
    uint8_t empty;                  // flag if the pipe buffer is empty
} __attribute__((packed)) pipe_t;

void init_ipc();
int create_pipe(uint32_t id, PCB_t *pcb);
int pipe_read(uint32_t pipe_id, char *buffer, uint32_t bytes, PCB_t *pcb);
int pipe_write(uint32_t pipe_id, char *buffer, uint32_t bytes, PCB_t *pcb);
int pipe_release(uint32_t id, PCB_t *pcb);
int is_pipe_empty(uint32_t id, PCB_t *pcb);
int is_pipe_locked(uint32_t id, PCB_t *pcb);
int verify_pipe_access(uint32_t id, PCB_t *pcb);
PCB_t *get_pipe_holder(uint32_t id);
int delete_pcb_from_all_pipes(PCB_t *pcb);
int remove_pcb_from_pipe(uint32_t id, PCB_t *pcb);

#endif