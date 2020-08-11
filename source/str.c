#include "nt/str.h"
#include "nt/assert.h"
#include "nt/mem.h"
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define LONG_BIT (((size_t)1) << (sizeof(size_t) * 8 - 1))

static inline bool is_short(nt_str_t const *self) {
  return (self->P_len_and_short & LONG_BIT) == 0;
}

static inline void set_short(nt_str_t *self, bool s) {
  if (s) {
    self->P_len_and_short &= ~LONG_BIT;
  } else {
    self->P_len_and_short |= LONG_BIT;
  }
}

static inline size_t get_short_len(nt_str_t const *self) {
  return sizeof(nt_str_t) -
         ((uint8_t *)&self->P_len_and_short)[sizeof(size_t) - 1] - 1;
}

static inline void set_short_len(nt_str_t *self, size_t len) {
  ((uint8_t *)&self->P_len_and_short)[sizeof(size_t) - 1] =
      sizeof(nt_str_t) - 1 - len;
}

static inline size_t get_long_len(nt_str_t const *self) {
  return self->P_len_and_short & (((size_t)0xFFFFFFFFFFFFFFFFull) >> 1u);
}

static inline void set_long_len(nt_str_t *self, size_t len) {
  self->P_len_and_short = (self->P_len_and_short & LONG_BIT) | len;
}
#else
static inline bool is_short(nt_str_t const *self) {
  return (self->P_len_and_short & 1ull) == 0;
}

static inline void set_short(nt_str_t *self, bool s) {
  if (s) {
    self->P_len_and_short &= ~((size_t)1);
  } else {
    self->P_len_and_short |= 1u;
  }
}

static inline size_t get_short_len(nt_str_t const *self) {
  return sizeof(nt_str_t) -
         (((uint8_t *)&self->P_len_and_short)[sizeof(size_t) - 1] >> 1u) - 1;
}

static inline void set_short_len(nt_str_t *self, size_t len) {
  ((uint8_t *)&self->P_len_and_short)[sizeof(size_t) - 1] =
      (sizeof(nt_str_t) - 1 - len) << 1u;
}

static inline size_t get_long_len(nt_str_t const *self) {
  return self->P_len_and_short >> 1u;
}

static inline void set_long_len(nt_str_t *self, size_t len) {
  self->P_len_and_short = (self->P_len_and_short & 1u) | (len << 1u);
}
#endif

#define SHORT_CAPACITY (sizeof(nt_str_t) - 1)

static inline void set_len(nt_str_t *self, size_t len) {
  if (is_short(self)) {
    set_short_len(self, len);
  } else {
    set_long_len(self, len);
  }
}

nt_str_t P_nt_str_new_cstr(nt_cstr_t src) { return P_nt_str_of_cstr(src, 1); }

nt_str_t P_nt_str_new_str(nt_str_t const *src) {
  return P_nt_str_of_str(src, 1);
}

nt_str_t nt_str_empty() {
  nt_str_t str;
  set_short(&str, true);
  set_short_len(&str, 0);
  return str;
}

nt_str_t P_nt_str_of_cstr(nt_cstr_t src, size_t count) {
  nt_str_t str;
  size_t total_len = src.len * count;

  if (total_len <= SHORT_CAPACITY) {
    set_short(&str, true);
    set_short_len(&str, total_len);
  } else {
    set_short(&str, false);
    set_long_len(&str, total_len);

    str.P_capacity = total_len;
    str.P_data = nt_alloc(char, total_len + 1);
  }

  char *ptr = (char *)nt_str_str(&str);
  if (total_len > 0) {
    for (size_t i = 0; i < count; ++i) {
      memcpy(ptr, src.str, src.len);
      ptr += src.len;
    }
  }
  *ptr = 0;

  return str;
}

nt_str_t P_nt_str_of_str(nt_str_t const *src, size_t count) {
  return P_nt_str_of_cstr(nt_cstr(nt_str_str(src), nt_str_len(src)), count);
}

void nt_str_free(nt_str_t *self) {
  if (!is_short(self)) {
    nt_free(self->P_data);
  }
}

size_t nt_str_len(nt_str_t const *self) {
  if (is_short(self)) {
    return get_short_len(self);
  }

  return get_long_len(self);
}

size_t nt_str_capacity(nt_str_t const *self) {
  if (is_short(self)) {
    return SHORT_CAPACITY;
  }

  return self->P_capacity;
}

char const *nt_str_str(nt_str_t const *self) {
  if (is_short(self)) {
    return (char const *)self;
  }

  return self->P_data;
}

bool P_nt_str_equal_cstr(nt_str_t const *self, nt_cstr_t other) {
  return nt_str_len(self) == other.len &&
         memcmp(nt_str_str(self), other.str, other.len) == 0;
}

bool P_nt_str_equal_str(nt_str_t const *self, nt_str_t const *other) {
  return P_nt_str_equal_cstr(self,
                             nt_cstr(nt_str_str(other), nt_str_len(other)));
}

static size_t align_capacity(size_t n) {
  if (n <= 4) {
    return 4;
  }

  if (sizeof(size_t) == 4) {
    return 1u << (32u - __builtin_clz(n - 1));
  }

  return 1ull << (64u - __builtin_clzll(n - 1));
}

