;Copyright (c) 2026 jozin1224
[BITS 16]
[ORG 0X7C00]

main:
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov [BOOT_DRIVE], dl
    mov bp, 0x9000
    mov sp, bp
    mov bl, 0x0F
    mov si, HelloMoto
    call print
    call load_kernel
    jmp 0x1000

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

load_kernel:
    mov ah, 0x02
    mov al, 15
    mov ch, 0x00
    mov dh, 0x00
    mov cl, 0x02
    mov dl, [BOOT_DRIVE]
    xor bx, bx
    mov es, bx
    mov bx, 0x1000
    int 0x13
    jc disk_error
    ret

disk_error:
    mov bl, 0x0F
    mov si, ErrorMsg
    call print
    jmp hltLoop

HelloMoto: db "Kernel Initializing...", 0x0D, 0x0A, 0x00
ErrorMsg: db "Disk Error!", 0x00
BOOT_DRIVE: db 0

times 510 - ($ - $$) db 0x00
dw 0xAA55
