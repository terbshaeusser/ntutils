#ifndef NTUTILS_MEM_H
#define NTUTILS_MEM_H

#include "nt/macros.h"
#include <stdbool.h>
#include <stddef.h>

#define nt_try_alloc(type, ...)                                                \
  (type *)P_nt_try_alloc(sizeof(type), NT_GET_ARG0_OR_DEFAULT(1, __VA_ARGS__), \
                         NT_GET_ARG1_OR_DEFAULT(false, __VA_ARGS__))

void *P_nt_try_alloc(size_t element_size, size_t count, bool clear);

#define nt_alloc(type, ...)                                                    \
  (type *)P_nt_alloc(sizeof(type), NT_GET_ARG0_OR_DEFAULT(1, __VA_ARGS__),     \
                     NT_GET_ARG1_OR_DEFAULT(false, __VA_ARGS__))

void *P_nt_alloc(size_t element_size, size_t count, bool clear);

#define nt_try_realloc(ptr, new_count)                                         \
  (typeof(type)) P_nt_try_realloc(ptr, sizeof(typeof(*(ptr))), new_count)

void *P_nt_try_realloc(void *ptr, size_t element_size, size_t new_count);

#define nt_realloc(ptr, new_count)                                             \
  (typeof(ptr)) P_nt_realloc(ptr, sizeof(typeof(*(ptr))), new_count)

void *P_nt_realloc(void *ptr, size_t element_size, size_t new_count);

void nt_free(void *ptr);

#endif // NTUTILS_MEM_H
