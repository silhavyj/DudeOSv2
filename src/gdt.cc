#include <gdt.h>
#include <support.h>
#include <stdlib/memory.h>

GDT_descriptor_t gdt_desc;
GDT_entry_t gdt[5]; // for now we define fine different segments

void init_gdt() {
    // 0x0 - the first descriptor entry is always NULL
    memset((char *)&gdt[0], 0, sizeof(GDT_entry_t));

    // code 0x8
    gdt[1].limit_low   = 0xFFFF;
    gdt[1].base_low    = 0x0;
    gdt[1].base_middle = 0x0;
    gdt[1].access      = 0x9A;  //10011010b this section has the execution bit set to 1
    gdt[1].granularity = 0xCF;  //11001111b
    gdt[1].base_high   = 0;

    // data 0x10
    gdt[2].limit_low   = 0xFFFF;
    gdt[2].base_low    = 0x0;
    gdt[2].base_middle = 0x0;
    gdt[2].access      = 0x92;  //10010010b
    gdt[2].granularity = 0xCF;  //11001111b
    gdt[2].base_high   = 0;

    // video memory 0x18
    gdt[3].limit_low   = 0xFFFF;
    gdt[3].base_low    = 0x8000;
    gdt[3].base_middle = 0x0B;
    gdt[3].access      = 0x92;  //10010010b
    gdt[3].granularity = 0xCF;  //11001111b
    gdt[3].base_high   = 0;

    // virtual video memory 0x20
    gdt[4].limit_low   = 0xFFFF;
    gdt[4].base_low    = 0x6000;
    gdt[4].base_middle = 0x50;
    gdt[4].access      = 0x92;  //10010010b
    gdt[4].granularity = 0xCF;  //11001111b
    gdt[4].base_high   = 0x6;

    // initialize the gdt descritor
    gdt_desc.size       = sizeof(gdt);
    gdt_desc.start_addr = (uint32_t)&gdt[0];

    // load the gdt dectriptor to the CPU
    _load_gdt((uint32_t)&gdt_desc);
}