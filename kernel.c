#include <stdio.h>

void kernel_start() {
    printf("Kernel has been loaded!\n");
}

int main() {
    kernel_start();
    return 0;
}