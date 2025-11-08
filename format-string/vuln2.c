/*
 * Level 2: Reading Specific Memory Locations
 *
 * Goal: Use direct parameter access to read the secret value
 * Try using %N$x to access the Nth parameter
 */

#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
    char buffer[100];
    int secret1 = 0xcafebabe;
    int secret2 = 0xdeadbeef;
    int secret3 = 0x31337;

    printf("=== Memory Layout ===\n");
    printf("secret1: 0x%x at %p\n", secret1, &secret1);
    printf("secret2: 0x%x at %p\n", secret2, &secret2);
    printf("secret3: 0x%x at %p\n", secret3, &secret3);
    printf("buffer at: %p\n", buffer);
    printf("\n");

    if (argc < 2) {
        printf("Usage: %s <format string>\n", argv[0]);
        printf("Hint: Use %%N$x to access the Nth parameter\n");
        return 1;
    }

    strcpy(buffer, argv[1]);

    printf("Output: ");
    printf(buffer);  // VULNERABLE
    printf("\n");

    printf("\nChallenge: Can you read all three secrets using format strings?\n");

    return 0;
}
