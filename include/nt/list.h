#ifndef NTUTILS_LIST_H
#define NTUTILS_LIST_H

#include "nt/mem.h"
#include <stdlib.h>

#define NT_LIST(name, item_type)                                               \
  typedef struct {                                                             \
    size_t P_capacity;                                                         \
    size_t P_count;                                                            \
    size_t P_item_size;                                                        \
    item_type *P_items;                                                        \
  } name##_t;                                                                  \
                                                                               \
  static inline name##_t name##_new() {                                        \
    return (name##_t){0, 0, sizeof(item_type), NULL};                          \
  }                                                                            \
                                                                               \
  static inline void name##_free(name##_t *self) { nt_free(self->P_items); }   \
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
  static inline size_t name##_add(name##_t *self, item_type item) {            \
    return P_nt_list_insert(self, self->P_count, &item);                       \
  }                                                                            \
                                                                               \
  static inline size_t name##_add_ref(name##_t *self, item_type const *item) { \
    return P_nt_list_insert(self, self->P_count, item);                        \
  }                                                                            \
                                                                               \
  static inline void name##_insert(name##_t *self, size_t index,               \
                                   item_type item) {                           \
    P_nt_list_insert(self, index, &item);                                      \
  }                                                                            \
                                                                               \
  static inline void name##_insert_ref(name##_t *self, size_t index,           \
                                       item_type const *item) {                \
    P_nt_list_insert(self, index, item);                                       \
  }                                                                            \
                                                                               \
  static inline void name##_remove(name##_t *self, size_t index) {             \
    P_nt_list_remove(self, index);                                             \
  }                                                                            \
                                                                               \
  static inline void name##_clear(name##_t *self) { self->P_count = 0; }       \
                                                                               \
  static inline item_type const *name##_get(name##_t const *self,              \
                                            size_t index) {                    \
    return P_nt_list_get(self, index);                                         \
  }

void P_nt_list_reserve(void *self, size_t amount);

void P_nt_list_shrink(void *self);

size_t P_nt_list_insert(void *self, size_t index, void const *item);

void P_nt_list_remove(void *self, size_t index);

void const *P_nt_list_get(void const *self, size_t index);

#endif // NTUTILS_LIST_H
