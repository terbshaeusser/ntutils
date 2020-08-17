#ifndef NTUTILS_QUEUE_H
#define NTUTILS_QUEUE_H

#include "nt/mem.h"
#include <stdint.h>

typedef struct {
  uint32_t prev;
  uint32_t next;
} P_nt_queue_entry_hdr_t;

#define NT_QUEUE(name, value_type)                                             \
  typedef struct {                                                             \
    P_nt_queue_entry_hdr_t hdr;                                                \
    value_type value;                                                          \
  } P_##name##_entry_t;                                                        \
                                                                               \
  typedef struct {                                                             \
    size_t P_capacity;                                                         \
    size_t P_count;                                                            \
    size_t P_value_size;                                                       \
    size_t P_entity_size;                                                      \
    uint32_t P_value_off;                                                      \
    uint32_t P_first_used;                                                     \
    uint32_t P_last_used;                                                      \
    uint32_t P_first_free;                                                     \
    uint8_t *P_entries;                                                        \
  } name##_t;                                                                  \
                                                                               \
  typedef struct {                                                             \
    name##_t const *P_container;                                               \
    uint32_t P_index;                                                          \
  } name##_iterator_t;                                                         \
                                                                               \
  static inline name##_t name##_new() {                                        \
    return (name##_t){0,                                                       \
                      0,                                                       \
                      sizeof(value_type),                                      \
                      sizeof(P_##name##_entry_t),                              \
                      offsetof(P_##name##_entry_t, value),                     \
                      0,                                                       \
                      0,                                                       \
                      0,                                                       \
                      NULL};                                                   \
  }                                                                            \
                                                                               \
  static inline void name##_free(name##_t *self) { nt_free(self->P_entries); } \
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
    P_nt_queue_reserve(self, amount);                                          \
  }                                                                            \
                                                                               \
  static inline void name##_add(name##_t *self, value_type value) {            \
    P_nt_queue_add(self, &value);                                              \
  }                                                                            \
                                                                               \
  static inline void name##_add_ref(name##_t *self, value_type const *value) { \
    P_nt_queue_add(self, value);                                               \
  }                                                                            \
                                                                               \
  static inline value_type name##_peek(name##_t const *self) {                 \
    return *(value_type const *)P_nt_queue_peek(self);                         \
  }                                                                            \
                                                                               \
  static inline value_type const *name##_peek_ref(name##_t const *self) {      \
    return P_nt_queue_peek(self);                                              \
  }                                                                            \
                                                                               \
  static inline value_type name##_take(name##_t *self) {                       \
    value_type value;                                                          \
    P_nt_queue_take(self, &value);                                             \
    return value;                                                              \
  }                                                                            \
                                                                               \
  static inline void name##_take_ref(name##_t *self, value_type *dst) {        \
    P_nt_queue_take(self, dst);                                                \
  }                                                                            \
                                                                               \
  static inline void name##_clear(name##_t *self) { P_nt_queue_clear(self); }  \
                                                                               \
  static inline name##_iterator_t name##_iterate(name##_t const *self) {       \
    return (name##_iterator_t){self, self->P_first_used};                      \
  }                                                                            \
                                                                               \
  static inline value_type const *name##_iterator_next(                        \
      name##_iterator_t *self) {                                               \
    return P_nt_queue_iterator_next(self);                                     \
  }

void P_nt_queue_reserve(void *self, size_t amount);

void P_nt_queue_add(void *self, void const *value);

void const *P_nt_queue_peek(void const *self);

void P_nt_queue_take(void *self, void *dst);

void P_nt_queue_clear(void *self);

void const *P_nt_queue_iterator_next(void *self);

#endif // NTUTILS_QUEUE_H
