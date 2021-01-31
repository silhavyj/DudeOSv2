#include "system.h"
#include "../src/stdlib/string.h"
#include "../src/stdlib/stdarg.h"
#include "../src/stdlib/stdint.h"
#include "../src/stdlib/memory.h"

char user_screen_buffer[USER_SCREEN_BUFF_SIZE];

void _uprintf(const char *format, ...) {
    uint32_t i;
    uint32_t args_count = 0;
    va_list valist;
    uint32_t len = strlen(format);

    for (i = 0; i < len; i++)
        if (format[i] == '%') {
            if (i < len - 1 && format[i + 1] != '%')
                args_count++;
        }

    va_start(valist, args_count);
    memset(user_screen_buffer, '\0', USER_SCREEN_BUFF_SIZE);
    va_stringf(user_screen_buffer, format, valist);
    _uprint(user_screen_buffer);
}