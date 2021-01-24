;----------------------------------------------
; definition of the global description table 
; used for 'separating' important parts of the os
; within RAM - see https://wiki.osdev.org/Global_Descriptor_Table.
;----------------------------------------------
gdt_start:                      ; the start point of gdt (used when calculating - see gdt descritptor)
;----------------------------------------------
gdt_null:                       ; the first entry must be always NULL - 8 bytes
    dd 0                        ; 4 bytes
    dd 0                        ; 4 bytes
;----------------------------------------------
gdt_code:
    dw 0xFFFF                   ; length of the segment bits 0-15
    dw 0x0000                   ; segment base bits 0-15         
    db 0x00                     ; segment base bits 16-23
    db 10011010b                ; flags (8 bits)
    db 11001111b                ; flags (4 bits) + segment length bits 16-19
    db 0x00                     ; segment base bit 24-31
;----------------------------------------------
gdt_data:
    dw 0xFFFF                   ; length of the segment bits 0-15 
    dw 0x0000                   ; segment base bits 0-15
    db 0x00                     ; segment base bits 16-23
    db 10010010b                ; flags (8 bits)
    db 11001111b                ; flags (4 bits) + segment length bits 16-19
    db 0x00                     ; segment base bit 24-31
;----------------------------------------------
gdt_vram:
    dw 0xFFFF                   ; length of the segment bits 0-15
    dw 0x8000                   ; segment base bits 0-15
    db 0x0B                     ; segment base bits 16-23
    db 10010010b                ; flags (8 bits)
    db 11001111b                ; flags (4 bits) + segment length bits 16-19
    db 0x00                     ; segment base bit 24-31
;----------------------------------------------
gdt_end:                        ; just so we have a pointer to the end of gdt
;----------------------------------------------
gdt_desc:
    dw gdt_end - gdt_start - 1  ; size of the whole gdt (limit)
    dd gdt_start                ; start addr of gdt