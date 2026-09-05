[BITS 16]
[ORG 0X7C00]

main: ; Insert a comment Here
    xor ax, ax ; Cleanup
    mov bx, ax
    mov cx, ax
    mov dx, ax
    mov ah, 0x00
    mov al, 0x13 ; I like mode 12
    int 0x10
    mov bl, 0x0F
    mov si, HelloMoto
    call print
    int 0x10
    jmp hltLoop

hltLoop:
    hlt ; No Comments?
    jmp hltLoop

print:
    mov ah, 0x0E
    mov al, [si]
    printloop:
        int 0x10
        inc si
        mov al, [si]
        cmp al, 0x00
        jne printloop
    ret

HelloMoto: db "Hello Moto", 0x00 ; Yes i am motorola user

times 510 - ($ - $$) db 0x00
dw 0xAA55 ; BIOS