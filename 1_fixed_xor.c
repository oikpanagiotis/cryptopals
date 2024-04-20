#include <stdio.h>
#include <stdlib.h>

#include "cryptopals.h"

char input_hex[] = "1c0111001f010100061a024b53535009181c";
char xor_hex[] = "686974207468652062756c6c277320657965";
char expected_hex[] = "746865206b696420646f6e277420706c6179";

int main(void) {
    buf_t input = hex_to_buf(input_hex);
    buf_t xor = hex_to_buf(xor_hex);

    buf_t result = fixed_xor(input, xor);

    char *result_hex = buf_to_hex(result);
    if (string_equals(result_hex, expected_hex)) {
        printf("Correct result! %s\n", result_hex);
    } else {
        printf("Wrong result! %s\n", result_hex);
    }

    free(input.data);
    free(xor.data);
    free(result.data);
    free(result_hex);
    return 0;
}

