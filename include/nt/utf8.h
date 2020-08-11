#ifndef NTUTILS_UTF8_H
#define NTUTILS_UTF8_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef _WIN32
#include <wchar.h>
#endif

_Static_assert(sizeof(char) == 1, "The size of the char type must be 1 byte");

enum { NT_UTF8_BOM_LEN = 3 };

typedef struct {
  uint32_t code_point;
} nt_cp_t;

#define nt_cp(code_point) ((nt_cp_t){code_point})

static inline bool nt_cp_is_valid(nt_cp_t cp) {
  return cp.code_point <= 0x10FFFF;
}

static inline bool nt_cp_is_newline(nt_cp_t cp) {
  return cp.code_point == '\n';
}

static inline bool nt_cp_is_whitespace(nt_cp_t cp) {
  switch (cp.code_point) {
  case ' ':
  case '\n':
  case '\r':
  case '\t':
    return true;
  default:
    return false;
  }
}

int nt_utf8_len(nt_cp_t cp);

nt_cp_t nt_utf8_read(char const **str, size_t *len);

void nt_utf8_write(char **str, size_t *len, nt_cp_t cp);

bool nt_utf8_has_bom(char const *str, size_t len);

void nt_utf8_write_bom(char *str);

#define nt_utf8_is_multibyte(str, ...)                                         \
  P_nt_utf8_is_multibyte(                                                      \
      str, NT_GET_ARG0_OR_DEFAULT((size_t)0xFFFFFFFFFFFFFFFFull, __VA_ARGS__))

bool P_nt_utf8_is_multibyte(char const *str, size_t len);

#ifdef _WIN32
#define nt_utf8_to_wide(dst, dst_len, src, ...)                                \
  P_nt_utf8_to_wide(dst, dst_len, src, NT_GET_ARG0_OR_DEFAULT(0, __VA_ARGS__))

size_t P_nt_utf8_to_wide(wchar_t *dst, size_t dst_len, char const *src,
                         size_t src_len);

#define nt_utf8_from_wide(dst, dst_len, src, ...)                              \
  P_nt_utf8_from_wide(dst, dst_len, src, NT_GET_ARG0_OR_DEFAULT(0, __VA_ARGS__))

size_t P_nt_utf8_from_wide(char *dst, size_t dst_len, wchar_t const *src,
                           size_t src_len);
#endif

#endif // NTUTILS_UTF8_H
