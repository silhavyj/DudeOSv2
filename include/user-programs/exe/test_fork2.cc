#include <system.h>

int main() {
    int *z = (int *)_umalloc(5 * sizeof(int));
    int i;
    for (i = 0; i < 5; i++)
        z[i] = i+1;

    if (_ufork() == 0) {
        z[0] = 20;
        int *x = (int *)_umalloc(sizeof(int));
        _uprintf("child: x=%d\n", x);
        _ufree(x);
    } else {
        z[0] = 12;
        int *y = (int *)_umalloc(sizeof(int));
        _uprintf("parent: y=%d\n", y);
        _ufree(y);
    }
    for (i = 0; i < 5; i++)
        _uprintf("%d\n", z[i]);
    _uexit();
}