;---------------------------------------
; terminates the current process - exit
; stack: [esp    ] return address
;---------------------------------------
[global _uexit]
_uexit:
    mov     eax, 100        ; 100 = system call number (exit)
    int     0x80            ; call the interrupt (0x80 = system calls)
    ret                     ; return
;---------------------------------------

;---------------------------------------
; dynamic allocation for the current
; process - malloc
; pointer to the allocate piece of memory
; in EAX register (return value)
; stack: [esp + 4] number of bytes to allocate
;        [esp    ] return address
;---------------------------------------
[global _umalloc]
_umalloc:
    mov     ebx, [esp + 4]  ; ebx = number of bytes to be allocated
    mov     eax, 103        ; 103 = system call number (malloc)
    int     0x80            ; call the interrupt (0x80 = system calls)
    ret                     ; return
;---------------------------------------

;---------------------------------------
; frees the memory given by an address
; as the parameter - free
; stack: [esp + 4] address to the memory
;        [esp    ] return address
;---------------------------------------
[global _ufree]
_ufree:
    mov     ebx, [esp + 4]  ; ebx = address to the mem we want to free
    mov     eax, 104        ; 104 = system call number (free)
    int     0x80            ; call the interrupt (0x80 = system calls)
    ret                     ; return
;---------------------------------------

;---------------------------------------
; exectues the program given as a
; parameter. The function returns PID of
; the new process for further validation
; (0 = error)
; stack: [esp + 4] name of the program to be executed
;        [esp    ] return address
;---------------------------------------
[global _uexec]
_uexec:
    mov     ebx, [esp + 4]  ; ebx = name of the program to be exectued
    mov     eax, 105        ; 105 = system call number (exec)
    int     0x80            ; call the interrupt (0x80 = system calls)
    ret                     ; return
;---------------------------------------

[global _ps]
_ps:
    mov     eax, 107
    int     0x80
    ret

[global _uprint]
_uprint:
    mov     esi, [esp + 4]
    mov     eax, 101
    int     0x80
    ret

[global _ureadln]
_ureadln:
    mov     edi, [esp + 4]
    mov     eax, 102
    int     0x80
    ret

[global _show_programs]
_show_programs:
    mov     eax, 108
    int     0x80
    ret

[global _ukill]
_ukill:
    mov     ebx, [esp + 4]
    mov     eax, 106
    int     0x80
    ret

[global _ufork]
_ufork:
    mov     eax, 109
    int     0x80
    ret