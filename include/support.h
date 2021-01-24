#ifndef _SUPPORT_H_
#define _SUPPORT_H_

#include <stdlib/stdint.h>

extern "C" {
    // sends data to the HW port
    void outb(uint16_t port, uint8_t data);

    // reads data from the HW port
    char inb(uint16_t port);

    // halts the CPU
    void halt();
}

#endif