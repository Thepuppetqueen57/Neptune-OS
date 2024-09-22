nasm -f elf32 boot.asm -o boot.o
gcc -m32 -c kernel.c -o kernel.o
gcc -m32 boot.o kernel.o -o Neptune