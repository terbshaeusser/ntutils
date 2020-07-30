#include "nt/mem.h"
#include "nt/assert.h"
#include <stdlib.h>

__attribute((weak)) void *P_nt_custom_malloc(size_t element_size,
                                             size_t count) {
  return malloc(element_size * count);
}

__attribute((weak)) void *P_nt_custom_calloc(size_t element_size,
                                             size_t count) {
  return calloc(element_size, count);
}

__attribute((weak)) void *P_nt_custom_realloc(void *ptr, size_t element_size,
                                              size_t new_count) {
  return realloc(ptr, element_size * new_count);
}

__attribute((weak)) void P_nt_custom_free(void *ptr) { return free(ptr); }

void *P_nt_try_alloc(size_t element_size, size_t count, bool clear) {
  nt_assert(element_size > 0);
  nt_assert(count > 0);

  if (clear) {
    return P_nt_custom_calloc(element_size, count);
  }

  return P_nt_custom_malloc(element_size, count);
}

void *P_nt_alloc(size_t element_size, size_t count, bool clear) {
  void *ptr = P_nt_try_alloc(element_size, count, clear);
  nt_assert(ptr);
  return ptr;
}

void *P_nt_try_realloc(void *ptr, size_t element_size, size_t new_count) {
  nt_assert(element_size > 0);
  nt_assert(new_count > 0);

  return P_nt_custom_realloc(ptr, element_size, new_count);
}

void *P_nt_realloc(void *ptr, size_t element_size, size_t new_count) {
  void *new_ptr = P_nt_try_realloc(ptr, element_size, new_count);
  nt_assert(new_ptr);
  return new_ptr;
}

void nt_free(void *ptr) { P_nt_custom_free(ptr); }
