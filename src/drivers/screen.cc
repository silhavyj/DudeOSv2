#include <drivers/screen.h>
#include <stdlib/string.h>
#include <stdlib/stdarg.h>
#include <stdlib/memory.h>
#include <support.h>

uint8_t color_ctrl;
uint32_t current_cursor_pos;
uint16_t video_memory_segment = 0x18;

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

void kprint_symbol(char symb, uint32_t pos) {
    asm (
        "mov %2, %%ax;"
        "mov %%ax, %%es;"
        "movb %0, %%es:(%1);"
        "movb $0x07, %%es:1(%1);"
        : : "r" (symb), "r" (pos), "r" (video_memory_segment)
        : "eax"
    );
    asm (
        "mov %2, %%ax;"
        "mov %%ax, %%es;"
        "movb %0, %%es:(%1);"
        "movb $0x07, %%es:1(%1);"
        : : "r" (color_ctrl), "r" (pos + 1), "r" (video_memory_segment)
        : "eax"
    );
    set_cursor_offset(pos);
}

void kprintf(const char *str, ...) {
uint32_t i;
    uint32_t args_count = 0;
    va_list valist;
    char buffer[128];
    int value;
    double d_value;

    uint32_t len = strlen(str);

    for (i = 0; i < len; i++)
        if (str[i] == '%') {
            if (i < len - 1 && str[i + 1] != '%')
                args_count++;
        }

    va_start(valist, args_count);
    for (i = 0; i < len; i++) {
        if (str[i] == '%') {
            if (i < len - 1) {
                if (str[i+1] != '%') {
                    memset(buffer, '\0', 32);
                    switch (str[i+1]) {
                        case 'd':
                            value = va_arg(valist, int32_t);
                            int_to_str(buffer, value, 10);
                            kprint(buffer);
                            break;
                        case 's':
                            kprint(va_arg(valist, char *));
                            break;
                        case 'c':
                            *buffer = va_arg(valist, char);
                            buffer[1] = '\0';
                            kprint(buffer);
                            break;
                        case 'x':
                            value = va_arg(valist, int32_t);
                            int_to_str(buffer, value, 16);
                            kprint(buffer);
                            break;
                        case 'f':
                            d_value = va_arg(valist, double);
                            double_to_ascii(d_value, buffer, 3);
                            kprint(buffer);
                            break;
                        default:
                            set_color(ERROR_COLOR);
                            kprint("ERROR\n");
                            reset_color();
                            return;
                    }
                    i++;
                    continue;
                } else {
                    i++;
                }
            }
        }
        buffer[0] = str[i];
        buffer[1] = '\0';
        kprint(buffer);
    }
}

void kprint(const char *str) {
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
            kprint_symbol(message[j], i);
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