section .text
    global bootloader_start
    extern kernel_start

bootloader_start:
    call kernel_start

    jmp $

section .data
message db 'Hello from the fake kernel!', 0xA
message_length equ $ - message