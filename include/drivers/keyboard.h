#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include <stdlib/stdint.h>

#define KEYBOARD_DATA_PORT 0x60

void init_keyboard();
void process_key(uint8_t scan_code); 
unsigned char convert_scan_code(uint8_t scan_code, uint8_t *pressed);

#endif