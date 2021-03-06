#include <drivers/keyboard.h>
#include <drivers/keymap.h>
#include <drivers/screen.h>
#include <drivers/ctrl_keyboard_shorcuts.h>
#include <stdlib/memory.h>
#include <process.h>

char keyboard_buffer[KEYBOARD_BUFF_SIZE];
uint8_t keyboard_buff_pos = 0;

uint8_t shift_on    = 0;
uint8_t ctrl_on     = 0;
uint8_t alt_on      = 0;
uint8_t capslock_on = 0;

void abort_current_cmd() {
    kprintf("^C\n");
    keyboard_buff_pos = 0;
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
               return KEY_CODE_BACK_SPACE;
            return 0;
        case KEY_CODE_LEFT_ARROW:
        case KEY_CODE_RIGHT_ARROW:
        case KEY_CODE_UP_ARROW:
        case KEY_CODE_DOWN_ARROW:
            return 0;
        case ENTER_KEY_CODE:
            if (*pressed)
                return ENTER_KEY_CODE;
            return 0;
    }
    if (ctrl_on && !shift_on) {
        crl_keyboard_shortcut_t *ctr_short_cut = get_ctrl_shortcut(key_map[original_scancode]);
        if (ctr_short_cut != NULL)
            ctr_short_cut->handler();
        return KEY_CODE_BACK_SPACE; // so the letter doesn't get displayed
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

void process_key(uint8_t scan_code) {
    uint8_t pressed;
    char symbol = convert_scan_code(scan_code, &pressed);

    if (symbol == KEY_CODE_BACK_SPACE) {
        if (keyboard_buff_pos > 0) {
            print_backspace();
            keyboard_buff_pos--;
        }
    } else if (symbol == ENTER_KEY_CODE) {
        kprintf("\n");
        keyboard_buffer[keyboard_buff_pos] = '\0';
        keyboard_create_resource(keyboard_buffer);
        keyboard_buff_pos = 0;
    }
    else if (pressed && symbol != 0) {
        kprintf("%c", symbol);
        keyboard_buffer[keyboard_buff_pos++] = symbol;
        if (keyboard_buff_pos == KEYBOARD_BUFF_SIZE)
            keyboard_buff_pos = 0;
    }
}

void init_keyboard() {
    memset(keyboard_buffer, 0, sizeof(keyboard_buffer));
}