#include "system.h"

int start() {
   // void *mem = _umalloc(1024);
   // _ufree(mem);

    _uexec("error.exe");

    while (1)
    {
    }

    _uexit();
    return 0;
}