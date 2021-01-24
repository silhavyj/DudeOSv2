DISK_ERROR_MSG:    db "Disk read error ", 0
;----------------------------------------------
dapack:
            db 0x10
            db 0
.count      dw 0                     ; int 13 resets this to number of blocks actually read/written 
.buff       dw 0                     ; memory buffer destination address
.seg        dw 0                     ; in memory page 0
.addr       dq 1                     ; skip the 1st sector whihc is the bootloader - loaded by BIOS 
;----------------------------------------------
load_kernel:
    pusha

    mov     ax, 127                  ; ax = 127 
    mov     [dapack.count], ax       ; dapcak.count = ax
    mov     ax, [KERNEL_SOURCE_CODE] ; ax = KERNAL_SOURCE_CODE 
    mov     [dapack.buff], ax        ; dapack.buff = ax

    mov     dl, [BOOT_DISK]          ; dl = *BOOT_DISK
    mov     si, dapack               ; si = dapack
    mov     ah, 0x42                 ; 0x42 = extended read sectors 
    int     0x13                     ; 0x13 = low level disk services - https://en.wikipedia.org/wiki/BIOS_interrupt_call

    jc      disk_error               ; if error (stored in the carry bit)

    popa                             ; pop registers all from the stack
    ret                              ; return
;----------------------------------------------
disk_error:
    mov     bx, DISK_ERROR_MSG       ; print out the DISK_ERROR message 
    call    print_string
    mov     dh, ah                   ; ah = error code, dl = disk drive that dropped the error
    call    print_hex                ; print the error value
    jmp     $                        ; we're done here :(
;----------------------------------------------