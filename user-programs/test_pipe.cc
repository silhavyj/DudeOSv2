#include <system.h>
#include <stdlib/memory.h>

#define BUFF_LEN 32

int main() {
    _uprintf("test pipe!!\n");
    
    char *buff = (char *)_umalloc(BUFF_LEN);
    char *buff2 = (char *)_umalloc(BUFF_LEN);
    _ureadln(buff);

    int id = 1;
    if (_upipe(id) == -1) {
        _uprintf("Err while creating a pipe\n");
         _uexit();
    } else {
        if (_upipe(id) == -1) {
            _uprintf("Err while creating a pipe\n");
            _uexit();
        }
        _uprintf("%s %s\n", buff, buff2);
        _upipe_write(1, buff, 32);
        _upipe_release(1);
        _upipe_read(1, buff2, 32);
        _uprintf("buff: %s\n", buff);
        _uprintf("buff2: %s\n", buff2);

        _ufree(buff);
        _ufree(buff2);
    }
    _uexit();
}

void print_str(char *str) {
    while (*str != '\0') {
        _uprintf("%c", *str);
        str++;
    }
}