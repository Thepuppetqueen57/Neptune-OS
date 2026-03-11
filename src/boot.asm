; Define the boot message string with a newline character (ASCII 10)
section .data
    boot_msg db "Booting...", 10

section .text
    global bootloader_start
    extern main

bootloader_start:
    ; Print "Booting..." to stdout via Linux sys_write syscall
    mov eax, 4
    mov ebx, 1
    mov ecx, boot_msg
    mov edx, 11
    int 0x80

    ; Hand off execution to the C main function
    call main

    ; Hang forever if main() returns (should never reach here)
    jmp $

; Mark the stack as non-executable (security/linking hint for the GNU linker)
section .note.GNU-stack noalloc noexec nowrite progbits