#include "nt/utf8.h"
#include <string.h>

#ifdef _WIN32
#include "nt/assert.h"
#include <windows.h>

#define thread_local __thread
#endif

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

static bool is_multibyte_w_len(char const *str, size_t len) {
  while (len > 0) {
    if (((uint8_t)*str) > 0x7Fu) {
      return true;
    }

    ++str;
    --len;
  }

  return false;
}

static bool is_multibyte_wo_len(char const *str) {
  while (true) {
    uint8_t c = (uint8_t)*str;

    if (c == 0) {
      return false;
    }
    if (c > 0x7Fu) {
      return true;
    }

    ++str;
  }
}

bool P_nt_utf8_is_multibyte(char const *str, size_t len) {
  if (len == (size_t)0xFFFFFFFFFFFFFFFFull) {
    return is_multibyte_wo_len(str);
  }

  return is_multibyte_w_len(str, len);
}

#ifdef _WIN32
#ifndef WC_ERR_INVALID_CHARS
#define WC_ERR_INVALID_CHARS 0x0080
#endif

size_t P_nt_utf8_to_wide(wchar_t *dst, size_t dst_len, char const *src,
                         size_t src_len) {
  bool no_src_len = src_len == 0;
  int written =
      MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, src,
                          no_src_len ? -1 : (int)src_len, dst, dst_len);

  return (size_t)written - (no_src_len ? 1 : 0);
}

size_t P_nt_utf8_from_wide(char *dst, size_t dst_len, wchar_t const *src,
                           size_t src_len) {
  bool no_src_len = src_len == 0;
  int written = WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, src,
                                    no_src_len ? -1 : (int)src_len, dst,
                                    dst_len, NULL, NULL);

  return (size_t)written - (no_src_len ? 1 : 0);
}
#endif
