#ifndef NTUTILS_CIRCULAR_H
#define NTUTILS_CIRCULAR_H

#include "nt/mem.h"
#include <stddef.h>

#define NT_CIRC_BUF(...)                                                       \
  NT_GET_ARG3(__VA_ARGS__, P_NT_CIRC_BUF_FX, P_NT_CIRC_BUF_DYN)(__VA_ARGS__)

#define P_NT_CIRC_BUF_DYN(name, value_type)                                    \
  typedef struct {                                                             \
    size_t P_first;                                                            \
    size_t P_count;                                                            \
    size_t P_capacity;                                                         \
    size_t P_value_size;                                                       \
    value_type *P_values;                                                      \
  } name##_t;                                                                  \
                                                                               \
  typedef struct {                                                             \
    name##_t const *P_container;                                               \
    size_t P_index;                                                            \
  } name##_iterator_t;                                                         \
                                                                               \
  static inline name##_t name##_new(size_t capacity) {                         \
    return (name##_t){0, 0, capacity, sizeof(value_type),                      \
                      nt_alloc(value_type, capacity)};                         \
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
  static inline void name##_push(name##_t *self, value_type value) {           \
    P_nt_circ_buf_dyn_push(self, &value);                                      \
  }                                                                            \
                                                                               \
  static inline void name##_push_ref(name##_t *self,                           \
                                     value_type const *value) {                \
    P_nt_circ_buf_dyn_push(self, value);                                       \
  }                                                                            \
                                                                               \
  static inline void name##_pop(name##_t *self, value_type *value) {           \
    P_nt_circ_buf_dyn_pop(self, value);                                        \
  }                                                                            \
                                                                               \
  static inline value_type const *name##_get(name##_t const *self,             \
                                             size_t index) {                   \
    return P_nt_circ_buf_dyn_get(self, index);                                 \
  }                                                                            \
                                                                               \
  static inline void name##_clear(name##_t *self) { self->P_count = 0; }       \
                                                                               \
  static inline name##_iterator_t name##_iterate(name##_t const *self) {       \
    return (name##_iterator_t){self, 0};                                       \
  }                                                                            \
                                                                               \
  static inline value_type const *name##_iterator_next(                        \
      name##_iterator_t *self) {                                               \
    return P_nt_circ_buf_dyn_iterator_next(self);                              \
  }

void P_nt_circ_buf_dyn_push(void *self, void const *value);

void P_nt_circ_buf_dyn_pop(void *self, void *value);

void const *P_nt_circ_buf_dyn_get(void const *self, size_t index);

void const *P_nt_circ_buf_dyn_iterator_next(void *self);

#define P_NT_CIRC_BUF_FX(name, value_type, buf_capacity)                       \
  typedef struct {                                                             \
    size_t P_first;                                                            \
    size_t P_count;                                                            \
    size_t P_value_size;                                                       \
    value_type P_values[buf_capacity];                                         \
  } name##_t;                                                                  \
                                                                               \
  typedef struct {                                                             \
    name##_t const *P_container;                                               \
    size_t P_index;                                                            \
  } name##_iterator_t;                                                         \
                                                                               \
  static inline name##_t name##_new() {                                        \
    return (name##_t){                                                         \
        .P_first = 0, .P_count = 0, .P_value_size = sizeof(value_type)};       \
  }                                                                            \
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
  static inline void name##_push(name##_t *self, value_type value) {           \
    P_nt_circ_buf_fx_push(self, buf_capacity, &value);                         \
  }                                                                            \
                                                                               \
  static inline void name##_push_ref(name##_t *self,                           \
                                     value_type const *value) {                \
    P_nt_circ_buf_fx_push(self, buf_capacity, value);                          \
  }                                                                            \
                                                                               \
  static inline void name##_pop(name##_t *self, value_type *value) {           \
    P_nt_circ_buf_fx_pop(self, buf_capacity, value);                           \
  }                                                                            \
                                                                               \
  static inline value_type const *name##_get(name##_t const *self,             \
                                             size_t index) {                   \
    return P_nt_circ_buf_fx_get(self, buf_capacity, index);                    \
  }                                                                            \
                                                                               \
  static inline void name##_clear(name##_t *self) { self->P_count = 0; }       \
                                                                               \
  static inline name##_iterator_t name##_iterate(name##_t const *self) {       \
    return (name##_iterator_t){self, 0};                                       \
  }                                                                            \
                                                                               \
  static inline value_type const *name##_iterator_next(                        \
      name##_iterator_t *self) {                                               \
    return P_nt_circ_buf_iterator_next(self, buf_capacity);                    \
  }

void P_nt_circ_buf_fx_push(void *self, size_t capacity, void const *value);

void P_nt_circ_buf_fx_pop(void *self, size_t capacity, void *value);

void const *P_nt_circ_buf_fx_get(void const *self, size_t capacity,
                                 size_t index);

void const *P_nt_circ_buf_iterator_next(void *self, size_t capacity);

#endif // NTUTILS_CIRCULAR_H
