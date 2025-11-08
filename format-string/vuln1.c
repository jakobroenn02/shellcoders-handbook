/*
 * Level 1: Basic Format String Vulnerability
 *
 * Goal: Use format string to read values from the stack
 * Try: ./vuln1 "%x %x %x %x"
 */

#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
    char buffer[100];
    int secret = 0xdeadbeef;

    printf("Secret value is at: %p\n", &secret);
    printf("Secret value is: 0x%x\n\n", secret);

    if (argc < 2) {
        printf("Usage: %s <input>\n", argv[0]);
        return 1;
    }

    strcpy(buffer, argv[1]);

    printf("You entered: ");
    printf(buffer);  // VULNERABLE: User input directly as format string
    printf("\n");

    printf("\nCan you leak the secret value (0x%x) using format strings?\n", secret);

    return 0;
}
