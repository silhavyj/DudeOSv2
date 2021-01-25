#include <irq.h>
#include <support.h>

void remap_irq() {
    // initialize the PICs
    _outb(PIC_MASTER_CMD_PORT,  0x11);
    _outb(PIC_SLAVE_CMD_PORT,   0x11);

    // new interrupt numbers (give the vector offset)
    _outb(PIC_MASTER_DATA_PORT, 0x20);
    _outb(PIC_SLAVE_DATA_PORT,  0x28);

    // acknowledge the other PIC
    _outb(PIC_MASTER_DATA_PORT, 0x04);
    _outb(PIC_SLAVE_DATA_PORT,  0x02);

    // 8086/88 mode
    _outb(PIC_MASTER_DATA_PORT, 0x01);
    _outb(PIC_SLAVE_DATA_PORT,  0x01);

    // outb(PIC1_PORT + 1, 0xFD); // only keaboard
    _outb(PIC_MASTER_DATA_PORT, 0xFC);   // keyboard and PIT
    _outb(PIC_SLAVE_DATA_PORT,  0xFF);
}

void init_timer() {
    uint32_t divisor = 1193180 / (uint32_t)PIT_FREQUENCY;

    // the divisor number has to be broken up into separate bytes
    uint8_t low_byte  = divisor & 0xFF;
    uint8_t high_byte = (divisor >> 8) & 0xFF;

    _outb(PIT_CMD_PORT, 0x36);   // PIT command write the MSB:LSB countdown value
    _outb(PIT0_PORT, low_byte);
    _outb(PIT0_PORT, high_byte);
}