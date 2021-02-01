#include <stdlib/memory.h>

void memcpy(char *dst, const char* src, uint32_t n) {
    uint32_t i;
    for (i = 0; i < n; i++)
        dst[i] = src[i];
}

void memset(char *dst, unsigned char value, uint32_t n) {
    uint32_t i;
    for (i = 0; i < n; i++)
        dst[i] = value;
}

int memcmp(const char *p1, const char *p2, uint32_t size) {
    uint32_t i;
    for (i = 0; i < size; i++)
        if (p1[i] != p2[i])
            return 1;
    return 0;
}