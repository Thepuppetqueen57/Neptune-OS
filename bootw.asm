section .text
    global bootloader_start
    extern kernel_start
    extern ExitProcess

bootloader_start:
    call kernel_start

    mov rdx, message
    mov rcx, message_length
    mov rcx, 0
    call ExitProcess

section .data
message db 'Hello from the fake kernel!', 0xA
message_length equ $ - message