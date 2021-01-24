#include <stdlib/string.h>
#include <stdlib/memory.h>
#include <stdlib/math.h>
#include <stdlib/limits.h>

uint32_t strlen(const char *str) {
    uint32_t i = 0;
    while (str[i] != 0)
        i++;
    return i;
}

int strcmp(const char *str1, const char *str2) {
    return memcmp(str1, str2, strlen(str1));
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