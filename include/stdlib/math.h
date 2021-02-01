#ifndef _MATH_H_
#define _MATH_H_

#include <stdlib/stdint.h>

#define abs(x) ((x) < 0 ? (-x) : (x))
#define max(x, y) ((x) > (y) ? (x) : (y))
#define min(x, y) ((x) < (y) ? (x) : (y))

uint32_t pow(uint32_t a, uint32_t b);

#endif