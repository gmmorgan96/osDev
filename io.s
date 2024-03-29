extern interrupt_handler           ; the function sum_of_three is defined elsewhere

global outb             ; make the label outb visible outside this file
                        ; outb - send a byte to an I/O port
                        ; stack: [esp + 8] the data byte
                        ; [esp + 4] the I/O port
                        ; [esp ] return address
global inb
                        ; inb - returns a byte from the given I/O port
                        ; stack: [esp + 4] The address of the I/O port
                        ; [esp ] The return address
;global load_idt
; load_idt - Loads the interrupt descriptor table (IDT).
; stack: [esp + 4] the address of the first entry in the IDT
; [esp ] the return address

outb:
    mov al, [esp + 8]   ; move the data to be sent into the al register
    mov dx, [esp + 4]   ; move the address of the I/O port into the dx register
    out dx, al          ; send the data to the I/O port
    ret                 ; return to the calling function

inb:
    mov dx, [esp + 4]   ; move the address of the I/O port to the dx register
    in al, dx           ; read a byte from the I/O port and store it in the al register
    ret                 ; return the read byte

global myprogram


myprogram:
    ; set eax to some distinguishable number, to read from the log afterwards
    mov eax, 0xDEADBEEF
    ; enter infinite loop, nothing more to do
    ; $ means "beginning of line", ie. the same instruction
    jmp $

%macro no_error_code_interrupt_handler 1
global interrupt_handler_%1
interrupt_handler_%1:
    push dword 0                    ; push 0 as error code
    push dword %1                   ; push the interrupt number
    jmp common_interrupt_handler    ; jump to the common handler
%endmacro

%macro error_code_interrupt_handler 1
global interrupt_handler_%1
interrupt_handler_%1:
    push dword %1 ; push the interrupt number
    jmp common_interrupt_handler ; jump to the common handler
%endmacro

common_interrupt_handler: ; the common parts of the generic interrupt handler
    ; save the registers
    push eax
    push ebx
    push ebp
    ; call the C function
    call interrupt_handler
    ; restore the registers
    pop ebp
    pop ebx
    pop eax
    ; restore the esp
    add esp, 8
    ; retur

no_error_code_interrupt_handler 0
no_error_code_interrupt_handler 1 ; create handler for interrupt 1

; load_idt:
;     mov eax, [esp+4] ; load the address of the IDT into register eax
;     lidt eax ; load the IDT
;     ret ; return to the calling function