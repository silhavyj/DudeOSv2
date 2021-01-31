#include "system.h"

int main() {
    char *p = (char *)0xFFFFFFF;
    *p = 'A';
    _uexit();
}