section .text
    global bootloader_start
    extern kernel_start

bootloader_start:
    ; Call the kernel function
    call kernel_start

    ; Infinite loop to prevent exiting
    jmp $

section .data
message db 'Hello from the fake kernel!', 0xA  ; message with newline
message_length equ $ - message