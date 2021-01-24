#ifndef _SCREEN_H_
#define _SCREEN_H_

#include <drivers/colors.h>
#include <stdlib/stdint.h>

#define MAX_ROWS 25
#define MAX_COLS 80
#define BYTES_PER_SYMBOL 2

#define REGISTER_SCREEN_CTRL 0x3D4
#define REGISTER_SCREEN_DATA 0x3D5
#define CURSOR_HIGH_BYTE_COMMAND 14
#define CURSOR_LOW_BYTE_COMMAND  15

#define DEFAULT_COLOR FOREGROUND_WHITE | BACKGROUND_BLACK
#define ERROR_COLOR FOREGROUND_WHITE | BACKGROUND_RED

#define OFFSET(y, x) ((uint16_t)(2 * ((y) * MAX_COLS + (x))))
#define POS_Y(offset) ((uint8_t)((offset) / (2 * MAX_COLS)))
#define POS_X(offset) ((uint8_t)(((offset) - (POS_Y((offset)) * 2 * MAX_COLS)) / 2))

void set_video_memory_segment(uint16_t seg);
void clear_screen();
void console_scroll(uint32_t lines_count);
void kprint_symbol(char symb, uint32_t pos);
void kprintf(const char *str, ...);
void kprint(const char *str);
uint32_t kprint_str(const char* message, uint32_t cursor);
void set_color(uint8_t val);
void reset_color();
void set_cursor_offset(uint16_t offset);
uint16_t get_cursor_offset();

#endif