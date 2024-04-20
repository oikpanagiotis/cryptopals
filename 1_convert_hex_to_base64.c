#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "cryptopals.h"

const char input[] = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
const char expected[] = "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t";


int main(void) {
    char *result = hex_to_base64(input);
    if (string_equals(result, expected)) {
        printf("Correct result! %s\n", result);
    } else {
        printf("Wrong result! %s\n", result);
    }

    free(result);
    return 0;
}

