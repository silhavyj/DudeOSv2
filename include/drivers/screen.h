#ifndef _SCREEN_H_
#define _SCREEN_H_

#include <stdlib/stdint.h>
#include <drivers/colors.h>

#define VIDEO_MEMORY 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80

#define REGISTER_SCREEN_CTRL 0x3d4
#define REGISTER_SCREEN_DATA 0x3d5

#define DEFAULT_COLOR FOREGROUND_WHITE | BACKGROUND_BLUE
#define ERROR_COLOR FOREGROUND_WHITE | BACKGROUND_RED
#define ERR_SYMBOL 'E'

#define OFFSET(y, x) ((uint16_t)(2 * ((y) * MAX_COLS + (x))))
#define POS_Y(offset) ((uint8_t)((offset) / (2 * MAX_COLS)))
#define POS_X(offset) ((uint8_t)(((offset) - (POS_Y((offset)) * 2 * MAX_COLS)) / 2))

void set_cursor_offset(uint16_t offset);
void clear_screen();
void color_screen(uint8_t color);
void init_screen_driver();
void kprintf_at(const char *str, uint8_t y, uint8_t x);
void kprintf(const char *str, ...);
void set_color(uint8_t val);
void reset_color();
void kprint_backspace();
void move_cursor_up();
void move_cursor_down();
void move_cursor_right();
void move_cursor_left();

#endif