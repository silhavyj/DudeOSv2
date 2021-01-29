#include "system.h"

int start() {
    void *mem = _umalloc(1024);
    _ufree(mem);

    _uexit();
    return 0;
}