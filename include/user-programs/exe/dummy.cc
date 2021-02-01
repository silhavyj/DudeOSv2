#include <system.h>

int main() {
    _uprintf("Test of array allocation!\n");
    int i;
    int *arr = (int *)_umalloc(10 * sizeof(int));
    for (i = 0; i < 10; i++) {
        arr[i] = i;
        _uprintf("%d %x\n", arr[i], &arr[i]);
    }
    _ufree(arr);
    
    _uexit();
}