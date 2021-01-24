#include <irq.h>
#include <support.h>

void remap_irq() {
    _outb(PIC1_PORT, 0x11);
    _outb(PIC2_PORT, 0x11);

    // new interrupt numbers
    _outb(PIC1_PORT + 1, 0x20);
    _outb(PIC2_PORT + 1, 0x28);

    _outb(PIC1_PORT + 1, 0x04);
    _outb(PIC2_PORT + 1, 0x02);

    _outb(PIC1_PORT + 1, 0x01);
    _outb(PIC2_PORT + 1, 0x01);

    _outb(PIC1_PORT + 1, 0xFC); //keayboard and PIT
    _outb(PIC2_PORT + 1, 0xFF);
}

void init_timer(uint32_t frequency) {
    uint32_t divisor = 1193180 / frequency;
    uint8_t low_byte = divisor & 0xFF;
    uint8_t high_byte = (divisor >> 8) & 0xFF;

    _outb(PIT_CMD_PORT, 0x36);
    _outb(PIT0_PORT, low_byte);
    _outb(PIT0_PORT, high_byte);
}