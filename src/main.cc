#include <gdt.h>
#include <idt.h>
#include <irq.h>
#include <support.h>
#include <stdlib/string.h>
#include <drivers/screen.h>

void print_os_logo();
void init(void (*init_fce)(), const char *msg);

extern "C" int _kmain() {
    init_screen_driver();
    print_os_logo();

    init(&init_gdt,  "initializing GDT...");
    init(&init_idt,  "initializing IDT...");
    init(&remap_irq, "remaping IRQs...");

    while (1)
    {
    }

    return 0;
}

void init(void (*init_fce)(), const char *msg) {
    uint8_t cursor_pos_y = POS_Y(get_cursor_offset());
    kprintf_at("[      ] ", cursor_pos_y, 0);
    kprintf("%s", msg);
    init_fce();
    set_color(FOREGROUND_GREEN);
    kprintf_at("OK", cursor_pos_y, 3);
    set_cursor_offset(OFFSET(cursor_pos_y + 1, 0));
    reset_color();
}

void print_os_logo() {
    kprintf_at(" _______                   __             ______    ______",       0, 0);
    kprintf_at("/       \\                 /  |           /      \\  /      \\",   1, 0);
    kprintf_at("$$$$$$$  | __    __   ____$$ |  ______  /$$$$$$  |/$$$$$$  |",     2, 0);
    kprintf_at("$$ |  $$ |/  |  /  | /    $$ | /      \\ $$ |  $$ |$$ \\__$$/",    3, 0);
    kprintf_at("$$ |  $$ |$$ |  $$ |/$$$$$$$ |/$$$$$$  |$$ |  $$ |$$      \\",     4, 0);
    kprintf_at("$$ |  $$ |$$ |  $$ |$$ |  $$ |$$    $$ |$$ |  $$ | $$$$$$  |",     5, 0);
    kprintf_at("$$ |__$$ |$$ \\__$$ |$$ \\__$$ |$$$$$$$$/ $$ \\__$$ |/  \\__$$ |", 6, 0);
    kprintf_at("$$    $$/ $$    $$/ $$    $$ |$$       |$$    $$/ $$    $$/",      7, 0);
    kprintf_at("$$$$$$$/   $$$$$$/   $$$$$$$/  $$$$$$$/  $$$$$$/   $$$$$$/",       8, 0);
    kprintf_at("",                                                                 9, 0);
    set_cursor_offset(OFFSET(10, 0));
}