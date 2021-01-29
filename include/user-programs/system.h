#ifndef _SYSTEM_H_
#define _SYSTEM_H_

extern "C" {
    void _uexit();
    void *_umalloc(unsigned int size);
    void _ufree(void *addr);
}

#endif