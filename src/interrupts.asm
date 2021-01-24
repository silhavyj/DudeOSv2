;---------------------------------------
; double fault
;---------------------------------------
[extern _int0x8_handler]
[global _isr0x8]
_isr0x8:
    cli                     ; don't let anyone interrupt us (disable interrupts) 
    pusha                   ; push all registers on the stack - we don't want to overwrite them

    mov     ax, ds
    push    eax
    mov     ax, 0x10        ; data segment
    mov     ds, ax
    mov     es, ax
    mov     gs, ax
    mov     fs, ax
    mov     ss, ax
    
    call    _int0x8_handler ; call the interrupt handler in C/C++
    
    pop     eax             ; pop the data segment off the stack
    mov     ds, ax          ; and set the state as it was before
    mov     es, ax
    mov     fs, ax
    mov     gs, ax
    mov     ss, ax

    popa                    ; pop all registers back off the stack  
    sti                     ; once we're done enable interrupts
    iret                    ; return
;---------------------------------------
; segment not present
;---------------------------------------
[extern _int0xB_handler]
[global _isr0xB]
_isr0xB:
    cli                     ; don't let anyone interrupt us (disable interrupts) 
    pusha                   ; push all registers on the stack - we don't want to overwrite them

    mov     ax, 0x10        ; data segment
    mov     ds, ax
    mov     es, ax
    mov     gs, ax
    mov     fs, ax
    mov     ss, ax
    
    call    _int0xB_handler ; call the interrupt handler in C/C++
    
    mov     ax, 0x10
    mov     ds, ax
    mov     es, ax
    mov     fs, ax
    mov     gs, ax
    mov     ss, ax

    popa                    ; pop all registers back off the stack  
    sti                     ; once we're done enable interrupts
    iret                    ; return
;---------------------------------------
; stack fault
;---------------------------------------
[extern _int0xC_handler]
[global _isr0xC]
_isr0xC:
    cli                      ; don't let anyone interrupt us (disable interrupts) 
    pusha                    ; push all registers on the stack - we don't want to overwrite them

    mov     ax, ds  
    push    eax              ; save the data segment onto the stack
    mov     ax, 0x10         ; data segment
    mov     ds, ax          
    mov     es, ax
    mov     gs, ax
    mov     fs, ax
    mov     ss, ax
    
    call    _int0xC_handler  ; call the interrupt handler in C/C++
    
    pop     eax              ; pop the data segment off the stack
    mov     ds, ax           ; and set the state as it was before
    mov     es, ax
    mov     fs, ax
    mov     gs, ax
    mov     ss, ax

    popa                     ; pop all registers back off the stack  
    sti                      ; once we're done enable interrupts
    iret                     ; return
;---------------------------------------
;  general protection fault
;---------------------------------------
[extern _int0xD_handler]
[global _isr0xD]
_isr0xD:
    cli                      ; don't let anyone interrupt us (disable interrupts) 
    pusha                    ; push all registers on the stack - we don't want to overwrite them

    mov     ax, ds
    push    eax              ; save the data segment onto the stack
    mov     ax, 0x10         ; data segment
    mov     ds, ax
    mov     es, ax
    mov     gs, ax
    mov     fs, ax
    mov     ss, ax
    
    call    _int0xD_handler  ; call the interrupt handler in C/C++

    pop     eax              ; pop the data segment off the stack
    mov     ds, ax           ; and set the state as it was before
    mov     es, ax
    mov     fs, ax
    mov     gs, ax
    mov     ss, ax

    popa                     ; pop all registers back off the stack  
    sti                      ; once we're done enable interrupts
    iret                     ; return
