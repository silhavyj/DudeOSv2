#ifndef _IRQ_H_
#define _IRQ_H_

#include <stdlib/stdint.h>

// ports associated with PIC
#define PIC_ACK               0x20  // let pic know we're done dealing with the interrupt 
#define PIC_MASTER_CMD_PORT   0x20  // master pic command port
#define PIC_SLAVE_CMD_PORT    0xA0  // slave pic command port
#define PIC_MASTER_DATA_PORT  0x21  // master pic command port
#define PIC_SLAVE_DATA_PORT   0xA1  // slave pic command port

// ports associated with PIT
#define PIT_CMD_PORT  0x43   // mode/command register (write only, a read is ignored)
#define PIT0_PORT     0x40   // channel 0 data port (read/write)
#define PIT1_PORT     0x41   // channel 01 data port (read/write)
#define PIT_FREQUENCY 20     // 20Hz

void remap_irq();
void init_timer();

#endif  