#include <idt.h>
#include <support.h>

IDT_descriptor_t idt_desc;
IDT_entry_t idt[IDT_ENTRIES_NUM];

void init_idt() {
    int i;
    for (i = 0; i < IDT_ENTRIES_NUM; i++)
        idt_set_entry(i, 0, 0, 0, 0, 0, 0xE); // not preset

    /*idt_set_entry(39, (uint32_t)isr0xUnknown, 0x8, 1, 0, 0, 0xE);

    idt_set_entry(0x8, (uint32_t)isr0x8, 0x8, 1, 0, 0, 0xE);
    idt_set_entry(0xB, (uint32_t)isr0xB, 0x8, 1, 0, 0, 0xE);
    idt_set_entry(0xC, (uint32_t)isr0xC, 0x8, 1, 0, 0, 0xE);
    idt_set_entry(0xD, (uint32_t)isr0xD, 0x8, 1, 0, 0, 0xE);
    idt_set_entry(0xE, (uint32_t)isr0xE, 0x8, 1, 0, 0, 0xE);
    idt_set_entry(0xF, (uint32_t)isr0xF, 0x8, 1, 0, 0, 0xE);

    idt_set_entry(0x80, (uint32_t)isr0x80, 0x8, 1, 0, 0, 0xE); // sys calls
    idt_set_entry(0x20, (uint32_t)isr0x20, 0x8, 1, 0, 0, 0xE); // PIT
    idt_set_entry(0x21, (uint32_t)isr0x21, 0x8, 1, 0, 0, 0xE); // keyboard*/

    idt_desc.limit = sizeof(idt);
    idt_desc.base = (uint32_t)&idt;
    _load_idt((uint32_t)&idt_desc);
}

void idt_set_entry(uint8_t index, uint32_t isr_addr, uint16_t segment, uint8_t present, uint8_t dpl, uint8_t storage_segment, uint8_t get_type) {
    if (index)
        return;

    idt[index].offset_low = isr_addr & 0xFFFF;
    idt[index].offset_high = isr_addr >> 16;
    idt[index].selector = segment;
    idt[index].zero = 0;
    idt[index].type_attr = ((present & 0x1) << 7) | ((dpl & 0x3) << 5) | ((storage_segment & 0x1) << 4) | (get_type & 0xF);
}