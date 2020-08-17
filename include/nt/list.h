#ifndef NTUTILS_LIST_H
#define NTUTILS_LIST_H

#include "nt/mem.h"
#include <stddef.h>

#define NT_LIST(name, value_type)                                              \
  typedef struct {                                                             \
    size_t P_capacity;                                                         \
    size_t P_count;                                                            \
    size_t P_value_size;                                                       \
    value_type *P_values;                                                      \
  } name##_t;                                                                  \
                                                                               \
  typedef struct {                                                             \
    name##_t const *P_container;                                               \
    size_t P_index;                                                            \
  } name##_iterator_t;                                                         \
                                                                               \
  static inline name##_t name##_new() {                                        \
    return (name##_t){0, 0, sizeof(value_type), NULL};                         \
  }                                                                            \
                                                                               \
  static inline void name##_free(name##_t *self) { nt_free(self->P_values); }  \
                                                                               \
  static inline size_t name##_capacity(name##_t const *self) {                 \
    return self->P_capacity;                                                   \
  }                                                                            \
                                                                               \
  static inline size_t name##_count(name##_t const *self) {                    \
    return self->P_count;                                                      \
  }                                                                            \
                                                                               \
  static inline void name##_reserve(name##_t *self, size_t amount) {           \
    P_nt_list_reserve(self, amount);                                           \
  }                                                                            \
                                                                               \
  static inline void name##_shrink(name##_t *self) { P_nt_list_shrink(self); } \
                                                                               \
  static inline size_t name##_add(name##_t *self, value_type value) {          \
    return P_nt_list_insert(self, self->P_count, &value);                      \
  }                                                                            \
                                                                               \
  static inline size_t name##_add_ref(name##_t *self,                          \
                                      value_type const *value) {               \
    return P_nt_list_insert(self, self->P_count, value);                       \
  }                                                                            \
                                                                               \
  static inline void name##_insert(name##_t *self, size_t index,               \
                                   value_type value) {                         \
    P_nt_list_insert(self, index, &value);                                     \
  }                                                                            \
                                                                               \
  static inline void name##_insert_ref(name##_t *self, size_t index,           \
                                       value_type const *value) {              \
    P_nt_list_insert(self, index, value);                                      \
  }                                                                            \
                                                                               \
  static inline void name##_remove(name##_t *self, size_t index) {             \
    P_nt_list_remove(self, index);                                             \
  }                                                                            \
                                                                               \
  static inline void name##_clear(name##_t *self) { self->P_count = 0; }       \
                                                                               \
  static inline value_type const *name##_get(name##_t const *self,             \
                                             size_t index) {                   \
    return P_nt_list_get(self, index);                                         \
  }                                                                            \
                                                                               \
  static inline name##_iterator_t name##_iterate(name##_t const *self) {       \
    return (name##_iterator_t){self, 0};                                       \
  }                                                                            \
                                                                               \
  static inline value_type const *name##_iterator_next(                        \
      name##_iterator_t *self) {                                               \
    return P_nt_list_iterator_next(self);                                      \
  }

void P_nt_list_reserve(void *self, size_t amount);

void P_nt_list_shrink(void *self);

size_t P_nt_list_insert(void *self, size_t index, void const *value);

void P_nt_list_remove(void *self, size_t index);

void const *P_nt_list_get(void const *self, size_t index);

void const *P_nt_list_iterator_next(void *self);

#endif // NTUTILS_LIST_H
