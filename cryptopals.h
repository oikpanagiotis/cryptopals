#ifndef CRYPTOPALS_H
#define CRYPTOPALS_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>


typedef int64_t i64;
typedef int32_t i32;
typedef int16_t i16;
typedef int8_t i8;

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

typedef float f32;
typedef double f64;

#define array_len(arr) (sizeof(arr) / sizeof(arr[0]))
#define max(a, b) (((a) > (b)) ? (a) : (b))
#define min(a, b) (((a) < (b)) ? (a) : (b))

typedef struct buf_t {
    u8 *data;
    size_t len;
} buf_t;

u8 hex_to_nibble(char hex_digit);
u8 base64_char_to_byte(char c);
char nibble_to_hex(u8 nibble);

buf_t ascii_to_buf(const char *str);
buf_t hex_to_buf(const char *hex);
buf_t base64_to_buf(char *b64);
buf_t fixed_xor(buf_t buf1, buf_t buf2);
buf_t repeating_xor(buf_t buf, buf_t key);
char *buf_to_hex(buf_t buf);
char *buf_to_str(buf_t buf);

void print_buf_raw(buf_t buf);
void print_buf_ascii(buf_t buf);
void print_buf_hex(buf_t buf);

char *hex_to_base64(const char *hex);
size_t hamming_distance(char *s1, char *s2);
size_t hamming_distance_buf(buf_t b1, buf_t b2);
bool string_equals(const char *s1, const char *s2);
i32 get_char_score(char character);

#endif
