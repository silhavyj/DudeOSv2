#ifndef IPC_H_
#define IPC_H_

#include <stdlib/stdint.h>
#include <process.h>

#define PIPE_MAX_BUFF_SIZE 256
#define PIPE_SUCCESS 0
#define PIPE_FAILURE -1
#define PIPE_BUFF_FAILURE 2
#define PIPE_UNAUTHORIZED_ACCESS 3

typedef struct {
    uint32_t id;
    uint32_t buff_index;
    char buff[PIPE_MAX_BUFF_SIZE];  // buffer 256B
    PCB_t *pcb1;                    // first process
    PCB_t *pcb2;                    // second process
    uint8_t locked;                 // flag if the process is done
                                    // writing/reading so the other one
                                    // can continue
} __attribute__((packed)) pipe_t;

void init_ipc();
int create_pipe(uint32_t id, PCB_t *pcb);
int delete_pipe(uint32_t id);
int pipe_read(uint32_t pipe_id, char *buffer, uint32_t bytes, PCB_t *pcb);
int pipe_write(uint32_t pipe_id, char *buffer, uint32_t bytes, PCB_t *pcb);
int pipe_release(uint32_t id, PCB_t *pcb);

#endif