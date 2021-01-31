#include "math.h"

uint32_t pow(uint32_t a, uint32_t b) {
    if (b == 0)
		return 1;
	uint32_t u = pow(a, b / 2);
	u = u * u;
	if (b & 1)
		u = (u * a);
	return u;
}