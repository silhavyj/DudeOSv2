#include <system.h>

int main() {
    if (_ufork() == 0)
        _uprintf("I'm the child\n"); 
    else
        _uprintf("I'm the parent\n");
    _uexit();
} 