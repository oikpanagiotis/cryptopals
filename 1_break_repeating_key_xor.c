#include <stdio.h>
#include <string.h>

#include "cryptopals.h"

#define MAX_LINE 60

static char *load_file(const char *filename, size_t *size_out) {
    FILE *f = fopen(filename, "rb");
    fseek(f, 0L, SEEK_END);
    size_t size = ftell(f);
    *size_out = size;
    fseek(f, 0L, SEEK_SET);

    size_t lines = 0;
    for (char c = getc(f); c != EOF; c = getc(f)) {
        if (c == '\n') {
            lines++;
        }
    }
    fseek(f, 0L, SEEK_SET);

    char *buf = calloc(1, size - lines + 1);
    size_t i = 0;
    for (char c = getc(f); c != EOF; c = getc(f)) {
        if (c != '\n') {
            buf[i++] = c;
        }
    }

    fclose(f);
    return buf;
}

int main(void) {
    size_t size = 0;
    char *ciphertext = load_file("resources/1_6.txt", &size);
    buf_t cipher_buf = base64_to_buf(ciphertext);    

    size_t scores[64] = {0};
    size_t scores_count = 0;

    buf_t key_candidates[512] = {0};
    size_t keys_count = 0;

    // create key candidates
    for (i32 byte = 0; byte < 256; byte++) {
        buf_t key = (buf_t){
            .data = malloc(cipher_buf.len),
            .len = cipher_buf.len
        };

        memset(key.data, (u8)byte, key.len);
        key_candidates[keys_count++] = key;
    }


    for (size_t key_size = 2; key_size < 40; key_size++) {
        buf_t b1 = (buf_t){cipher_buf.data, key_size};
        buf_t b2 = (buf_t){cipher_buf.data + key_size + 1, key_size};

        size_t distance = hamming_distance_buf(b1, b2) / key_size;
        scores[scores_count++] = distance;
    }

    size_t key_size = 0;
    for (size_t i = 0; i < scores_count; i++) {
        key_size = min(scores[i], key_size);
    }

    // break cipher into blocks of key_size length
    size_t num_blocks = cipher_buf.len / key_size;
    u8 *blocks = malloc(num_blocks * key_size);
    for (size_t i = 0; i < num_blocks; i += key_size) {
        memcpy(blocks + i, cipher_buf.data, key_size);
    }

    // transpose blocks so first one has all first bytes, ...
    u8 *transposed = malloc(num_blocks *key_size);
    for (size_t i = 0; i < num_blocks; i += key_size) {
        u8 byte = blocks[i];
        for (size_t j = 0; i < key_size; j++) {
            transposed[j] = byte;
        }
    }

    for (size_t i = 0; i < num_blocks; i += key_size) {
        u8 *block = transposed + i;
        buf_t block_buf = (buf_t){block, key_size};
        // ...
    }


    free(cipher_buf.data);
    free(ciphertext);
    return 0;
}

