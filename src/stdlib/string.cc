#include <stdlib/string.h>
#include <stdlib/memory.h>
#include <stdlib/math.h>
#include <stdlib/limits.h>

static int int_to_ascii_pos(int x, char str[], int pos);

int append(char *str1, const char *str2) {
    uint32_t len1 = strlen(str1);
    uint32_t len2 = strlen(str2);
    uint32_t i;

    for (i = 0; i < len2; i++)
        str1[i + len1] = str2[i];
    
    str1[len1 + len2] = '\0';
    return len1 + len2;
}

uint32_t strlen(const char *str) {
    uint32_t i = 0;
    while (str[i] != 0)
        i++;
    return i;
}

int strcmp(const char *str1, const char *str2) {
    return memcmp(str1, str2, strlen(str1));
}

int strcmp(const char *str1, const char *str2, int n) {
     return memcmp(str1, str2, n);
}

void strcpy(char *dst, const char *src) {
    uint32_t len = strlen(src);
    memcpy(dst, src, len);
    dst[len] = '\0';
}

void reverse(char *str) {
    uint32_t len = strlen(str);
    uint32_t middle = len / 2;
    uint32_t i;

    for (i = 0; i < middle; i++) {
        str[i] = str[i] ^ str[len - i - 1];
        str[len - i - 1] = str[i] ^ str[len - i - 1];
        str[i] = str[i] ^ str[len - i - 1];
    }
}

void append(char *str, char c) {
    uint32_t len = strlen(str);
    str[len] = c;
    str[len + 1] = '\0';
}

static int int_to_ascii_pos(int num, char str[], int pos) {
    int_to_str(str + pos, num, 10);
    return strlen(str);
}

void int_to_str(char *str, int num, int base) {
    switch (base) {
        case 2:
            int_to_str_02(str, num);
            break;
        case 16:
            int_to_str_16(str, num);
            break;
        case 10:
        default:
            int_to_str_10(str, num);
            break;
    }
}

void int_to_str_10(char *str, int num) {
    uint32_t index = 0;
    uint8_t sign = num < 0;
    uint32_t y = (uint32_t)abs(num);

    do {
        str[index++] = y % 10 + '0';
    } while ((y /= 10) > 0);
    
    if (sign == 1)
        str[index++] = '-';
    str[index] = '\0';
    reverse(str);
}

void int_to_str_16(char *str, int num) {
    uint8_t sign = num < 0;
    uint32_t index = 0;
    uint32_t remainder;
    uint32_t y = (uint32_t)abs(num);

    do {
        remainder = y % 16;
        if (remainder >= 10)
            str[index++] = 'A' + (remainder - 10);
        else str[index++] = remainder + '0';
    } while ((y /= 16) > 0);
    
    append(str, 'x');
    append(str, '0');

    index += 2;
    if (sign == 1)
        str[index++] = '-';
    str[index] = '\0';
    reverse(str);
}

void int_to_str_02(char *str, int num) {
    uint32_t index = 0;
    uint8_t sign = num < 0;
    uint32_t y = (uint32_t)abs(num);

    do {
        str[index++] = y % 2 + '0';
    } while ((y /= 2) > 0);
    
    if (sign == 1)
        str[index++] = '-';
    str[index] = '\0';
    reverse(str);
}

int atoi(const char *str) {
    uint8_t sign = 0;
    int result = 0;
    int32_t i = find_first_not_of(str, ' ');

    if (i == -1)
        return 0;
    if (str[i] == '-') {
        sign = 1;
        i++;
    } else if (sign == '+') {
        i++;
    } else if (str[i] < '0' || str[i] > '9') {
        return 0;
    }
    while (str[i] != '\0') {
        if (str[i] >= '0' && str[i] <= '9') {
            if (result < INT_MAX / 10 || (result == INT_MAX / 10 && (str[i] - '0') % 10 <= INT_MAX % 10))
                result = 10 * result + (str[i] - '0');
            else {
                return sign == 1 ? INT_MIN : INT_MAX;
            }
        } else {
            break;
        }
        i++;
    }
    return sign == 1 ? -result : result;
}

int find_first_not_of(const char *str, char c) {
    uint32_t index = 0;
    while (str[index] != '\0') {
        if (str[index] != c)
            return index;
        index++;
    }
    return -1;
}

void double_to_ascii(double num, char *str, uint8_t afterpoint) {
    uint8_t sign = num < 0;
    if (sign == 1)
        str[0] = '-';
    num = abs(num);

    uint32_t integer_part = (uint32_t)num;
    double float_part = num - (double)integer_part;

    uint32_t i = int_to_ascii_pos(integer_part, str, sign);
    if (afterpoint != 0) {
        append(str, '.');
        float_part = float_part * pow(10, afterpoint); 
        int_to_ascii_pos((uint32_t)float_part, str, i + 1); 
    }
}

void va_stringf(char *str, const char* format, va_list valist) {
    uint32_t i;
    char buffer[128];
    double d_value;
    int value;

    uint32_t len = strlen(format);

     for (i = 0; i < len; i++) {
        if (format[i] == '%') {
            if (i < len - 1) {
                if (format[i+1] != '%') {
                    memset(buffer, '\0', 32);
                    switch (format[i+1]) {
                        case 'd':
                            value = va_arg(valist, int32_t);
                            int_to_str(buffer, value, 10);
                            break;
                        case 's':
                            strcpy(buffer, va_arg(valist, char *));
                            break;
                        case 'c':
                            buffer[0] = va_arg(valist, int);
                            buffer[1] = '\0';
                            break;
                        case 'x':
                            value = va_arg(valist, int32_t);
                            int_to_str(buffer, value, 16);
                            break;
                        case 'f':
                            d_value = va_arg(valist, double);
                            double_to_ascii(d_value, buffer, 3);
                            break;
                        default:
                            // TODO fix this
                            break;
                    }
                    append(str, buffer);
                    i++;
                    continue;
                } else {
                    i++;
                }
            }
        }
        buffer[0] = format[i];
        buffer[1] = '\0';
        append(str, buffer);
    }
}