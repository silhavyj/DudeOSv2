[global _ureadln]
_ureadln:
    mov     edi, [esp + 4]
    mov     eax, 102
    int     0x80
    ret