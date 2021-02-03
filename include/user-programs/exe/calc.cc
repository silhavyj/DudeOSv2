#include <system.h>
#include <stdlib/string.h>

#define BUFF_LEN 32

int main() {
    char *buff = (char *)_umalloc(BUFF_LEN);
    int x, y;

    _uprintf("enter number x: ");
    _ureadln(buff);
    x = atoi(buff);

    _uprintf("enter number y: ");
    _ureadln(buff);
    y = atoi(buff);

    _uprintf("x + y = %d\n", x + y);
    _uprintf("x - y = %d\n", x - y);
    _uprintf("x * y = %d\n", x * y);

    _uexit();
}