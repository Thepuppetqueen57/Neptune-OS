# Detect the platform
OS := $(shell uname)

# Default target
all: build clean1 run clean2

# Build commands
ifeq ($(OS), Linux)
    TARGET := Neptune
    BUILD_CMD := \
        nasm -f elf32 bootl.asm -o boot.o && \
        gcc -m32 -c kernel.c -o kernel.o && \
        gcc -m32 boot.o kernel.o -o $(TARGET)
    RUN_CMD := ./$(TARGET)
else
    TARGET := Neptune.exe
    BUILD_CMD := \
        nasm -f win32 bootw.asm -o boot.o && \
        gcc -m32 -c kernel.c -o kernel.o && \
        gcc -m32 boot.o kernel.o -o $(TARGET)
    RUN_CMD := cmd /c $(TARGET)
endif

build:
	$(BUILD_CMD)

clean1:
	rm -f boot.o kernel.o

run:
	$(RUN_CMD)

clean2:
	rm -f $(TARGET)