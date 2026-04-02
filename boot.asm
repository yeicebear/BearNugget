ORG 0x7C00
BITS 16

start:
    mov si, msg
    mov ah, 0eh
    mov bx, 0

.loop:
    lodsb
    cmp al, 0
    je .done
    int 10h
    jmp .loop

.done:
    jmp $

msg db 'Welcome to BearNugget', 0

times 510-($-$$) db 0
dw 0xAA55