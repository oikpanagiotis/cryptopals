#include <stdio.h>

#include "cryptopals.h"

const char *to_encrypt = "Burning 'em, if you ain't quick and nimble\nI go crazy when I hear a cymbal";
const char *encryption_key = "ICE";

void cli_mode(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: ./repeat_xor <filename>\n");
    }

    char *filename = argv[1];
    FILE *f = fopen(filename, "rb");
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);

    u8 *file_buf = malloc(size);
    fread(file_buf, size, 1, f);
    fclose(f);

    buf_t key = ascii_to_buf(encryption_key);
    buf_t buf = (buf_t){file_buf, size};
    buf_t result = repeating_xor(buf, key);

    fwrite(result.data, 1, result.len, stdout);

    free(key.data);
    free(buf.data);
    free(result.data);
}

int main(int argc, char *argv[]) {
    if (argc > 1) {
        cli_mode(argc, argv);
        exit(0);
    }


    buf_t buf = ascii_to_buf(to_encrypt);
    buf_t key = ascii_to_buf(encryption_key);

    buf_t result = repeating_xor(buf, key);
    print_buf_hex(result);

    free(buf.data);
    free(key.data);
    free(result.data);
    return 0;
}

