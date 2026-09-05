unset GTK_PATH

# This code compile the OS

nasm -fbin Bootloader/boot.asm -o Bin/boot.img #compile Boot
qemu-system-i386 -fda Bin/boot.img -smp 2 -m 3G # This is my old vostro 1320 specs