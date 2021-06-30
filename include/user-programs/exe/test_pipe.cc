#include <system.h>
#include <stdlib/string.h>
#include <stdlib/memory.h>

#define N 1
#define BUFF_LEN 32
#define PIPE1_ID 135
#define PIPE2_ID 136

int main() {
    int i, x;
    char *buffer = NULL;

    if (_ufork() == 0) {
        buffer = (char *)_umalloc(BUFF_LEN);
        _upipe(PIPE1_ID);
        _upipe(PIPE2_ID);

        for (i = 0; i < N; i++) {
            memset(buffer, 0, BUFF_LEN);
            int_to_str(buffer, i+1, 10);
            _upipe_write(PIPE1_ID, buffer, strlen(buffer));
            _uprintf("(A) sent number (x): %d\n", i+1);
            _upipe_release(PIPE1_ID);

            memset(buffer, 0, BUFF_LEN);
            _upipe_read(PIPE2_ID, buffer, BUFF_LEN);
            x = atoi(buffer);
            _uprintf("(A) received number (2*x): %d\n", x);
            _upipe_release(PIPE2_ID);
        }
    } else {
        buffer = (char *)_umalloc(BUFF_LEN);
        _upipe(PIPE1_ID);
        _upipe(PIPE2_ID);

        for (i = 0; i < N; i++) {
            memset(buffer, 0, BUFF_LEN);
            _upipe_read(PIPE1_ID, buffer, BUFF_LEN);
            x = atoi(buffer);
            _uprintf("(B) received number (x): %d\n", x);
            _upipe_release(PIPE1_ID);

            x = x * 2;
            memset(buffer, 0, BUFF_LEN);
            int_to_str(buffer, x, 10);
            _upipe_write(PIPE2_ID, buffer, strlen(buffer));
            _uprintf("(B) sent number (2*x): %d\n", x);
             if (i < N-1)
            _upipe_release(PIPE2_ID);
        }
    }
    _ufree(buffer);
    _uexit();
}