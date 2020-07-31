#ifndef NTUTILS_CIRCULAR_H
#define NTUTILS_CIRCULAR_H

#include "nt/mem.h"
#include <stdlib.h>

#define NT_CIRC_BUF(name, item_type)                                           \
  typedef struct {                                                             \
    item_type *P_items;                                                        \
    size_t P_capacity;                                                         \
    size_t P_first;                                                            \
    size_t P_count;                                                            \
  } name##_t;                                                                  \
                                                                               \
  static inline name##_t name##_new(size_t capacity) {                         \
    return (name##_t){nt_alloc(item_type, capacity), capacity, 0, 0};          \
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
  static inline void name##_push(name##_t *self, item_type item) {             \
    P_nt_circ_buf_push(self, &item, sizeof(item_type));                        \
  }                                                                            \
                                                                               \
  static inline void name##_push_ref(name##_t *self, item_type const *item) {  \
    P_nt_circ_buf_push(self, item, sizeof(item_type));                         \
  }                                                                            \
                                                                               \
  static inline void name##_pop(name##_t *self, item_type *item) {             \
    P_nt_circ_buf_pop(self, item, sizeof(item_type));                          \
  }                                                                            \
                                                                               \
  static inline item_type const *name##_get(name##_t const *self,              \
                                            size_t index) {                    \
    return P_nt_circ_buf_get(self, index, sizeof(item_type));                  \
  }                                                                            \
                                                                               \
  static inline void name##_clear(name##_t *self) { self->P_count = 0; }

void P_nt_circ_buf_push(void *self, void const *item, size_t item_size);

void P_nt_circ_buf_pop(void *self, void *item, size_t item_size);

void const *P_nt_circ_buf_get(void const *self, size_t index, size_t item_size);

#endif // NTUTILS_CIRCULAR_H
