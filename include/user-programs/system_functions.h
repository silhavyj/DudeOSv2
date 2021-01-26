#ifndef _SYSTEM_FUNCTIONS_H_
#define _SYSTEM_FUNCTIONS_H_

extern "C" {
    //void _uprintf(const char *str, ...);
    void _ureadln(char *dest);
    void _uexit();
    void *_umalloc(unsigned int size);
    void _ufree(void *addr);
    unsigned int _uexec(char *program);
    void _ukill(unsigned int pid);
    void _uprint_processes();
}

#endif