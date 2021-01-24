#include <support.h>
#include <stdlib/string.h>
#include <drivers/screen.h>

extern "C" int _kmain() {
    init_screen_driver();
    kprintf("Hello :)\n");

    while (1)
    {
    }
    return 0;
}