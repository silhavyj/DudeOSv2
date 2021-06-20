#ifndef _IPC_H_
#define _IPC_H_

#include <stdlib/stdint.h>

#define IPC_BUFF_SIZE 32

typedef struct {
    uint32_t pid_sender;
    uint32_t pid_receiver;
    char buff[IPC_BUFF_SIZE];
} ipc_msg_t;

void init_ipc_msg_queue();
void insert_msg(ipc_msg_t msg);

#endif