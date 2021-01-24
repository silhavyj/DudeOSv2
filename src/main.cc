#include <stdio.h>

extern "C" int _kmain() {
    clear_creen();
    kprintf("Hello from the kernel\n");

    while (1)
    {
    }
    return 0;
}