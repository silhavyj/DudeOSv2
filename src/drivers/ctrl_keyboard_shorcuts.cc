#include <drivers/ctrl_keyboard_shorcuts.h>

crl_keyboard_shortcut_t ctr_keyboard_shortcuts[] = {
    {'l', &clear_screen }
};

int ctr_shortcuts_count = sizeof(ctr_keyboard_shortcuts) / sizeof(crl_keyboard_shortcut_t);

crl_keyboard_shortcut_t *get_ctrl_shortcut(char letter) {
    int i;
    for (i = 0; i < ctr_shortcuts_count; i++)
        if (ctr_keyboard_shortcuts[i].letter == letter)
            return &ctr_keyboard_shortcuts[i];
    return NULL;
}