;---------------------------------------
; page fault pushes (retursn) error code
;---------------------------------------
[extern _int0xE_handler]
[global _isr0xE]
_isr0xE:
    cli                      ; don't let anyone interrupt us (disable interrupts) 
    mov     ebp, esp
    pusha                    ; push all registers on the stack - we don't want to overwrite them   

    mov     ax, ds
    push    eax              ; save the data segment onto the stack
    mov     ax, 0x10         ; data segment
    mov     ds, ax
    mov     es, ax
    mov     gs, ax
    mov     fs, ax
    mov     ss, ax
    
    mov     eax, cr2         ; eax = address of the page fault
    push    eax              ; store it onto the stack
    call    _int0xE_handler  ; call the interrupt handler in C/C++
    
    pop     eax              ; pop the data segment off the stack
    mov     ds, ax           ; and set the state as it was before
    mov     es, ax
    mov     fs, ax
    mov     gs, ax
    mov     ss, ax

    popa                     ; pop all registers back off the stack  
    sti                      ; once we're done enable interrupts
    iret                     ; return
;---------------------------------------
; unknown interrupt
;---------------------------------------
[extern _int0xF_handler]
[global _isr0xF]
_isr0xF:
    cli                      ; don't let anyone interrupt us (disable interrupts)
    pusha                    ; push all registers on the stack - we don't want to overwrite them

    mov     ax, ds
    push    eax              ; save the data segment onto the stack
    mov     ax, 0x10         ; data segment
    mov     ds, ax
    mov     es, ax
    mov     gs, ax
    mov     fs, ax
    mov     ss, ax
    
    call    _int0xF_handler  ; call the interrupt handler in C/C++
    
    pop     eax              ; pop the data segment off the stack
    mov     ds, ax           ; and set the state as it was before
    mov     es, ax
    mov     fs, ax
    mov     gs, ax
    mov     ss, ax

    popa                     ; pop all registers back off the stack
    sti                      ; once we're done enable interrupts
    iret                     ; return
;---------------------------------------
; system calls interrupt handler
; this handler is used when a user process
; wants some service from the kernel
; (open a file, fork, exit, printf, ...)
;---------------------------------------
[extern _int0x80_handler]
[global _isr0x80]
_isr0x80:
    cli                      ; don't let anyone interrupt us (disable interrupts)
    pusha                    ; push all registers on the stack - we don't want to overwrite them

    mov     ax, 0x10         ; data segment
    mov     ds, ax
    mov     es, ax
    mov     gs, ax
    mov     fs, ax
    mov     ss, ax
    
    call _int0x80_handler    ; call the interrupt handler in C/C++
    
    mov     eax, 0x10
    mov     ss, ax
    mov     ds, ax
    mov     es, ax
    mov     fs, ax
    mov     gs, ax

    popa                     ; pop all registers back off the stack
    sti                      ; once we're done enable interrupts
    iret                     ; return
;---------------------------------------
; keyboard handler - it is called
; everytime the user presses/releases a key
;---------------------------------------
[extern _int0x21_handler]
[global _isr0x21]
_isr0x21:
    cli                      ; don't let anyone interrupt us (disable interrupts)
    pusha                    ; push all registers on the stack - we don't want to overwrite them

    mov     ax, ds
    mov     ax, 0x10         ; data segment
    mov     ds, ax
    mov     es, ax
    mov     gs, ax
    mov     fs, ax
    mov     ss, ax
    
    call    _int0x21_handler ; call the interrupt handler in C/C++
    
    mov     eax, 0x10
    mov     ds, ax
    mov     es, ax
    mov     fs, ax
    mov     gs, ax
    mov     ss, ax

    popa                     ; pop all registers back off the stack
    sti                      ; once we're done enable interrupts
    iret                     ; return
;---------------------------------------
; PIT (system timer) handler
; this handler is essential for preemptive
; process scheduling 
;---------------------------------------
[extern _int0x20_handler]
[global _isr0x20]
_isr0x20:
    cli                      ; don't let anyone interrupt us (disable interrupts)
    pusha                    ; push all registers on the stack - we don't want to overwrite them

    mov     ax, ds
    mov     ax, 0x10         ; data segment
    mov     ds, ax
    mov     es, ax
    mov     gs, ax
    mov     fs, ax
    mov     ss, ax
    
    call    _int0x20_handler ; call the interrupt handler in C/C++
    
    mov     eax, 0x10
    mov     ds, ax
    mov     es, ax
    mov     fs, ax
    mov     gs, ax
    mov     ss, ax

    popa                     ; pop all registers back off the stack
    sti                      ; once we're done enable interrupts
    iret                     ; return
;---------------------------------------