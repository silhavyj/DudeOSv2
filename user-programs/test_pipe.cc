#include <system.h>

int main() {
    int id = _upipe(1);
    _uprintf("%d\n", id);
    id = _upipe(1);
    _uprintf("%d\n", id);
    id = _upipe(1);
    _uprintf("%d\n", id);
    _uexit();
}