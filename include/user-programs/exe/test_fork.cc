#include <system.h>

int main() {
    int id = _ufork();
    if (id == 0) {
        _uprintf("I'm the child\n");
        if (_ufork() == 0)
            _uprintf("I'm the child of the child\n");
        _uprintf("Hello :)\n");
    }
    else
        _uprintf("I'm the parent\n");
    _uexit();
}