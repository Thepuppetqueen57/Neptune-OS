# Default target
all: build

# Compiler settings
CC = clang

# Targets
build:
	@$(MAKE) --no-print-directory cleanobjs

	@echo "Compiling source files"
	@$(CC) -c src/boot.c -o boot.o
	@$(CC) -c src/kernel.c -o kernel.o
	@$(CC) -c src/terminal.c -o terminal.o
	@$(CC) -c src/programs/calculator.c -o calculator.o

	@echo "Building libraries..."
	@$(CC) -c lib/cJSON.c -o cJSON.o
	@$(CC) -c lib/seqft/tokenizer.c -o tokenizer.o
	@$(CC) -c lib/seqft/evaluator.c -o evaluator.o
	@$(CC) -c lib/seqft/stack.c -o stack.o
	@$(CC) -c lib/seqft/common.c -o common.o

	@echo "Linking compiled files..."
	@$(CC) boot.o kernel.o terminal.o calculator.o \
		cJSON.o tokenizer.o evaluator.o stack.o common.o \
		-o Neptune \
		-lm -no-pie

	@echo "Done building Neptune OS!"

	@$(MAKE) --no-print-directory cleanobjs

run:
	@$(MAKE) --no-print-directory build
	@./Neptune
	@$(MAKE) --no-print-directory clean

cleanobjs:
	@echo "Cleaning up object files..."
	@find ./ -name "*.o" -delete

cleanexec:
	@echo "Cleaning up build executable..."
	@rm -f Neptune

clean:
	@$(MAKE) --no-print-directory cleanobjs
	@$(MAKE) --no-print-directory cleanexec

.PHONY: all build run cleanobjs cleanexec clean
