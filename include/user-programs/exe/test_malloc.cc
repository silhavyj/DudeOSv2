#include <system.h>

void test();

int main() {
    _uprintf("Test of array allocation!\n");
    test();
    test();
    _uexit();
}

void test() {
    int i;
    int *arr;
    arr = (int *)_umalloc(10 * sizeof(int));
    for (i = 0; i < 10; i++) {
        arr[i] = i;
        _uprintf("arr[i]=%d &arr[i]=%x\n", arr[i], &arr[i]);
    }
    _ufree(arr);
}