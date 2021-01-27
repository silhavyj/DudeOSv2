int start() {
    //while (1);
    asm(
        "mov %0, %%eax;"
        "int $0x80;"
        : : "r" (100)
        : "eax"
    );
    return 0;
}