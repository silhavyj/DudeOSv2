#ifndef _APP_H_
#define _APP_H_

#include <stdlib/stdint.h>

#define FILE_NAME_LEN 32

typedef struct {
    char name[FILE_NAME_LEN];
    uint32_t size;
    uint8_t *data;
} program_t; 

const program_t *get_program(const char *filename);
void list_all_programs();
void init_programs_list();

#endif