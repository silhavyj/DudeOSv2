#ifndef _SYSTEM_H_
#define _SYSTEM_H_

void _uprintf(const char *format, ...);

extern "C" {
    void _uexit();
    void *_umalloc(unsigned int size);
    void _ufree(void *addr);
    unsigned int _uexec(const char *program);
   // void _uprint(const char *str);
}

#endif