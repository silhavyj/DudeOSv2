#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include <stdlib/stdint.h>

#define KEYBOARD_BUFF_SIZE 64     // keyboard buffer is 64B
#define KEYBOARD_DATA_PORT 0x60

// for all codes see - https://wiki.osdev.org/PS/2_Keyboard
#define KEY_CODE_LEFT_SHIFT  0x36
#define KEY_CODE_RIGHT_SHIFT 0x2A
#define KEY_CODE_CTRL        0x1D
#define KEY_CODE_ALT         0x38
#define KEY_CODE_CAPSLOCK    0x3A
#define KEY_CODE_BACK_SPACE  0x0E
#define KEY_CODE_LEFT_ARROW  75
#define KEY_CODE_RIGHT_ARROW 77
#define KEY_CODE_UP_ARROW    72
#define KEY_CODE_DOWN_ARROW  80
#define ENTER_KEY_CODE       28

#define MAX_CMD_HISTORY_RECORDS 10

typedef struct {
    char buffer[KEYBOARD_BUFF_SIZE];
} cmd_history_record_t;

void init_keyboard();
void process_key(uint8_t scan_code); 
unsigned char convert_scan_code(uint8_t scan_code, uint8_t *pressed);
void abort_current_cmd();

#endif