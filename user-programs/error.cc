#include <system.h>

int main() {
    _uprintf("program to demonstrate a page fault\n");
    char *p = (char *)0xFFFFFFF;
    *p = 'A';
    _uexit();
}