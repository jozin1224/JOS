unset GTK_PATH

nasm -f elf32 Kernel/kernel_entry.asm -o Bin/kernel_entry.o
g++ -m32 -ffreestanding -fno-pie -fno-exceptions -fno-rtti -c Kernel/kernel.cpp -o Bin/kernel.o
ld -m elf_i386 -T linker.ld Bin/kernel_entry.o Bin/kernel.o -o Bin/kernel.bin
nasm -fbin Bootloader/boot.asm -o Bin/boot.bin
cat Bin/boot.bin Bin/kernel.bin > JOS.img
qemu-system-i386 -fda JOS.img -smp 2 -m 3G

