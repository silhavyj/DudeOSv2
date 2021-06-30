#include <system.h>
#include <stdlib/string.h>
#include <stdlib/stdarg.h>
#include <stdlib/stdint.h>
#include <stdlib/memory.h>

char user_screen_buffer[USER_SCREEN_BUFF_SIZE];

void _uprintf(const char *format, ...) {
    va_list valist;
    va_start(valist, format);
    memset(user_screen_buffer, '\0', USER_SCREEN_BUFF_SIZE);
    va_stringf(user_screen_buffer, format, valist);
    _uprint(user_screen_buffer);
}