#include "screen.h"
#include "../stdlib/string.h"
#include "../stdlib/stdarg.h"
#include "../stdlib/memory.h"
#include "../support.h"

uint8_t color_ctrl;
uint32_t current_cursor_pos;
uint16_t video_memory_segment = 0x18;
char screen_buffer[SCREEN_BUFFER_SIZE];

void set_color(uint8_t val) {
    color_ctrl = val;
}

void reset_color() {
    color_ctrl = DEFAULT_COLOR;
}

void set_video_memory_segment(uint16_t seg) {
    video_memory_segment = seg;
}

void clear_screen() {
    reset_color();
    current_cursor_pos = 0;
    console_scroll(MAX_ROWS);
}

void console_scroll(uint32_t lines_count) {
    uint32_t bytes_to_cpy = (MAX_ROWS - lines_count) * MAX_COLS * BYTES_PER_SYMBOL;
    uint32_t spaces_to_fill = lines_count * MAX_COLS * BYTES_PER_SYMBOL;
    uint32_t copy_start = MAX_COLS * lines_count * BYTES_PER_SYMBOL;
    uint32_t spaces_start = (MAX_ROWS - lines_count) * MAX_COLS * BYTES_PER_SYMBOL;

    asm (
        "mov    %2, %%ax;"
        "mov    %%ax, %%es;"
        "mov    %0, %%ecx;"
        "mov    %1, %%esi;"
        "xor    %%edi, %%edi;"
        "rep    movsb %%es:(%%esi), %%es:(%%edi)"
        : : "g" (bytes_to_cpy),
            "g" (copy_start),
            "m" (video_memory_segment)
        : "eax", "ecx", "edi", "esi"
    );

    asm (
        "mov    $0x00, %%al;"
        "mov    %0, %%ecx;"
        "mov    %1, %%edi;"
        "rep    stosb"
        : : "r" (spaces_to_fill),
            "r" (spaces_start)
        : "eax", "ecx", "edi"
    );
}

void kputc(char symb, uint32_t pos) {
    asm (
        "mov    %2, %%ax;"          // move 2nd arg into ax register (video memory segment)
        "mov    %3, %%ebx;"         // move 3rd arg into ebx register (control byte)
        "mov    %%ax, %%es;"        // set the segment register
        "movb   %0, %%es:(%1);"     // the char to print out  (1st byte)
        "movb   %%bx, %%es:1(%1);"  // control byte           (2nd byte)
        : : "r" (symb), 
            "r" (pos), 
            "r" (video_memory_segment), 
            "r" ((uint32_t)color_ctrl)
        : "eax", "ebx"
    );
    set_cursor_offset(pos);
}

void print_backspace() {
    kputc(' ', current_cursor_pos - 2);
}

void kprintf(const char *str, ...) {
    uint32_t i;
    uint32_t args_count = 0;
    va_list valist;
    uint32_t len = strlen(str);

    for (i = 0; i < len; i++)
        if (str[i] == '%') {
            if (i < len - 1 && str[i + 1] != '%')
                args_count++;
        }

    va_start(valist, args_count);
    memset(screen_buffer, '\0', SCREEN_BUFFER_SIZE);
    va_stringf(screen_buffer, str, valist);
    kputs(screen_buffer);
}

void kputs(const char *str) {
    current_cursor_pos = kprint_str(str, current_cursor_pos);
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
            set_cursor_offset(i);
        } else {
            kputc(message[j], i);
            j++;
            i += BYTES_PER_SYMBOL;
        }
    }
    return i;
}

void set_cursor_offset(uint16_t offset) {
    current_cursor_pos = offset;
    offset >>= 1;
    _outb(REGISTER_SCREEN_CTRL, CURSOR_HIGH_BYTE_COMMAND);
    _outb(REGISTER_SCREEN_DATA, (uint8_t)(offset >> 8));
    _outb(REGISTER_SCREEN_CTRL, CURSOR_LOW_BYTE_COMMAND);
    _outb(REGISTER_SCREEN_DATA, (uint8_t)(offset & 0xFF));
}

uint16_t get_cursor_offset() {
    return current_cursor_pos;
}

void panic(const char *debug_info) {
    set_cursor_offset(0);
    set_color(BACKGROUND_RED);
    int i;
    for (i = 0; i < MAX_COLS * MAX_ROWS; i++)
        kprintf(" ");
        
    set_cursor_offset(0);
    set_color(BACKGROUND_RED | FOREGROUND_WHITE);
    kprintf("%s", debug_info);

    while (1) {
        _halt();
    }
}