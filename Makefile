all: build clean1 run clean2

build:
	nasm -f elf32 boot.asm -o boot.o
	gcc -m32 -c kernel.c -o kernel.o
	gcc -m32 boot.o kernel.o -o Neptune

clean1:
	rm -f boot.o kernel.o

run:
	./Neptune

clean2:
	rm -f Neptune