#ifndef NTUTILS_CIRCULAR_H
#define NTUTILS_CIRCULAR_H

#include "nt/mem.h"
#include <stdlib.h>

#define NT_CIRC_BUF(...)                                                       \
  NT_GET_ARG3(__VA_ARGS__, P_NT_CIRC_BUF_FX, P_NT_CIRC_BUF_DYN)(__VA_ARGS__)

#define P_NT_CIRC_BUF_DYN(name, item_type)                                     \
  typedef struct {                                                             \
    size_t P_first;                                                            \
    size_t P_count;                                                            \
    size_t P_capacity;                                                         \
    item_type *P_items;                                                        \
  } name##_t;                                                                  \
                                                                               \
  static inline name##_t name##_new(size_t capacity) {                         \
    return (name##_t){0, 0, capacity, nt_alloc(item_type, capacity)};          \
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
    P_nt_circ_buf_dyn_push(self, &item, sizeof(item_type));                    \
  }                                                                            \
                                                                               \
  static inline void name##_push_ref(name##_t *self, item_type const *item) {  \
    P_nt_circ_buf_dyn_push(self, item, sizeof(item_type));                     \
  }                                                                            \
                                                                               \
  static inline void name##_pop(name##_t *self, item_type *item) {             \
    P_nt_circ_buf_dyn_pop(self, item, sizeof(item_type));                      \
  }                                                                            \
                                                                               \
  static inline item_type const *name##_get(name##_t const *self,              \
                                            size_t index) {                    \
    return P_nt_circ_buf_dyn_get(self, index, sizeof(item_type));              \
  }                                                                            \
                                                                               \
  static inline void name##_clear(name##_t *self) { self->P_count = 0; }

void P_nt_circ_buf_dyn_push(void *self, void const *item, size_t item_size);

void P_nt_circ_buf_dyn_pop(void *self, void *item, size_t item_size);

void const *P_nt_circ_buf_dyn_get(void const *self, size_t index,
                                  size_t item_size);

#define P_NT_CIRC_BUF_FX(name, item_type, buf_capacity)                        \
  typedef struct {                                                             \
    size_t P_first;                                                            \
    size_t P_count;                                                            \
    item_type P_items[buf_capacity];                                           \
  } name##_t;                                                                  \
                                                                               \
  static inline name##_t name##_new() { return (name##_t){0}; }                \
                                                                               \
  static inline void name##_free(name##_t *self) { (void)self; }               \
                                                                               \
  static inline size_t name##_capacity(name##_t const *self) {                 \
    (void)self;                                                                \
    return buf_capacity;                                                       \
  }                                                                            \
                                                                               \
  static inline size_t name##_count(name##_t const *self) {                    \
    return self->P_count;                                                      \
  }                                                                            \
                                                                               \
  static inline void name##_push(name##_t *self, item_type item) {             \
    P_nt_circ_buf_fx_push(self, buf_capacity, &item, sizeof(item_type));       \
  }                                                                            \
                                                                               \
  static inline void name##_push_ref(name##_t *self, item_type const *item) {  \
    P_nt_circ_buf_fx_push(self, buf_capacity, item, sizeof(item_type));        \
  }                                                                            \
                                                                               \
  static inline void name##_pop(name##_t *self, item_type *item) {             \
    P_nt_circ_buf_fx_pop(self, buf_capacity, item, sizeof(item_type));         \
  }                                                                            \
                                                                               \
  static inline item_type const *name##_get(name##_t const *self,              \
                                            size_t index) {                    \
    return P_nt_circ_buf_fx_get(self, buf_capacity, index, sizeof(item_type)); \
  }                                                                            \
                                                                               \
  static inline void name##_clear(name##_t *self) { self->P_count = 0; }

void P_nt_circ_buf_fx_push(void *self, size_t capacity, void const *item,
                           size_t item_size);

void P_nt_circ_buf_fx_pop(void *self, size_t capacity, void *item,
                          size_t item_size);

void const *P_nt_circ_buf_fx_get(void const *self, size_t capacity,
                                 size_t index, size_t item_size);

#endif // NTUTILS_CIRCULAR_H
