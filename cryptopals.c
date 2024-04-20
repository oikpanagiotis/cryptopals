#include "cryptopals.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>


static char b64_table[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                                'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                                'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                                'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                                'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                                'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                                'w', 'x', 'y', 'z', '0', '1', '2', '3',
                                '4', '5', '6', '7', '8', '9', '+', '/'};

u8 hex_to_nibble(char hex_digit) {
    if (hex_digit >= '0' && hex_digit <= '9') {
        return hex_digit - '0';
    } else if (hex_digit >= 'A' && hex_digit <= 'F') {
        return hex_digit - 'A' + 10;
    } else if (hex_digit >= 'a' && hex_digit <= 'f') {
        return hex_digit - 'a' + 10;
    } else {
        assert(NULL && "Invalid input");
    }
}

// b64 digit(6 bits)  111111
// hex digit(4 bits)    1111
//
// 3 hex -> 2 b64 
char *hex_to_base64(const char *hex) {
    size_t hex_len = strlen(hex);
    size_t base64_len = (hex_len * 2) / 3;
    char *buf = calloc(1, base64_len + 1);

    for (size_t i = 0, j = 0; i < hex_len; i += 3, j += 2) {
        u8 nibble1 = hex_to_nibble(hex[i]);
        u8 nibble2 = i + 1 < hex_len ? hex_to_nibble(hex[i + 1]) : 0;
        u8 nibble3 = i + 2 < hex_len ? hex_to_nibble(hex[i + 2]) : 0;

        //printf("Nibble %d 0x%x %08b\n", 1, nibble1, nibble1);
        //printf("Nibble %d 0x%x %08b\n", 2, nibble2, nibble2);
        //printf("Nibble %d 0x%x %08b\n", 3, nibble3, nibble3);
        //printf("=====================\n");
        // first 4 bits from nibble1 and 3rd,4th bit from nibble2
        buf[j] = b64_table[(nibble1 << 2) | (nibble2 >> 2)];
        // first 2bits from nibble 2 and remaining 4 bits of nibble3
        buf[j + 1] = b64_table[((nibble2 & 0x03)) << 4 | nibble3];
    }

    // might need to handle padding later

    return buf;
}


bool string_equals(const char *s1, const char *s2) {
    return strcmp(s1, s2) == 0;
}
