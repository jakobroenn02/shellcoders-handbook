/*
 * Level 3: Writing to Memory with %n
 *
 * Goal: Overwrite the 'auth' variable to bypass authentication
 * Use %n to write the number of bytes printed
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int auth = 0;

int main(int argc, char **argv) {
    char buffer[200];

    printf("=== Authentication Challenge ===\n");
    printf("auth variable at: %p\n", &auth);
    printf("auth value: %d\n\n", auth);

    if (argc < 2) {
        printf("Usage: %s <format string>\n", argv[0]);
        printf("Hint: Use %%n to write to memory\n");
        printf("Hint: Address of auth is %p\n", &auth);
        return 1;
    }

    strcpy(buffer, argv[1]);

    printf("Processing input...\n");
    printf(buffer);  // VULNERABLE
    printf("\n\n");

    printf("auth value after: %d\n", auth);

    if (auth == 1337) {
        printf("\n*** SUCCESS! You've bypassed authentication! ***\n");
        printf("Flag: FLAG{f0rm4t_str1ng_wr1t3}\n");
    } else {
        printf("Authentication failed. auth = %d, needed 1337\n", auth);
    }

    return 0;
}
