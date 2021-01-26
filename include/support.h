#ifndef _SUPPORT_H_
#define _SUPPORT_H_

#include <stdlib/stdint.h>

extern "C" {
    void _outb(uint16_t port, uint8_t data);
    char _inb(uint16_t port);
    void _halt();
    void _load_gdt(uint32_t addr);
    void _load_idt(uint32_t addr);
    void _enable_interrupts();
    void _disable_interrupts();
    void _enable_paging();
    void _disable_paging();
    void _load_page_dir(uint32_t addr);
}

#endif