#ifndef _FAT_FS_H_
#define _FAT_FS_H_

#include <stdlib/stdint.h>

#define FAT_TYPE 12
#define MAX_FAT_VALUE (1 << FAT_TYPE)
#define CLUSTER_SIZE 512

typedef struct {
    uint32_t val : FAT_TYPE;
} __attribute__((packed)) fat_table_entry_t;

typedef struct {
    uint32_t fat_table_addr;
    uint32_t clusters_addr;
    uint32_t cluster_size;
    uint32_t cluster_count;
} __attribute__((packed)) superblock_t;

void init_fat_fs(uint32_t size);

#endif