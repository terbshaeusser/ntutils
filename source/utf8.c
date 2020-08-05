#include "nt/utf8.h"
#include <string.h>

int nt_utf8_len(nt_cp_t cp) {
  if (cp.code_point <= 0x7F) {
    return 1;
  }

  if (cp.code_point <= 0x7FF) {
    return 2;
  }

  if (cp.code_point <= 0xFFFF) {
    return 3;
  }

  if (cp.code_point <= 0x10FFFF) {
    return 4;
  }

  return 0;
}

nt_cp_t nt_utf8_read(char const **str, size_t *len) {
  if (len[0] == 0) {
    goto invalid;
  }

  uint8_t first = (uint8_t)str[0][0];
  ++str[0];
  --len[0];

  if (first <= 0x7Fu) {
    return (nt_cp_t){first};
  }

  if (len[0] == 0) {
    goto invalid;
  }

  uint8_t second = (uint8_t)str[0][0];
  ++str[0];
  --len[0];

  if ((second & 0xC0u) != 0x80u) {
    goto invalid;
  }

  if ((first & 0xE0u) == 0xC0u) {
    return (nt_cp_t){((first & 0x1Fu) << 6u) | (second & 0x3Fu)};
  }

  if (len[0] == 0) {
    goto invalid;
  }

  uint8_t third = (uint8_t)str[0][0];
  ++str[0];
  --len[0];

  if ((third & 0xC0u) != 0x80u) {
    goto invalid;
  }

  if ((first & 0xF0u) == 0xE0u) {
    return (nt_cp_t){((first & 0x0Fu) << 12u) | ((second & 0x3Fu) << 6u) |
                     (third & 0x3Fu)};
  }

  if (len[0] == 0) {
    goto invalid;
  }

  uint8_t fourth = (uint8_t)str[0][0];
  ++str[0];
  --len[0];

  if ((fourth & 0xC0u) != 0x80u) {
    goto invalid;
  }

  if ((first & 0xF8u) == 0xF0u) {
    return (nt_cp_t){((first & 0x0Fu) << 18u) | ((second & 0x3Fu) << 12u) |
                     ((third & 0x3Fu) << 6u) | (fourth & 0x3Fu)};
  }

invalid:
  return (nt_cp_t){UINT32_MAX};
}

void nt_utf8_write(char **str, size_t *len, nt_cp_t cp) {
  int req_len = nt_utf8_len(cp);
  if (len[0] < (size_t)req_len) {
    return;
  }

  switch (req_len) {
  case 1:
    str[0][0] = (char)cp.code_point;
    break;
  case 2:
    str[0][0] = (char)((cp.code_point >> 6u) | 0xC0u);
    str[0][1] = (char)((cp.code_point & 0x3Fu) | 0x80u);
    break;
  case 3:
    str[0][0] = (char)((cp.code_point >> 12u) | 0xE0u);
    str[0][1] = (char)(((cp.code_point >> 6u) & 0x3Fu) | 0x80u);
    str[0][2] = (char)((cp.code_point & 0x3Fu) | 0x80u);
    break;
  case 4:
    str[0][0] = (char)((cp.code_point >> 18u) | 0xF0u);
    str[0][1] = (char)(((cp.code_point >> 12u) & 0x3Fu) | 0x80u);
    str[0][2] = (char)(((cp.code_point >> 6u) & 0x3Fu) | 0x80u);
    str[0][3] = (char)((cp.code_point & 0x3Fu) | 0x80u);
    break;
  default:
    break;
  }

  str[0] += req_len;
  len[0] -= req_len;
}

static const uint8_t utf8_bom[NT_UTF8_BOM_LEN] = {0xEFu, 0xBBu, 0xBFu};

bool nt_utf8_has_bom(char const *str, size_t len) {
  return len >= NT_UTF8_BOM_LEN && memcmp(utf8_bom, str, NT_UTF8_BOM_LEN) == 0;
}

void nt_utf8_write_bom(char *str) { memcpy(str, utf8_bom, NT_UTF8_BOM_LEN); }
