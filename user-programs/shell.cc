#include "system.h"

int main() {
    // void *mem = _umalloc(1024);
    // _ufree(mem);
    
    _uexec("dummy.exe");
   
    while (1)
    {
    }

    _uexit();
}