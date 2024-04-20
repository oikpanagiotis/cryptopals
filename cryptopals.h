#ifndef CRYPTOPALS_H
#define CRYPTOPALS_H

#include <stdint.h>
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

u8 hex_to_nibble(char hex_digit);
char *hex_to_base64(const char *hex);
bool string_equals(const char *s1, const char *s2);

#endif
