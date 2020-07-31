#ifndef NTUTILS_UTF8_H
#define NTUTILS_UTF8_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

enum { NT_UTF8_BOM_LEN = 3 };

typedef struct {
  uint32_t code_point;
} nt_cp_t;

#define nt_cp(code_point) ((nt_cp_t){code_point})

static inline bool nt_cp_is_valid(nt_cp_t cp) {
  return cp.code_point <= 0x10FFFF;
}

int nt_utf8_len(nt_cp_t cp);

nt_cp_t nt_utf8_read(char const **str, size_t *len);

void nt_utf8_write(char **str, size_t *len, nt_cp_t cp);

bool nt_utf8_has_bom(char const *str, size_t len);

void nt_utf8_write_bom(char *str);

#endif // NTUTILS_UTF8_H
