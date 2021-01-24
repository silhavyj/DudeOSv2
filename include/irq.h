#ifndef _IRQ_H_
#define _IRQ_H_

#include <stdlib/stdint.h>

#define TIMER_FREQUENCY 20

#define PIC1_PORT 0x20 // master pic command port
#define PIC2_PORT 0xA0 // slave pic command port

#define PIT_CMD_PORT 0x43
#define PIT0_PORT    0x40

void remap_irq();
void init_timer(uint32_t frequency);

#endif  