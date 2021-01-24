#include <gdt.h>
#include <idt.h>
#include <irq.h>
#include <support.h>
#include <stdlib/string.h>
#include <drivers/screen.h>

void print_os_logo();
void init(void (*init_fce)(), const char *msg);

extern "C" int _kmain() {
    clear_screen();

    print_os_logo();

    init(&init_gdt,  "initializing GDT...");
    init(&init_idt,  "initializing IDT...");
    init(&remap_irq, "remaping IRQs...");

    asm ("int $39");

    while (1)
    {
    }

    return 0;
}

void init(void (*init_fce)(), const char *msg) {
    uint16_t cursor_pos = get_cursor_offset();
    kprintf("[      ] %s", msg);
    init_fce();
    set_color(FOREGROUND_GREEN);
    cursor_pos += 3 * BYTES_PER_SYMBOL;
    set_cursor_offset(cursor_pos);
    kprintf("OK\n");
    reset_color();
}

void print_os_logo() {
    set_color(FOREGROUND_DARKGRAY);
    kprintf(" _______                   __             ______    ______\n");
    kprintf("/       \\                 /  |           /      \\  /      \\\n");
    kprintf("$$$$$$$  | __    __   ____$$ |  ______  /$$$$$$  |/$$$$$$  |\n");
    kprintf("$$ |  $$ |/  |  /  | /    $$ | /      \\ $$ |  $$ |$$ \\__$$/\n");
    kprintf("$$ |  $$ |$$ |  $$ |/$$$$$$$ |/$$$$$$  |$$ |  $$ |$$      \\\n");
    kprintf("$$ |  $$ |$$ |  $$ |$$ |  $$ |$$    $$ |$$ |  $$ | $$$$$$  |\n");
    kprintf("$$ |__$$ |$$ \\__$$ |$$ \\__$$ |$$$$$$$$/ $$ \\__$$ |/  \\__$$ |\n");
    kprintf("$$    $$/ $$    $$/ $$    $$ |$$       |$$    $$/ $$    $$/\n");
    kprintf("$$$$$$$/   $$$$$$/   $$$$$$$/  $$$$$$$/  $$$$$$/   $$$$$$/\n");
    kprintf("\n");
    reset_color();
}