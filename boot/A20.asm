;----------------------------------------------
; Enable A20 as described at https://wiki.osdev.org/A20_Line.
; It uses the PS/2 controller to enable it.
; There are two main ports of this controller:
;
; 0x64 - command port used for sending commands to keyboard constroller (PS/2). 
; 0x60 - data port used for sending data to/from PS/2(Keyboard) 
;        controller or the PS/2 device itself. 
;----------------------------------------------
a20_enable:
    pusha                        ; push all registers on the stack - we don't want to overwrite them
    cli                          ; disable interrups (do not let anyone interrupt us while we're enabling a20 line)

    call    a20_wait_input       ; wait for the input buffer of PS/2 controller to be ready
    mov     al, 0xAD             ; 0xAD = disable keyboard
    out     0x64, al             ; send the command to the controller

    call    a20_wait_input       ; wait for the input buffer of PS/2 controller to be ready
    mov     al, 0xD0             ; 0xD0 = copy the command byte into 0x60 register
    out     0x64, al             ; send the command to the controller

    call    a20_wait_output      ; wait for the output buffer of PS/2 controller to be ready
    in      al, 0x60             ; read data the command byte off the output buffer
    push    ax                   ; store it onto the stack for later use

    call    a20_wait_input       ; wait for the input buffer of PS/2 controller to be ready
    mov     al, 0xD1             ; 0xD1 = next written port 0x60 is written to 0x64
    out     0x64, al             ; send the command to the controller

    call    a20_wait_input       ; wait for the input buffer of PS/2 controller to be ready
    pop     ax                   ; pop the ax value we stored before
    or      al, 2                ; enable a20
    out     0x60, al             ; send the data to port 0x60

    call    a20_wait_input       ; wait for the input buffer of PS/2 controller to be ready
    mov     al, 0xAE             ; 0xAE = enable keyboard
    out     0x64, al             ; send the command to the controller

    call    a20_wait_input       ; wait for the input buffer of PS/2 controller to be ready
    sti                          ; enable interrupts once we're done
    popa                         ; pop all registers back off the stack
    ret                          ; return
;----------------------------------------------

;----------------------------------------------
; Waits for the input buffer of PS/2 controller
; to be free - see https://wiki.osdev.org/A20_Line.
; We need to make sure that it's ready to recieve
; another data before we do so.
;
; port 0x64 (command port) is used for sending
; commands to keyboard controller (PS/2). 
;----------------------------------------------
a20_wait_input:
    pusha                            ; store all registers to the stack
a20_wait_input_loop:
    in      al, 0x64                 ; al = status register of PS/2 controller (8bit in length) 
    test    al, 2                    ; test the status of the input buffer
                                     ; to see if it's full or empty - bit 1(0x2).
    jnz     a20_wait_input_loop      ; if it's still busy, keep testing
    popa                             ; pop all registers off the stack
    ret                              ; return
;----------------------------------------------

;----------------------------------------------
; Waits for the output buffer of PS/2 controller
; to be free - see https://wiki.osdev.org/A20_Line.
; We need to make sure that it's ready before we
; read another data from it.
;
; port 0x64 (command port) is used for sending
; commands to keyboard controller (PS/2). 
;----------------------------------------------
a20_wait_output:
    pusha                            ; store all registers to the stack
a20_wait_output_loop:
    in      al, 0x64                 ; al = status register of PS/2 controller (8bit in length) 
    test    al, 1                    ; test the status of the output buffer
                                     ; to see if it's full or empty - bit 1(0x1).
    jz      a20_wait_output_loop     ; if it's still busy, keep testing
    popa                             ; pop all registers off the stack
    ret                              ; return
;----------------------------------------------