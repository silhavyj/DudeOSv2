#ifndef _MEMORY_H_
#define _MEMORY_H_

#include "stdint.h"

void memcpy(char *dst, const char* src, uint32_t n);
void memset(char *dst, unsigned char value, uint32_t n);
int memcmp(const char *p1, const char *p2, uint32_t size);

#endif