#!/bin/bash
mkdir -p iso_root
nasm -f elf32 Kernel/kernel_entry.asm -o Bin/kernel_entry.o
g++ -m32 -ffreestanding -fno-pie -fno-exceptions -fno-rtti -c Kernel/kernel.cpp -o Bin/kernel.o
ld -m elf_i386 -T linker.ld Bin/kernel_entry.o Bin/kernel.o -o Bin/kernel.bin
nasm -fbin Bootloader/boot.asm -o Bin/boot.bin
cat Bin/boot.bin Bin/kernel.bin > Bin/JOS.img
dd if=/dev/zero of=Bin/disquete.img bs=1024 count=1440 2>/dev/null
dd if=Bin/JOS.img of=Bin/disquete.img conv=notrunc 2>/dev/null
mv Bin/disquete.img Bin/JOS.img
cp Bin/JOS.img iso_root/
xorriso -as mkisofs -R -b JOS.img -o JOS.iso iso_root/
rm -rf iso_root
