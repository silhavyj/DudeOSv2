#include <system.h>
#include <stdlib/string.h>
#include <stdlib/memory.h>

#define BUFF_LEN 32
#define N 10
#define PIPE_ID 135

int main() {
    int i;
    if (_ufork() == 0) {
        char *buff_sender = (char *)_umalloc(BUFF_LEN);
        _upipe(PIPE_ID);

        for (i = 0; i < N; i++) {
            memset(buff_sender, 0, BUFF_LEN);
            int_to_str(buff_sender, i+1, 10);
            while (_upipe_write(PIPE_ID, buff_sender, strlen(buff_sender)) == 6);
            _uprintf("sent number: %d\n", i+1);
           // _uprintf("sender - releasing pipe\n");
            _upipe_release(PIPE_ID);
        }
        _ufree(buff_sender);
    } else {
        int x;
        char *buff_receiver = (char *)_umalloc(BUFF_LEN);
        _upipe(PIPE_ID);

        for (i = 0; i < N; i++) {
            memset(buff_receiver, 0, BUFF_LEN);
           // _uprintf("receiver - reading from pipe\n");
            while (_upipe_read(PIPE_ID, buff_receiver, BUFF_LEN-1) == 6);
            x = atoi(buff_receiver);
            _uprintf("received number: %d\n", x);
            _upipe_release(PIPE_ID);
           // _uprintf("receiver - releasing pipe\n");
        }
        _ufree(buff_receiver);
    }
    _uexit();
}