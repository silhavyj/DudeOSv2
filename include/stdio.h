#ifndef _STDIO_H_
#define _STDIO_H_

#include <stdint.h>

#define MAX_ROWS 25
#define MAX_COLS 80
#define BYTES_PER_SYMBOL 2

void set_video_memory_segment(uint16_t seg);

void clear_creen();
void console_scroll(uint32_t lines_count);
void kprint_symbol(char symb, uint32_t pos);
void kprintf(const char *str, ...);
void kprint(const char *str);
uint32_t kprint_str(const char* message, uint32_t cursor);

#endif