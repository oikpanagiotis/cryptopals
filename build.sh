#!/bin/bash

gcc -Wall -Wextra cryptopals.c 1_convert_hex_to_base64.c -lm -o hex2b64
gcc -Wall -Wextra cryptopals.c 1_fixed_xor.c -lm -o fixed_xor
gcc -Wall -Wextra cryptopals.c 1_single_byte_xor_cipher.c -lm -o xor_cipher
gcc -Wall -Wextra cryptopals.c 1_detect_single_byte_xor_cipher.c -lm -o detect_xor_cipher
