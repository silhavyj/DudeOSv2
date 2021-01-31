[global _uexit]
_uexit:
    mov     eax, 100
    int     0x80
    ret

[global _umalloc]
_umalloc:
    mov     ebx, [esp + 4]
    mov     eax, 103
    int     0x80
    ret

[global _ufree]
_ufree:
    mov     ebx, [esp + 4]
    mov     eax, 104
    int     0x80
    ret

[global _uexec]
_uexec:
    mov     ebx, [esp + 4]
    mov     eax, 105
    int     0x80
    ret

[global _ps]
_ps:
    mov     eax, 107
    int     0x80
    ret