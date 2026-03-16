# Default target
all: run

# Targets
build:
	@echo "Building Neptune..."
	@if [ ! -f Neptune ]; then \
		echo "Cleaning up old object files..."; \
		rm -f boot.o kernel.o cJSON.o terminal.o tokenizer.o evaluator.o stack.o common.o; \
	fi

#   Compile source files
	@nasm -f elf32 src/boot.asm -o boot.o
	@gcc -m32 -c src/kernel.c -o kernel.o
	@gcc -m32 -c src/terminal.c -o terminal.o
	@gcc -m32 -c src/programs/calculator.c -o calculator.o

#   Compile library files
	@gcc -m32 -c lib/cJSON.c -o cJSON.o
	@gcc -m32 -c lib/seqft/tokenizer.c -o tokenizer.o
	@gcc -m32 -c lib/seqft/evaluator.c -o evaluator.o
	@gcc -m32 -c lib/seqft/stack.c -o stack.o
	@gcc -m32 -c lib/seqft/common.c -o common.o

#   Link object files into final executable
	@gcc -m32 boot.o kernel.o terminal.o calculator.o \
		cJSON.o tokenizer.o evaluator.o stack.o common.o \
		-o Neptune -lm -no-pie -e bootloader_start

	@$(MAKE) --no-print-directory clean1

clean1:
	@rm -f boot.o kernel.o terminal.o calculator.o \
		cJSON.o tokenizer.o evaluator.o stack.o common.o

run:
	@$(MAKE) --no-print-directory build
	@./Neptune
	@$(MAKE) --no-print-directory clean1
	@$(MAKE) --no-print-directory clean2

clean2:
	@rm -f Neptune

clean:
	@$(MAKE) --no-print-directory clean1
	@$(MAKE) --no-print-directory clean2

.PHONY: all build clean1 run clean2 clean