[extern _int0xUnknown_handler]
[global _isr0xUnknown]
_isr0xUnknown:
    cli  
    pusha

    mov     ax, 0x10
    mov     ds, ax
    mov     es, ax
    mov     gs, ax
    mov     fs, ax
    mov     ss, ax
    
    call    _int0xUnknown_handler
    
    mov     eax, 0x10
    mov     ss, ax
    mov     ds, ax
    mov     es, ax
    mov     fs, ax
    mov     gs, ax
    popa
    iret 
    ret

[global _isr0x8]
_isr0x8:
    ret

[global _isr0xB]
_isr0xB:
    ret

[global _isr0xC]
_isr0xC:
    ret

[global _isr0xD]
_isr0xD:
    ret

[global _isr0xE]
_isr0xE:
    ret

[global _isr0xF]
_isr0xF:
    ret

[global _isr0x80]
_isr0x80:
    ret

[global _isr0x21]
_isr0x21:
    ret

[global _isr0x20]
_isr0x20:
    ret