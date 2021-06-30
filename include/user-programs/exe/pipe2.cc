#include <system.h>
#include <stdlib/string.h>
#include <stdlib/memory.h>

#define N 3
#define BUFF_LEN 32
#define PIPE_ID  13

int main() {
    int i, x;
    _upipe(PIPE_ID);
    char *buffer = (char *)_umalloc(BUFF_LEN);

    for (i = 0; i < N; i++) {
        memset(buffer, 0, BUFF_LEN);
        _upipe_read(PIPE_ID, buffer, BUFF_LEN);
        x = atoi(buffer);
        _uprintf("(B) received number: %d\n", x);
        _upipe_release(PIPE_ID);
    }
    _ufree(buffer);
    _upipe_close(PIPE_ID);
    _uexit();
}