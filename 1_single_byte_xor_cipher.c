#include <stdio.h>
#include <string.h>

#include "cryptopals.h"


#define MAX_ARRAY 512

char *cipher_xored = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";

typedef struct char_score_t {
    char character;
    i32 value;
} char_score_t;

char_score_t score_map[] = {
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


int main(void) {
    i32 scores[MAX_ARRAY] = {0};
    size_t scores_count = 0;

    buf_t key_candidates[MAX_ARRAY] = {0};
    size_t keys_count = 0;

    buf_t cipher_buf = hex_to_buf(cipher_xored);

    // Create key candidates
    for (i32 byte = 0; byte < 256; byte++) {
        buf_t key = (buf_t){
            .data = malloc(cipher_buf.len),
            .len = cipher_buf.len
        };

        memset(key.data, (u8)byte, key.len);
        key_candidates[keys_count++] = key;
    }

    // Calculate key candidate scores
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

    // Find top scoring key
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
    print_buf_ascii(result);

    for (size_t i = 0; i < keys_count; i++) {
        free(key_candidates[i].data);
    }
    free(cipher_buf.data);
    free(result.data);
    return 0;
}
