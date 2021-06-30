#ifndef CTRL_KEYBOARD_SHORTCUTS_H_
#define CTRL_KEYBOARD_SHORTCUTS_H_

typedef struct {
    char letter;
    void(*handler)(void);
} crl_keyboard_shortcut_t;

crl_keyboard_shortcut_t *get_ctrl_shortcut(char letter);

#endif