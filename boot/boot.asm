[bits 16]

org     0x7C00                          ; BIOS places our code at this address
                                        ; so we define the offset here
jmp     start                           ; jump to the main procedure
;----------------------------------------------
STACK_SIZE           equ 0x700          ; stack 512B
BOOT_DISK            db  0              ; memory location to store the disk number
KERNEL_START_ADDR    dd  0x6400000      ; kernel start address in the memory
KERNAL_STACK_PTR     dd  0x6504FFF      ; kernel stack pointer
KERNEL_SOURCE_CODE   dd  0x7E00         ; kernel source code start
;----------------------------------------------
WELCOME_MSG:         db "Welcome to DudeOSv2 bootloader", 0
;----------------------------------------------
%include "print.asm"                    ; include module for printing
%include "A20.asm"                      ; include module for enabling A20
%include "gdt.asm"                      ; include module with gdt 
%include "kernel_loader.asm"            ; include module for loading kernel
;----------------------------------------------
start:
    xor     ax, ax                      ; ax = 0
    mov     es, ax                      ; clear up the segment registers
    mov     ds, ax                      ; all of them will be set to 0
    mov     ss, ax
    mov     sp, STACK_SIZE              ; initialize the stack pointer

    mov     [BOOT_DISK], dl             ; BIOS automatically puts the disk number into dl
                                        ; so we store it into our 'variable' for later use

    mov     bx, WELCOME_MSG             ; store the WELCOME_MSG into register si (parameter for boot_print)
    call    println_string              ; call the print procedure
    
    call    a20_enable                  ; enable a20 line - https://wiki.osdev.org/A20_Line
    call    load_kernel                 ; load the kernel

    cli                                 ; diable interrupts (it'll be neabled later on
                                        ; by the first process put on the CPU)
    lgdt    [gdt_desc]                  ; load the GDT desctiptor
    mov     eax, cr0                    ; unfortuantely we can't access the cr0 register directly
    or      eax, 1                      ; switch to protected mode
    mov     cr0, eax                    ; update the cr0 register

    mov     ax, 0x10                    ; ax = 0x10 (kernel data segment)
    mov     ds, ax                      ; set up data segment registers after gdt desc has been loaded
    mov     es, ax
    mov     gs, ax
    mov     fs, ax
    mov     ss, ax

    mov     esp, [KERNAL_STACK_PTR]     ; esp = *KERNAL_STACK_PTR
    jmp     0x08:protected_mode_start   ; perform far jump to "fix" CS (code segment register)
;----------------------------------------------

; protected mode must be included down here becuse
; it's a different mode (32 bits) than the rest of the code
%include "protected_mode.asm"

;----------------------------------------------
; the last two bytes of a bootloader have to be
; the magic number 0x55AA, so BIOS knows that
; this is a bootloader. The total size is 512B,
; so we need to fill the remaining space with 0
;----------------------------------------------
times   510-($-$$) db 0                 ; 510-'size of the code' times put there 0
                                        ; $ represents the beginning of the code
                                        ; and $$ the end of it
db      0x55                            ; first byte of the magic number 0x55
db      0xAA                            ; second byte of the magic number 0xAA
;----------------------------------------------