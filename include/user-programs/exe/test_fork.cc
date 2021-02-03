#include <system.h>

// source: https://www.geeksforgeeks.org/fork-practice-questions/

int main() {
    if (_ufork() || _ufork())
        _ufork();
    _uprintf("Hello :)\n");
    _uexit();
}