void nt_str_reserve(nt_str_t *self, size_t amount) {
  size_t old_len = nt_str_len(self);
  size_t new_capacity = align_capacity(old_len + amount);

  if (new_capacity <= nt_str_capacity(self)) {
    return;
  }

  if (is_short(self)) {
    char *data = nt_alloc(char, new_capacity + 1);

    if (old_len > 0) {
      memcpy(data, self, old_len + 1);
    }

    self->P_data = data;
    self->P_capacity = new_capacity;
    set_short(self, false);
    set_long_len(self, old_len);
  } else {
    self->P_data = nt_realloc(self->P_data, new_capacity + 1);
    self->P_capacity = new_capacity;
  }
}

void nt_str_shrink(nt_str_t *self) {
  if (is_short(self)) {
    return;
  }

  size_t len = nt_str_len(self);
  if (len == self->P_capacity) {
    return;
  }

  if (len <= SHORT_CAPACITY) {
    char *data = self->P_data;
    set_short(self, true);
    set_short_len(self, len);
    memcpy(self, data, len + 1);
    nt_free(data);
  } else {
    self->P_data = nt_realloc(self->P_data, len + 1);
    self->P_capacity = len;
  }
}

void P_nt_str_append_cstr(nt_str_t *self, nt_cstr_t src) {
  if (src.len == 0) {
    return;
  }

  nt_str_reserve(self, src.len);
  size_t old_len = nt_str_len(self);
  size_t new_len = old_len + src.len;
  char *data = (char *)nt_str_str(self);

  memcpy(data + old_len, src.str, src.len);
  data[new_len] = 0;
  set_len(self, new_len);
}

void P_nt_str_append_str(nt_str_t *self, nt_str_t const *src) {
  P_nt_str_append_cstr(self, nt_cstr(nt_str_str(src), nt_str_len(src)));
}

void P_nt_str_append_int(nt_str_t *self, int64_t src) {
  nt_str_append_format(self, "%" PRId64, src);
}

void P_nt_str_append_uint(nt_str_t *self, uint64_t src) {
  nt_str_append_format(self, "%" PRIu64, src);
}

void P_nt_str_append_double(nt_str_t *self, double src) {
  nt_str_append_format(self, "%g", src);
}

nt_str_t nt_str_format(char const *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  nt_str_t str = nt_str_empty();
  nt_str_append_vformat(&str, fmt, args);
  va_end(args);
  return str;
}

nt_str_t nt_str_vformat(char const *fmt, va_list args) {
  nt_str_t str = nt_str_empty();
  nt_str_append_vformat(&str, fmt, args);
  return str;
}

void nt_str_append_format(nt_str_t *self, char const *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  nt_str_append_vformat(self, fmt, args);
  va_end(args);
}

void nt_str_append_vformat(nt_str_t *self, char const *fmt, va_list args) {
  va_list args_copy;
  va_copy(args_copy, args);

  int len = vsnprintf(NULL, 0, fmt, args);
  nt_assert(len > 0);

  nt_str_reserve(self, len);
  char *data = (char *)nt_str_str(self);
  size_t old_len = nt_str_len(self);
  size_t new_len = old_len + len;

  vsnprintf(data + old_len, len + 1, fmt, args_copy);
  data[new_len] = 0;
  set_len(self, new_len);
}

size_t P_nt_str_find_cstr(nt_str_t const *self, nt_cstr_t sub_str,
                          size_t start) {
  size_t len = nt_str_len(self);
  char const *data = nt_str_str(self);

  if (start >= len || sub_str.len > len - start || sub_str.len == 0) {
    return NT_STR_NOT_FOUND;
  }

  char const *ptr = memmem(data + start, len - start, sub_str.str, sub_str.len);

  if (ptr) {
    return ptr - data;
  }

  return NT_STR_NOT_FOUND;
}

size_t P_nt_str_find_str(nt_str_t const *self, nt_str_t const *sub_str,
                         size_t start) {
  return P_nt_str_find_cstr(
      self, nt_cstr(nt_str_str(sub_str), nt_str_len(sub_str)), start);
}

size_t P_nt_str_rfind_cstr(nt_str_t const *self, nt_cstr_t sub_str,
                           size_t start) {
  size_t len = nt_str_len(self);
  char const *data = nt_str_str(self);

  if (sub_str.len > len || sub_str.len == 0) {
    return NT_STR_NOT_FOUND;
  }

  if (start > len - sub_str.len) {
    start = len - sub_str.len;
  }

  len = start + 1;
  char const *ptr = memrchr(data, sub_str.str[0], len);
  if (sub_str.len > 1) {
    while (ptr) {
      if (memcmp(ptr, sub_str.str, sub_str.len) == 0) {
        break;
      }

      if (ptr == data) {
        ptr = NULL;
        break;
      }

      len = ptr - data;
      ptr = memrchr(data, sub_str.str[0], len);
    }
  }

  if (ptr) {
    return ptr - data;
  }

  return NT_STR_NOT_FOUND;
}

size_t P_nt_str_rfind_str(nt_str_t const *self, nt_str_t const *sub_str,
                          size_t start) {
  return P_nt_str_rfind_cstr(
      self, nt_cstr(nt_str_str(sub_str), nt_str_len(sub_str)), start);
}

nt_str_t P_nt_str_slice(nt_str_t const *self, size_t start, size_t end) {
  size_t len = nt_str_len(self);

  nt_assert(start < len);

  if (end >= len) {
    end = len - 1;
  }

  nt_assert(start <= end);

  return nt_str_new(nt_cstr(nt_str_str(self) + start, end - start + 1));
}
