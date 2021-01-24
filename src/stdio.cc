#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>

uint32_t current_cursor_pos;

uint16_t video_memory_segment = 0x18;

void set_video_memory_segment(uint16_t seg) {
    video_memory_segment = seg;
}

void kprint_symbol(char symb, uint32_t pos) {
    asm (
        "mov %2, %%ax;"
        "mov %%ax, %%es;"
        "movb %0, %%es:(%1);"
        "movb $0x07, %%es:1(%1);"
        : : "r" (symb), "r" (pos), "r" (video_memory_segment)
        : "eax"
    );
}

uint32_t kprint_str(const char* message, uint32_t cursor) {
    uint32_t i, j = 0;
    uint32_t line;

    i = cursor;
    line = cursor / BYTES_PER_SYMBOL / MAX_COLS;

    while (message[j] != 0) {
        if (message[j] == '\n') {
            line++;
            if (line >= MAX_ROWS) {
                console_scroll(1);
                line--;
            }
            i = line * MAX_COLS * BYTES_PER_SYMBOL;
            j++;
        } else {
            kprint_symbol(message[j], i);
            j++;
            i += BYTES_PER_SYMBOL;
        }
    }
    return i;
}

void kprint(const char *str) {
    current_cursor_pos = kprint_str(str, current_cursor_pos);
}

void kprintf(const char *str, ...) {
    char formatted_str[2048];
    va_list list;

    va_start(list, str);
    va_stringf(formatted_str, str, list);
    kprint(formatted_str);
}

void console_scroll(uint32_t lines_count) {
    uint32_t bytes_to_cpy = (MAX_ROWS - lines_count) * MAX_COLS * BYTES_PER_SYMBOL;
    uint32_t spaces_to_fill = lines_count * MAX_COLS * BYTES_PER_SYMBOL;
    uint32_t copy_start = MAX_COLS * lines_count * BYTES_PER_SYMBOL;
    uint32_t spaces_start = (MAX_ROWS - lines_count) * MAX_COLS * BYTES_PER_SYMBOL;

    asm (
        "mov %2, %%ax;"
        "mov %%ax, %%es;"
        "mov %0, %%ecx;"
        "mov %1, %%esi;"
        "xor %%edi, %%edi;"
        "rep movsb %%es:(%%esi), %%es:(%%edi)"
        : : "g" (bytes_to_cpy), "g" (copy_start), "m" (video_memory_segment)
        : "eax", "ecx", "edi", "esi"
    );

    asm (
        "mov $0x00, %%al;"
        "mov %0, %%ecx;"
        "mov %1, %%edi;"
        "rep stosb"
        : : "r" (spaces_to_fill), "r" (spaces_start)
        : "eax", "ecx", "edi"
    );
}

void clear_creen() {
    current_cursor_pos = 0;
    console_scroll(MAX_ROWS);
}