#include <stdio.h>

// Forward declaration of kernel_main function
// This is so I don't have to make a header file cuz I hate header files
int kernel_main();

int bootloader() {
    printf("Booting...\n");
    kernel_main();
    return 0;
}

// Pretend this doesnt exist
// When I tried setting the entry point to bootloader it didnt work
// So I just made this I guess
int main() {
    return bootloader();
}