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

u8 base64_char_to_byte(char c) {
    if (c >= 'A' && c <= 'Z'){
        return c - 'A';
    }
    if (c >= 'a' && c <= 'z') {
        return c - 'a' + 26;
    }
    if (c >= '0' && c <= '9') {
        return c - '0' + 52;
    }
    if (c == '+') {
        return 62;
    }
    if (c == '/') {
        return 63;
    }
    printf("Non base64 character: %c\n", c);
    assert(NULL && "Invalid character");
    return 0;
}

char nibble_to_hex(u8 nibble) {
    if (nibble <= 9) {
        return '0' + nibble;
    } 

    return 'a' + (nibble - 10);
}

// b64 digit(6 bits)  00111111
// hex digit(4 bits)  00001111
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
        // first 2 bits from nibble2 and remaining 4 bits of nibble3
        buf[j + 1] = b64_table[((nibble2 & 0x03)) << 4 | nibble3];
    }

    // might need to handle padding later

    return buf;
}

buf_t ascii_to_buf(const char *str) {
    size_t len = strlen(str);
    u8 *buf = malloc(len);
    memcpy(buf, str, len);
    return (buf_t){buf, len};
}

buf_t hex_to_buf(const char *hex) {
    size_t hex_len = strlen(hex);
    size_t buf_len = hex_len / 2;
    u8 *buf = malloc(buf_len);

    for (size_t i = 0, j = 0; i < hex_len; i += 2, j++) {
        u8 nibble1 = hex_to_nibble(hex[i]);
        u8 nibble2 = i + 1 < hex_len ? hex_to_nibble(hex[i + 1]) : 0;
        buf[j] = (nibble1 << 4) | nibble2;
    }

    return (buf_t){buf, buf_len};
}

buf_t base64_to_buf(char *b64) {
    size_t b64_len = strlen(b64);
    i32 padding = 0;
    for (size_t i = 0; i < b64_len; i++) {
        if (b64[i] == '=') {
            padding++;
        }
    }

    size_t buf_len = (b64_len * 3) / 4 - padding;
    u8 *buf = malloc(buf_len);

    for (size_t i = 0, j = 0; i < buf_len; i += 4, j += 3) {
        assert(i + 4 < b64_len && "Invalid idx for base64");
        assert(j + 3 < buf_len && "Invalid idx for bytes");

        // 6 bits for each base64 character
        u8 byte1 = base64_char_to_byte(b64[i]);
        u8 byte2 = base64_char_to_byte(b64[i + 1]);
        u8 byte3 = base64_char_to_byte(b64[i + 2]);
        u8 byte4 = base64_char_to_byte(b64[i + 3]);

        buf[j] = (byte1 << 2) | (byte2 >> 4);
        buf[j + 1] = (byte2 << 4) | (byte3 >> 2);
        buf[j + 2] = (byte3 << 6) | byte4;
    }
    return (buf_t){buf, buf_len};
}

buf_t fixed_xor(buf_t buf1, buf_t buf2) {
    assert(buf1.len == buf2.len && "buffers must be of the same size");

    buf_t result = (buf_t){
        .data = malloc(buf1.len),
        .len = buf1.len
    };
    for (size_t i = 0; i < buf1.len; i++) {
        result.data[i] = buf1.data[i] ^ buf2.data[i];
    }

    return result;
}

buf_t repeating_xor(buf_t buf, buf_t key) {
    buf_t result = (buf_t){
        .data = malloc(buf.len),
        .len = buf.len
    };
    size_t xored = 0;
    for (size_t i = 0; i < buf.len; i++) {
        size_t key_idx = xored++ % key.len;
        result.data[i] = buf.data[i] ^ key.data[key_idx];
    }
    return result;
}

char *buf_to_hex(buf_t buf) {
    size_t len = buf.len * 2;
    char *hexstr = calloc(1, len + 1);
    for (size_t i = 0, j = 0; i < len; i += 2, j++) {
        u8 nibble1 = (buf.data[j] >> 4) & 0x0f;
        u8 nibble2 = buf.data[j] & 0x0f;

        hexstr[i] = nibble_to_hex(nibble1);
        hexstr[i + 1] = nibble_to_hex(nibble2);
    }
    return hexstr;
}

char *buf_to_str(buf_t buf) {
    char *str = calloc(1, buf.len + 1);
    memcpy(str, buf.data, buf.len);
    return str;
}

void print_buf_raw(buf_t buf) {
    for (size_t i = 0; i < buf.len; i++) {
        printf("%d", buf.data[i]);
    }
    printf("\n");
}

void print_buf_ascii(buf_t buf) {
    for (size_t i = 0; i < buf.len; i++) {
        printf("%c", (char)buf.data[i]);
    }
    printf("\n");
}

void print_buf_hex(buf_t buf) {
    for (size_t i = 0; i < buf.len; i++) {
        printf("%02x", buf.data[i]);
    }
    printf("\n");
}

size_t hamming_distance(char *s1, char *s2) {
    size_t s1len = strlen(s1);
    size_t s2len = strlen(s2);
    assert(s1len == s2len && "Passed strings of different length");

    size_t diff_bits = 0;
    for (size_t i = 0; i < s1len; i++) {
        u8 xored = s1[i] ^ s2[i];
        while (xored) {
            diff_bits += xored & 0x01;
            xored >>= 1;
        }

    }
    return diff_bits;
}

size_t hamming_distance_buf(buf_t b1, buf_t b2) {
    assert(b1.len == b2.len && "Passed buffers of different length");

    size_t diff_bits = 0;
    for (size_t i = 0; i < b1.len; i++) {
        u8 xored = b1.data[i] ^ b2.data[i];
        while (xored) {
            diff_bits += xored & 0x01;
            xored >>= 1;
        }

    }
    return diff_bits;
}


bool string_equals(const char *s1, const char *s2) {
    if (!s1 || !s2) {
        return false;
    }
    return strcmp(s1, s2) == 0;
}

typedef struct char_score_t {
    char character;
    i32 value;
} char_score_t;

static char_score_t score_map[] = {
    {'Z', 1},
    {'z', 1},

    {'J', 2},
    {'j', 2},
    {'X', 2},
    {'x', 2},

    {'Q', 3},
    {'q', 3},

    {'K', 4},
    {'k', 4},

    {'V', 5},
    {'v', 5},

    {'B', 6},
    {'b', 6},

    {'G', 7},
    {'g', 7},
    {'P', 7},
    {'p', 7},

    {'W', 8},
    {'w', 8},
    {'Y', 8},
    {'y', 8},

    {'F', 9},
    {'f', 9},

    {'C', 10},
    {'c', 10},
    {'M', 10},
    {'m', 10},

    {'U', 11},
    {'u', 11},

    {'L', 12},
    {'l', 12},

    {'D', 13},
    {'d', 13},

    {'R', 14},
    {'r', 14},

    {'H', 15},
    {'h', 15},

    {'S', 16},
    {'s', 16},

    {' ', 17},

    {'N', 18},
    {'n', 18},

    {'I', 19},
    {'i', 19},

    {'O', 20},
    {'o', 20},

    {'A', 21},
    {'a', 21},

    {'T', 22},
    {'t', 22},

    {'E', 23},
    {'e', 23},
};

i32 get_char_score(char character) {
    for (size_t i = 0; i < array_len(score_map); i++) {
        char_score_t entry = score_map[i];
        if (character == entry.character) {
            return entry.value;
        }
    }
    return 0;
}

