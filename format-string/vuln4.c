/*
 * Level 4: Arbitrary Write - Advanced Challenge
 *
 * Goal: Overwrite a function pointer to execute the secret_function
 * This requires precise control over what you write and where
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void normal_function() {
    printf("This is the normal execution path.\n");
}

void secret_function() {
    printf("\n*** CONGRATULATIONS! ***\n");
    printf("You've successfully exploited an arbitrary write!\n");
    printf("Flag: FLAG{arb1tr4ry_wr1t3_m4st3r}\n");
}

void (*func_ptr)() = normal_function;

int main(int argc, char **argv) {
    char buffer[256];

    printf("=== Advanced Format String Challenge ===\n");
    printf("normal_function at: %p\n", normal_function);
    printf("secret_function at: %p\n", secret_function);
    printf("func_ptr at: %p\n", &func_ptr);
    printf("func_ptr points to: %p\n\n", func_ptr);

    if (argc < 2) {
        printf("Usage: %s <format string>\n", argv[0]);
        printf("\nGoal: Overwrite func_ptr to point to secret_function\n");
        printf("Hint: You may need multiple writes for a full address\n");
        printf("Hint: Use %%hn for half-word writes or %%hhn for byte writes\n");
        return 1;
    }

    strcpy(buffer, argv[1]);

    printf("Processing input...\n");
    printf(buffer);  // VULNERABLE
    printf("\n\n");

    printf("func_ptr now points to: %p\n", func_ptr);
    printf("Calling function pointer...\n\n");

    func_ptr();

    return 0;
}
