/*
 * Level 3: Shellcode Injection and Execution
 *
 * Learning objectives:
 * - Understand how to inject executable code into a buffer
 * - Learn about NOPs (No Operation) and NOP sleds
 * - Practice calculating addresses for shellcode execution
 * - Execute your own shellcode on the stack
 *
 * Goal: Inject and execute shellcode to spawn a shell
 */

#include <stdio.h>
#include <string.h>

void vulnerable_function(char *input) {
    char buffer[512];  // Larger buffer for shellcode + NOP sled

    printf("Buffer is at: %p\n", buffer);
    printf("Size of buffer: %zu bytes\n", sizeof(buffer));

    // Vulnerable: no bounds checking!
    strcpy(buffer, input);

    printf("Input copied. Returning...\n");
}

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: %s <input>\n", argv[0]);
        printf("Goal: Inject and execute shellcode to spawn a shell\n");
        printf("\nHints:\n");
        printf("1. Place shellcode in the buffer\n");
        printf("2. Add a NOP sled before shellcode\n");
        printf("3. Overwrite return address to point into NOP sled\n");
        printf("4. Shellcode should spawn a shell (/bin/sh)\n");
        return 1;
    }

    printf("=== Level 3: Shellcode Injection ===\n");
    vulnerable_function(argv[1]);

    printf("Returned normally.\n");
    return 0;
}
