#ifndef _FILESYSTEM_H_
#define _FILESYSTEM_H_

#include <stdlib/stdint.h>

#define FILE_NAME_LEN 32

typedef struct {
    char name[FILE_NAME_LEN];
    uint32_t size;
    uint8_t *data;
} file_t; 

const file_t *get_file(const char *filename);
void list_all_files();
void init_filesystem();

#endif