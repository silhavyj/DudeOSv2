;----------------------------------------------
; We're now in a 32 bit so all we now should do
; is to call the kernel entry function 
;----------------------------------------------
[bits 32]
protected_mode_start:
    mov     edi, [KERNEL_START_ADDR]    ; update the data segment
    mov     esi, [KERNEL_SOURCE_CODE]   ; update the code segment
    mov     ecx, 0xFE00                 ; 127 * 512B
    rep     movsb                       ; copy ds:esi to es:edi ecx times

    cli                                 ; diable interrupts
                                        ; it'll be enabled by the first process
                                        ; assigned to the CPU

    jmp     [KERNEL_START_ADDR]         ; jmp to the kernel entry function
    jmp $                               ; an infinit loop (the code should never get here though
                                        ; as the kernel has its own infinit loop but just in case)
;----------------------------------------------