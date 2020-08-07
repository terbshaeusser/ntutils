#ifndef NTUTILS_STR_H
#define NTUTILS_STR_H

#define _GNU_SOURCE
#include "nt/macros.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

typedef struct {
  char const *str;
  size_t len;
} nt_cstr_t;

#define nt_cstr(str, ...)                                                      \
  ((nt_cstr_t){str, NT_GET_ARG0_OR_DEFAULT(strlen(str), __VA_ARGS__)})

typedef struct {
  char *P_data;
  size_t P_capacity;
  size_t P_len_and_short;
} nt_str_t;

#define nt_str_new(src)                                                        \
  _Generic(src, nt_cstr_t                                                      \
           : P_nt_str_new_cstr, nt_str_t*                                      \
           : P_nt_str_new_str, nt_str_t const*                                 \
           : P_nt_str_new_str                                                  \
  )(src)

nt_str_t P_nt_str_new_cstr(nt_cstr_t src);

nt_str_t P_nt_str_new_str(nt_str_t const *src);

nt_str_t nt_str_empty();

#define nt_str_of(src, count)                                                  \
  _Generic(src, nt_cstr_t                                                      \
           : P_nt_str_of_cstr, nt_str_t*                                       \
           : P_nt_str_of_str, nt_str_t const*                                  \
           : P_nt_str_of_str                                                   \
  )(src, count)

nt_str_t P_nt_str_of_cstr(nt_cstr_t src, size_t count);

nt_str_t P_nt_str_of_str(nt_str_t const *src, size_t count);

void nt_str_free(nt_str_t *self);

size_t nt_str_len(nt_str_t const *self);

size_t nt_str_capacity(nt_str_t const *self);

char const *nt_str_str(nt_str_t const *self);

#define nt_str_equal(self, other)                                              \
  _Generic(other, nt_cstr_t                                                    \
           : P_nt_str_equal_cstr, nt_str_t*                                    \
           : P_nt_str_equal_str, nt_str_t const*                               \
           : P_nt_str_equal_str                                                \
  )(self, other)

bool P_nt_str_equal_cstr(nt_str_t const *self, nt_cstr_t other);

bool P_nt_str_equal_str(nt_str_t const *self, nt_str_t const *other);

void nt_str_reserve(nt_str_t *self, size_t amount);

void nt_str_shrink(nt_str_t *self);

#define nt_str_append(self, src)                                               \
  _Generic(src, nt_cstr_t                                                      \
           : P_nt_str_append_cstr, nt_str_t*                                   \
           : P_nt_str_append_str, nt_str_t const*                              \
           : P_nt_str_append_str, int8_t                                       \
           : P_nt_str_append_int, int16_t                                      \
           : P_nt_str_append_int, int32_t                                      \
           : P_nt_str_append_int, int64_t                                      \
           : P_nt_str_append_int, uint8_t                                      \
           : P_nt_str_append_uint, uint16_t                                    \
           : P_nt_str_append_uint, uint32_t                                    \
           : P_nt_str_append_uint, uint64_t                                    \
           : P_nt_str_append_uint, float                                       \
           : P_nt_str_append_double, double                                    \
           : P_nt_str_append_double                                            \
  )(self, src)

void P_nt_str_append_cstr(nt_str_t *self, nt_cstr_t src);

void P_nt_str_append_str(nt_str_t *self, nt_str_t const *src);

void P_nt_str_append_int(nt_str_t *self, int64_t src);

void P_nt_str_append_uint(nt_str_t *self, uint64_t src);

void P_nt_str_append_double(nt_str_t *self, double src);

nt_str_t nt_str_format(char const *fmt, ...);

nt_str_t nt_str_vformat(char const *fmt, va_list args);

void nt_str_append_format(nt_str_t *self, char const *fmt, ...);

void nt_str_append_vformat(nt_str_t *self, char const *fmt, va_list args);

#define NT_STR_NOT_FOUND ((size_t)0xFFFFFFFFFFFFFFFFull)

#define nt_str_find(self, sub_str, ...)                                        \
  _Generic(sub_str, nt_cstr_t                                                  \
           : P_nt_str_find_cstr, nt_str_t*                                     \
           : P_nt_str_find_str, nt_str_t const*                                \
           : P_nt_str_find_str                                                 \
  )(self, sub_str, NT_GET_ARG0_OR_DEFAULT(0, __VA_ARGS__))

size_t P_nt_str_find_cstr(nt_str_t const *self, nt_cstr_t sub_str,
                          size_t start);

size_t P_nt_str_find_str(nt_str_t const *self, nt_str_t const *sub_str,
                         size_t start);

#define nt_str_rfind(self, sub_str, ...)                                       \
  _Generic(sub_str, nt_cstr_t                                                  \
           : P_nt_str_rfind_cstr, nt_str_t*                                    \
           : P_nt_str_rfind_str, nt_str_t const*                               \
           : P_nt_str_rfind_str                                                \
  )(self, sub_str, NT_GET_ARG0_OR_DEFAULT(0xFFFFFFFFFFFFFFFFull, __VA_ARGS__))

size_t P_nt_str_rfind_cstr(nt_str_t const *self, nt_cstr_t sub_str,
                           size_t start);

size_t P_nt_str_rfind_str(nt_str_t const *self, nt_str_t const *sub_str,
                          size_t start);

#endif // NTUTILS_STR_H
