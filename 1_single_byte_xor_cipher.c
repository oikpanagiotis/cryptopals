#include <stdio.h>
#include <string.h>

#include "cryptopals.h"


#define MAX_ARRAY 512

char *cipher_xored = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";


int main(void) {
    i32 scores[MAX_ARRAY] = {0};
    size_t scores_count = 0;

    buf_t key_candidates[MAX_ARRAY] = {0};
    size_t keys_count = 0;

    buf_t cipher_buf = hex_to_buf(cipher_xored);

    // create key candidates
    for (i32 byte = 0; byte < 256; byte++) {
        buf_t key = (buf_t){
            .data = malloc(cipher_buf.len),
            .len = cipher_buf.len
        };

        memset(key.data, (u8)byte, key.len);
        key_candidates[keys_count++] = key;
    }

    // calculate key candidate scores
    for (size_t i = 0; i < keys_count; i++) {
        buf_t candidate = key_candidates[i];
        buf_t result = fixed_xor(cipher_buf, candidate);
        for (size_t c = 0; c < result.len; c++) {
            u8 decoded = result.data[c];
            scores[scores_count] += get_char_score((char)decoded);
        }
        scores_count++;
        free(result.data);
    }

    // find top scoring key
    i32 max_score_idx = 0;
    for (size_t i = 0; i < scores_count; i++) {
        i32 current_score = scores[i];
        i32 current_max_score = scores[max_score_idx];
        if (current_score > current_max_score) {
            max_score_idx = i;
        }
    }

    buf_t key = key_candidates[max_score_idx];
    buf_t result = fixed_xor(cipher_buf, key);
    printf("Key found! Decrypting with ");
    print_buf_ascii(key);
    print_buf_ascii(result);

    for (size_t i = 0; i < keys_count; i++) {
        free(key_candidates[i].data);
    }
    free(cipher_buf.data);
    free(result.data);
    return 0;
}
