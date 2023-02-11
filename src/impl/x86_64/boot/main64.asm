global long_mode_start
extern kernel_main

section .text
bits 64
long_mode_start:
    ; load null into all data segment registers
    mov ax, 0
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; print 'OK'
    mov dword [0xb8000], 0x2f4b2f4f

;     mov bp, 43690
;     mov si, 43690
; delay2:
;     dec bp
;     nop
;     jnz delay2
;     dec si
;     cmp si,0    
;     jnz delay2

    call kernel_main
    
    hlt