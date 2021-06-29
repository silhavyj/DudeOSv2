#ifndef IPC_H_
#define IPC_H_

#include <stdlib/stdint.h>
#include <process.h>

#define PIPE_BUFF_SIZE 256
#define PIPE_SUCCESS 0
#define PIPE_FAILURE -1

typedef struct {
    uint32_t id;
    char buff[PIPE_BUFF_SIZE];  // buffer 256B
    PCB_t *pcb1;                // first process
    PCB_t *pcb2;                // second process
    uint8_t finished_op;        // flag if the process is done
                                // writing/reading so the other one
                                // can continue
} __attribute__((packed)) pipe_t;

void init_ipc();
int create_pipe(uint32_t id);
int delete_pip(uint32_t id);

#endif