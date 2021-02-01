;---------------------------------------
; outb - sends a byte to an I/O port
; stack: [esp + 8] data byte
;        [esp + 4] I/O port
;        [esp    ] return address
;---------------------------------------
[global _outb]
_outb:
    mov     al, [esp + 8]       ; move the data to be sent into the al register
    mov     dx, [esp + 4]       ; move the address of the I/O port into the dx register
    out     dx, al              ; send the data to the I/O port
    ret                         ; return to the calling function
;---------------------------------------

;---------------------------------------
; inb - returns a byte from the given I/O port
; stack: [esp + 4] address of the I/O port
;---------------------------------------
[global _inb]
_inb:
    mov     dx, [esp + 4]       ; move the address of the I/O port to the dx register
    in      al, dx              ; read a byte from the I/O port and store it in the al register
    ret                         ; return the read byte
;---------------------------------------

;---------------------------------------
; halt - halts machine
;---------------------------------------
[global _halt]
_halt:
    hlt                         ; halt the CPU
;---------------------------------------

;---------------------------------------
; load_gdt - loads GDT table to memory using lgdt instruction
; stack: [esp + 4] GDTR record
;        [esp    ] return address
;---------------------------------------
[global _load_gdt]
_load_gdt:
    mov     eax, [esp + 4]      ; move GDTR address to eax
    lgdt    [eax]               ; load the address to internal segmentation register
    jmp     0x08:init_data_sp   ; perform far jump to "fix" CS (code segment register)

init_data_sp:
    ; 0x10 is the new data segment selector
    mov     ax, 0x10            ; move segment selector to ax
    mov     ds, ax              ; init DS
    mov     es, ax              ; init ES
    mov     fs, ax              ; init FS
    mov     gs, ax              ; init GS
    mov     ss, ax              ; init stack segment (stack is in data segment)
    ret                         ; return back
;---------------------------------------

;---------------------------------------
; load_idt - loads IDT table to memory using lidt instruction
; stack: [esp + 4] IDTR record
;        [esp    ] return address
;---------------------------------------
[global _load_idt]
_load_idt:
    mov     eax, [esp + 4]      ; move IDTR address to eax
    lidt    [eax]               ; load the address
    ret                         ; return back
;---------------------------------------

;---------------------------------------
; enable_interrupts - enables interrupts
;---------------------------------------
[global _enable_interrupts]
_enable_interrupts:
    sti                         ; enable interrupts
    ret                         ; return back
;---------------------------------------

;---------------------------------------
; disable_interrupts - enables interrupts
;---------------------------------------
[global _disable_interrupts]
_disable_interrupts:
    cli                         ; disable interrupts
    ret                         ; return back
;---------------------------------------

;---------------------------------------
; enables paging on the CPU
;---------------------------------------
[global _enable_paging]
_enable_paging:
    mov     eax, cr0            ; eax = cr0 (we can't access it directly)
    or      eax, 0x80000000     ; set the paging bit; see - https://wiki.osdev.org/Paging#Enabling
    mov     cr0, eax            ; cr0 = eax (update the control register)
    ret                         ; return
;---------------------------------------

;---------------------------------------
; disables paging on the CPU
;---------------------------------------
[global _disable_paging]
_disable_paging:
    mov     eax, cr0            ; eax = cr0 (we can't access it directly)
    and     eax, 0x7FFFFFFF     ; reset the paging bit; see - https://wiki.osdev.org/Paging#Enabling
    mov     cr0, eax            ; cr0 = eax (update the control register)
    ret                         ; return
;---------------------------------------

;---------------------------------------
; loads the page directory into the CPU
; when enabling paging
; stack: [esp + 4] page directory addr
;        [esp    ] return address
;---------------------------------------
[global _load_page_dir]
_load_page_dir:
    mov     eax, [esp + 4]      ; get the page dir addr from the stack
    mov     cr3, eax            ; save the page dir into register cr3
    ret                         ; return
;---------------------------------------