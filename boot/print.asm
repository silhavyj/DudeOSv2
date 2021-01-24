;----------------------------------------------
; prints out the string given as a parameter
; in the bx register on the screen
; 
; ah - BIOS service number (0x0E = printing)
; al - the byte (char) we want to print out
; bx - string given as a parameter
;----------------------------------------------
print_string:
    pusha                       ; push all registers on the stack - we don't want to overwrite them
    mov     ah, 0x0E            ; ah = 0x0E (printing to the screen)

print_string_loop:              
    mov     al, [bx]            ; al = *str 
    cmp     al, 0               ; test if al == 0
    je      print_string_end    ; if so, goto print_string_end
    int     0x10                ; call the BIOS interrupt
    add     bx, 1               ; str++
    jmp     print_string_loop   ; goto print_string_loop

print_string_end:
    popa                        ; pop all registers off the stack
    ret                         ; return
;----------------------------------------------

;----------------------------------------------
; moves the cursor at the beginning of the nexts
; line. This means printing out both characters
; '\n' and '\r' 
;
; ah - BIOS service number (0x0E = printing)
; al - the byte (char) we want to print out
;----------------------------------------------
print_ln:
    pusha                       ; push all registers onto the stack
    mov     ah, 0x0E            ; ah = 0x0E (printing to the screen)
    mov     al, 0x0A            ; al = 0x0A ('\n')
    int     0x10                ; call the BIOS interrupt to print it out
    mov     al, 0x0D            ; al = 0x0D ('\r')
    int     0x10                ; call the BIOS interrupt to print it out
    popa                        ; pop all registers off the stack
    ret                         ; return
;----------------------------------------------

;----------------------------------------------
; prints out the string given as a parameter
; in the bx register onto the screen along with
; a new line
;
; bx - string given as a parameter
;----------------------------------------------
println_string:
    call    print_string        ; print out the string
    call    print_ln            ; move the cursor at the beginning of the next line 
    ret                         ; return
;----------------------------------------------

;----------------------------------------------
; prints out the HEX number given as a parameter
; int the register dx. The number is supposed
; to be two bytes long e.g. (0xF0C3)
;----------------------------------------------
print_hex:
    pusha                       ; push all registers onto the stack
    mov     cx, 0               ; cx = 0 (index)

print_hex_loop:
    cmp     cx, 4               ; if (cx == 4) goto print_hex_end
    je      print_hex_end
    
    mov     ax, dx              ; ax = dx
    and     ax, 0x000F          ; 0x1234 -> 0x0004 by masking first three to zeros
    add     al, 0x30            ; add 0x30 to N to convert it to ASCII "N"
    cmp     al, 0x39            ; if > 9, add extra 8 to represent 'A' to 'F'
    jle     store_hex
    add     al, 7               ; 'A' is ASCII 65 instead of 58, so 65-58=7

store_hex:
    mov     bx, HEX_OUT + 5     ; bx = base + length
    sub     bx, cx              ; our index variable
    mov     [bx], al            ; copy the ASCII char in 'al' to the position pointed by 'bx'
    ror     dx, 4               ; 0x1234 -> 0x4123 -> 0x3412 -> 0x2341 -> 0x1234

    add     cx, 1               ; cx++
    jmp     print_hex_loop      ; move on to the next digit

print_hex_end:
    mov     bx, HEX_OUT         ; pass HEX_OUT as a parameter
    call    print_string        ; to the print_string procedure and call it
    popa                        ; pop all registers from the stack
    ret                         ; return
;----------------------------------------------

HEX_OUT:
    db '0x0000', 0              ; reserve a piece of memory for the final HEX number