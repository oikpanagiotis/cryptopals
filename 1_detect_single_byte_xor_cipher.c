#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

#include "cryptopals.h"

#define MAX_ARRAY 512
#define MAX_LINE 60

buf_t *load_bufs(const char *filename, size_t *bufs_count_out) {
    FILE *f = fopen(filename, "r");

    size_t line_count = 0;
    for (i32 character = getc(f); character != EOF; character = getc(f)) {
        if (character == '\n') {
            line_count++;
        }
    }
    fseek(f, 0, SEEK_SET);

    buf_t *bufs = malloc(line_count * sizeof(buf_t));
    size_t bufs_count = 0;

    char hex[MAX_LINE + 1] = {0};
    while (fgets(hex, MAX_LINE + 1, f)) {
        hex[strcspn(hex, "\n")] = '\0';

        // there is a new line somewhere so we need to skip it
        if (strlen(hex) == 0) {
            continue;
        }

        buf_t buf = hex_to_buf(hex);
        bufs[bufs_count++] = buf;

        assert(bufs_count <= line_count);
        memset(hex, 0, MAX_LINE + 1);
    }

    *bufs_count_out = bufs_count;
    fclose(f);
    return bufs;
}


bool is_highest_score(i32 *scores, size_t len, i32 score) {
    i32 highest_score = 0;
    for (size_t i = 0; i < len; i++) {
        highest_score = max(scores[i], highest_score);
    }
    return score > highest_score;
}


int main(void) {
    size_t bufs_count = 0;
    buf_t *bufs = load_bufs("resources/1_4.txt", &bufs_count);

    i32 scores[MAX_ARRAY] = {0};
    size_t scores_count = 0;

    buf_t key_candidates[MAX_ARRAY] = {0};
    size_t keys_count = 0;
    size_t highest_scoring_key_idx = 0;

    // create key candidates
    for (i32 byte = 0; byte < 256; byte++) {
        buf_t key = (buf_t){
            .data = malloc(bufs[0].len),
            .len = bufs[0].len
        };

        memset(key.data, (u8)byte, key.len);
        key_candidates[keys_count++] = key;
    }

    // calculate top score of each line
    for (size_t buf_idx = 0; buf_idx < bufs_count; buf_idx++) {
        buf_t buf = bufs[buf_idx];
        for (size_t key_idx = 0; key_idx < keys_count; key_idx++) {
            buf_t candidate = key_candidates[key_idx];
            buf_t result = fixed_xor(buf, candidate);

            i32 buf_score = 0;
            for (size_t char_idx = 0; char_idx < result.len; char_idx ++) {
                u8 decoded = result.data[char_idx];
                buf_score += get_char_score((char)decoded);
            }

            if (is_highest_score(scores, scores_count, buf_score)) {
                highest_scoring_key_idx = key_idx;
            }

            scores[scores_count] = max(buf_score, scores[scores_count]);
            free(result.data);
        }
        scores_count++;
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

    buf_t buf = bufs[max_score_idx];
    buf_t key = key_candidates[highest_scoring_key_idx];
    buf_t result = fixed_xor(buf, key);
    print_buf_ascii(result);

    for (size_t i = 0; i < keys_count; i++) {
        free(key_candidates[i].data);
    }
    for (size_t i = 0; i < bufs_count; i++) {
        free(bufs[i].data);
    }
    free(bufs);
    free(result.data);
    return 0;
}

