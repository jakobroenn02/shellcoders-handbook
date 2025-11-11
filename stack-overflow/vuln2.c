/*
 * Level 2: Overwriting Return Address
 *
 * Learning objectives:
 * - Understand the stack frame structure (saved EBP/RBP and return address)
 * - Learn how to calculate offset to return address
 * - Practice redirecting execution flow by overwriting return address
 *
 * Goal: Overflow the buffer to redirect execution to secret_function()
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void secret_function() {
    printf("\n*** CONGRATULATIONS! ***\n");
    printf("You've successfully redirected execution to secret_function()!\n");
    printf("You've learned how to control the instruction pointer via stack overflow!\n");
    exit(0);
}

void normal_function() {
    printf("This is the normal function. Nothing special here.\n");
}

void vulnerable_function(char *input) {
    char buffer[64];

    printf("Buffer is at: %p\n", buffer);
    printf("Secret function is at: %p\n", secret_function);
    printf("Normal function is at: %p\n", normal_function);

    // Vulnerable: no bounds checking!
    strcpy(buffer, input);

    printf("Returning from vulnerable_function...\n");
}

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: %s <input>\n", argv[0]);
        printf("Goal: Overflow the buffer to redirect execution to secret_function()\n");
        return 1;
    }

    printf("=== Level 2: Overwriting Return Address ===\n");
    vulnerable_function(argv[1]);

    // If we get here, the exploit didn't work
    normal_function();
    printf("Execution returned normally. Try harder!\n");

    return 0;
}
