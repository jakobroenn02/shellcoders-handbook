/*
 * Level 1: Basic Buffer Overflow - Overwriting Adjacent Variables
 *
 * Learning objectives:
 * - Understand stack layout and how local variables are stored
 * - Learn how buffers can overflow into adjacent memory
 * - Practice overwriting variables to change program behavior
 *
 * Goal: Overwrite the 'auth' variable to be 0xdeadbeef
 */

#include <stdio.h>
#include <string.h>

void vulnerable_function(char *input) {
    int auth = 0;
    char buffer[64];

    printf("Buffer is at: %p\n", buffer);
    printf("Auth is at: %p\n", &auth);
    printf("Auth value: 0x%x\n", auth);

    // Vulnerable: no bounds checking!
    strcpy(buffer, input);

    printf("Auth value after copy: 0x%x\n", auth);

    if (auth == 0xdeadbeef) {
        printf("\n*** SUCCESS! You've overwritten auth correctly! ***\n");
        printf("You've learned how buffer overflows can modify adjacent stack variables!\n");
    } else {
        printf("\nAuth is not correct. Try overflowing the buffer to overwrite it.\n");
    }
}

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: %s <input>\n", argv[0]);
        printf("Goal: Overflow the buffer to set auth = 0xdeadbeef\n");
        return 1;
    }

    printf("=== Level 1: Basic Buffer Overflow ===\n");
    vulnerable_function(argv[1]);

    return 0;
}
