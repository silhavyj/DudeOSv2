#include <drivers/screen.h>
#include <stdlib/math.h>
#include <stdlib/string.h>
#include <stdlib/stdarg.h>
#include <stdlib/memory.h>
#include <support.h>

static uint8_t color_ctrl;

void set_cursor_offset(uint16_t offset);
static void set_cursor_offset_pos(int16_t y, int16_t x);
static uint16_t kprint_char_at(char c, uint8_t y, uint8_t x);
static void kprint_char(char c);
static uint16_t get_cursor_offset();
static void scroll_down();

void set_color(uint8_t val) {
    color_ctrl = val;
}

void reset_color() {
    color_ctrl = DEFAULT_COLOR;
}

void init_screen_driver() {
    clear_screen();
    reset_color();
}

static void set_cursor_offset_pos(int16_t y, int16_t x) {
    y = min(max(0, y), MAX_ROWS - 1);
    x = min(max(0, x), MAX_COLS - 1);
    uint32_t offset = OFFSET(y, x);
    set_cursor_offset(offset);
}

void move_cursor_up() {
    uint32_t offset = get_cursor_offset();
    set_cursor_offset_pos(POS_Y(offset) - 1, POS_X(offset));
}

void move_cursor_down() {
    uint32_t offset = get_cursor_offset();
    set_cursor_offset_pos(POS_Y(offset) + 1, POS_X(offset));
}

void move_cursor_right() {
    uint32_t offset = get_cursor_offset();
    set_cursor_offset_pos(POS_Y(offset), POS_X(offset) + 1);
}

void move_cursor_left() {
    uint32_t offset = get_cursor_offset();
    set_cursor_offset_pos(POS_Y(offset), POS_X(offset) - 1);
}

void color_screen(uint8_t color) {
    uint16_t i;
    char *video_memory = (char *)VIDEO_MEMORY;
    uint16_t screen_size = MAX_COLS * MAX_ROWS;

    for (i = 0; i < screen_size; i++) {
        video_memory[i * 2] = ' ';
        video_memory[i * 2 + 1] = color;
    }
    set_cursor_offset(OFFSET(0, 0));
}

void clear_screen() {
    uint16_t i;
    char *video_memory = (char *)VIDEO_MEMORY;
    uint16_t screen_size = MAX_COLS * MAX_ROWS;

    for (i = 0; i < screen_size; i++) {
        video_memory[i * 2] = ' ';
        video_memory[i * 2 + 1] = DEFAULT_COLOR;
    }
    set_cursor_offset(OFFSET(0, 0));
}

static void kprint_char(char c) {
    uint16_t offset = get_cursor_offset();
    uint8_t y = POS_Y(offset);
    uint8_t x = POS_X(offset);
    kprint_char_at(c, y, x);
}

static uint16_t kprint_char_at(char c, uint8_t y, uint8_t x) {
    char *video_memory = (char *)VIDEO_MEMORY;
    uint16_t offset;

    if (y >= MAX_ROWS || x >= MAX_COLS) {
        offset = 2 * MAX_COLS * MAX_ROWS;
        video_memory[offset - 2] = ERR_SYMBOL;
        video_memory[offset - 1] = ERROR_COLOR;
        return OFFSET(MAX_ROWS, MAX_COLS);
    }
    offset = OFFSET(y, x);
    switch (c) {
        case '\n':
            offset = OFFSET(y + 1, 0);
            break;
        case 0x08:
            video_memory[offset] = ' ';
            video_memory[offset + 1] = color_ctrl;
            break;
        default:
            video_memory[offset] = c;
            video_memory[offset + 1] = color_ctrl;
            offset += 2;
            break;
    }
    if (offset >= 2 * MAX_ROWS * MAX_COLS) {
        scroll_down();
        char *last_line = (char *)(OFFSET(MAX_ROWS - 1, 0) + VIDEO_MEMORY);
        uint32_t i;
        for (i = 0; i < 2 * MAX_COLS; i += 2) {
            last_line[i] = 0;
            last_line[i+1] = color_ctrl;
        }
        offset -= 2 * MAX_COLS;
    }
    set_cursor_offset(offset);
    return offset;
}

void kprint_backspace() {
    uint32_t offset = get_cursor_offset() - 2;
    uint8_t y = POS_Y(offset);
    uint8_t x = POS_X(offset);
    kprint_char_at(0x08, y, x);
}

static void scroll_down() {
    uint32_t i;
    for (i = 1; i < MAX_ROWS; i++) {
        char *src = (char *)(OFFSET(i, 0) + VIDEO_MEMORY);
        char *dest = (char *)(OFFSET(i - 1, 0) + VIDEO_MEMORY);
        memcpy(src, dest, 2 * MAX_COLS);
    }
}

void kprintf_at(const char *str, uint8_t y, uint8_t x) {
    uint16_t offset = OFFSET(y, x);
    uint32_t i;

    for (i = 0; str[i] != '\0'; ) {
        offset = kprint_char_at(str[i++], y, x);
        y = POS_Y(offset);
        x = POS_X(offset);
    }
}

void kprint_str(char *str) {
    uint16_t offset = get_cursor_offset();
    uint8_t y = POS_Y(offset);
    uint8_t x = POS_X(offset);
    kprintf_at(str, y, x);
}

void kprintf(const char *str, ...) {
    uint32_t i;
    uint32_t args_count = 0;
    va_list valist;
    char buffer[32];
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
                            kprint_str(buffer);
                            break;
                        case 's':
                            kprint_str(va_arg(valist, char *));
                            break;
                        case 'c':
                            *buffer = va_arg(valist, char);
                            kprint_char(buffer[0]);
                            break;
                        case 'x':
                            value = va_arg(valist, int32_t);
                            int_to_str(buffer, value, 16);
                            kprint_str(buffer);
                            break;
                        /*case 'f':
                            d_value = va_arg(valist, double);
                            double_to_ascii(d_value, buffer, 3);
                            kprint_str(buffer);
                            break;*/
                        default:
                            set_color(ERROR_COLOR);
                            kprint_str("ERROR\n");
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
        kprint_char(str[i]);
    }
}

void set_cursor_offset(uint16_t offset) {
    offset >>= 1;
    outb(REGISTER_SCREEN_CTRL, 14);
    outb(REGISTER_SCREEN_DATA, (uint8_t)(offset >> 8));
    outb(REGISTER_SCREEN_CTRL, 15);
    outb(REGISTER_SCREEN_DATA, (uint8_t)(offset & 0xFF));
}

static uint16_t get_cursor_offset() {
    uint16_t offset;
    outb(REGISTER_SCREEN_CTRL, 14);
    offset = inb(REGISTER_SCREEN_DATA);
    offset <<= 8;
    outb(REGISTER_SCREEN_CTRL, 15);
    offset += inb(REGISTER_SCREEN_DATA);
    return offset << 1;
}