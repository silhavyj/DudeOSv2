#include <ipc.h>
#include <stdlib/list.h>

list_t *ipc_msgs;

void init_ipc_msg_queue() {
    ipc_msgs = list_create();
}

void insert_msg(ipc_msg_t msg) {

}