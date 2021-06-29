#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#define USER_SCREEN_BUFF_SIZE 256

void _uprintf(const char *format, ...);

extern "C" {
    void _uexit();
    void *_umalloc(unsigned int size);
    void _ufree(void *addr);
    unsigned int _uexec(const char *program);
    void _ps();
    void _uprint(const char *str);
    void _ureadln(char *dest);
    void _show_programs();
    int _ukill(unsigned int pid);
    int _ufork();
    void _uclear();
    int _upipe(unsigned int id);
    int _upipe_write(unsigned int id, char *buffer, unsigned int bytes);
    int _upipe_read(unsigned int id, char *buffer, unsigned int bytes);
    int _upipe_release(unsigned int id);
}

#endif