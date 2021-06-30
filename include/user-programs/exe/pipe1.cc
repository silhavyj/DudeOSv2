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

    _uexec("pipe2.exe");

    for (i = 0; i < N; i++) {
        memset(buffer, 0, BUFF_LEN);
        x = i+1;
        int_to_str(buffer, x, 10);
        _upipe_write(PIPE_ID, buffer, strlen(buffer));
        _uprintf("(A) sent number:     %d\n", i+1);
        _upipe_release(PIPE_ID);
    }
    _ufree(buffer);
    _upipe_close(PIPE_ID);
    _uexit();
}