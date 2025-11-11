/*
 * Level 4: Return-to-libc Attack
 *
 * Learning objectives:
 * - Understand return-to-libc technique when stack is non-executable
 * - Learn how to chain function calls using stack manipulation
 * - Practice finding libc function addresses
 * - Execute system("/bin/sh") without injecting shellcode
 *
 * Goal: Use return-to-libc to spawn a shell without executing stack code
 *
 * Note: Compile with -z noexecstack to simulate DEP/NX protection
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void vulnerable_function(char *input) {
    char buffer[128];

    printf("Buffer is at: %p\n", buffer);
    printf("system() is at: %p\n", system);
    printf("exit() is at: %p\n", exit);

    // Print the address of a useful string
    char *shell_str = "/bin/sh";
    printf("\"/bin/sh\" string is at: %p\n", shell_str);

    // Vulnerable: no bounds checking!
    strcpy(buffer, input);

    printf("Returning from vulnerable_function...\n");
}

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: %s <input>\n", argv[0]);
        printf("Goal: Use return-to-libc to execute system(\"/bin/sh\")\n");
        printf("\nHints:\n");
        printf("1. Stack is marked non-executable (NX/DEP)\n");
        printf("2. Cannot execute shellcode on the stack\n");
        printf("3. Must use existing code in libc\n");
        printf("4. Overwrite return address with address of system()\n");
        printf("5. Set up stack to pass \"/bin/sh\" as argument\n");
        printf("6. Consider stack frame layout for function calls\n");
        return 1;
    }

    printf("=== Level 4: Return-to-libc Attack ===\n");
    vulnerable_function(argv[1]);

    printf("Returned normally.\n");
    return 0;
}
