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

void outb(uint16_t port, uint8_t data);
void outw(uint16_t port, uint16_t word);
void outd(uint16_t port, uint32_t dword);
uint8_t inb(uint16_t port);

#endif