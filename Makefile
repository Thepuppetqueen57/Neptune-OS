# Default target
all: run

# Targets
build:
	@echo "Building Neptune..."
	@if [ ! -f Neptune ]; then \
		echo "Cleaning up old object files..."; \
		rm -f boot.o kernel.o cJSON.o; \
	fi; \

	@nasm -f elf32 boot.asm -o boot.o && \
	gcc -m32 -c kernel.c -o kernel.o && \
	gcc -m32 -c lib/cJSON.c -o cJSON.o && \
	gcc -m32 boot.o kernel.o cJSON.o -o Neptune

	@make clean1

clean1:
	@rm -f boot.o kernel.o cJSON.o

run:
	@make build
	@./Neptune
	@make clean1
	@make clean2

clean2:
	@rm -f Neptune

clean:
	@make clean1
	@make clean2

.PHONY: all build clean1 run clean2