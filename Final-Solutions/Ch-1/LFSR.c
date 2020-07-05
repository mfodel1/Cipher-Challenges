#include <stdio.h>
#include <string.h>
# define FEEDBACK 0x87654321
// adapted from https://stackoverflow.com/questions/44584215/linear-feedback-shift-register-explaination

/* In-place string obfuscation */
unsigned char *Crypt(unsigned char *data, int dataLength, unsigned int initialValue) {
    int i, j, lsb;

    for (i = 0; i < dataLength; i++) {
        for (j = 0; j < 8; j++) { // a key value needs to read after 8 steps
            lsb = initialValue & 1;
            initialValue >>= 1;
            if (lsb) initialValue ^= FEEDBACK;
        }
    data[i] ^= initialValue & 0x00ff; // apply the key, XOR the first byte of the key with the first byte of the input.
    }
    return data;
}

// used for testing challenge #1
int main(void) {
    int i, n;
//message and intial value passed corresponde with provided example tests.
    unsigned char message[] = "apple";

    printf("Original message: %s\n", message);
    n = strlen(message);
    printf("Data Length: %d\n", n);
    printf("Intial Value: 0x12345678 (Hard Coded)\n");
    Crypt(message, strlen(message), 0x12345678);

    printf("Obfuscated results : ");
    for (i=0; i<n; i++) printf("x%02hhX ", message[i]);
    putchar('\n');

    // Repeat to retrieve original message
    Crypt(message, strlen(message), 0x12345678);
    printf("Recovered message: %s\n", message);

    return 0;
}
