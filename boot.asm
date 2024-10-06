section .text
    global bootloader_start
    extern kernel_start

bootloader_start:
    call kernel_start

    jmp $