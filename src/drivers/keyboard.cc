#include <drivers/keyboard.h>
#include <drivers/keymap.h>
#include <drivers/screen.h>
#include <stdlib/memory.h>

char keyboard_buffer[256];
uint8_t keyboard_buff_pos = 0;

uint8_t shift_on    = 0;
uint8_t ctrl_on     = 0;
uint8_t alt_on      = 0;
uint8_t capslock_on = 0;

void init_keyboard() {
    memset(keyboard_buffer, 0, sizeof(keyboard_buffer));
}

void process_key(uint8_t scan_code) {
    uint8_t pressed;
    char symbol = convert_scan_code(scan_code, &pressed);
    if (pressed && symbol != 0) {
        kprintf("%c", symbol);
        
        // todo fix overflow
        /* keyboard_buffer[keyboard_buff_pos++] = symbol;
        if (symbol == '\n') {
            keyboard_buffer[keyboard_buff_pos - 1] = 0;
            keyboard_create_recource(keyboard_buffer);
            keyboard_buff_pos = 0;
        } */
    }
}

unsigned char convert_scan_code(uint8_t scan_code, uint8_t *pressed) {
    int original_scancode = scan_code & 0x7F;
    *pressed = (scan_code & 0x80) ? 1 : 0;

    switch (original_scancode) {
        case KEY_CODE_LEFT_SHIFT:
        case KEY_CODE_RIGHT_SHIFT:
            shift_on = !(*pressed);
            return 0;
        case KEY_CODE_CTRL:
            ctrl_on = !(*pressed);
            return 0;
        case KEY_CODE_ALT:
            alt_on = !(*pressed);
            return 0;
        case KEY_CODE_CAPSLOCK:
            if (*pressed)
                capslock_on = !capslock_on;
            return 0;
        case KEY_CODE_BACK_SPACE:
            if (*pressed)
                print_backspace();
            return 0;
    }
    if (shift_on) {
        if (capslock_on)
            return key_map_caps_shifted[original_scancode];
        return key_map_shifted[original_scancode];
    }
    if (capslock_on)
        return key_map_caps_normal[original_scancode];
    return key_map[original_scancode];
}