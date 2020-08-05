#ifndef NTUTILS_SET_H
#define NTUTILS_SET_H

#include "nt/compare.h"
#include "nt/hash.h"
#include "nt/hash_container.h"
#include "nt/mem.h"
#include <stddef.h>

#define NT_SET(name, key_type)                                                 \
  typedef struct {                                                             \
    P_nt_hash_container_entry_hdr_t hdr;                                       \
    key_type key;                                                              \
  } P_##name##_entry_t;                                                        \
                                                                               \
  typedef struct {                                                             \
    uint32_t P_entry_capacity;                                                 \
    uint32_t P_entry_count;                                                    \
    size_t P_key_size;                                                         \
    size_t P_entry_size;                                                       \
    uint32_t P_key_off;                                                        \
    uint32_t P_free_entries_start;                                             \
    uint32_t P_buckets_capacity;                                               \
    uint32_t P_buckets_count;                                                  \
    nt_hasher_t P_hasher;                                                      \
    nt_comparator_t(P_comparator, key_type const *);                           \
    uint8_t *P_entries;                                                        \
    uint32_t *P_buckets;                                                       \
  } name##_t;                                                                  \
                                                                               \
  typedef struct {                                                             \
    P_nt_hash_container_iterator_t itr;                                        \
  } name##_iterator_t;                                                         \
                                                                               \
  static inline name##_t name##_new(                                           \
      nt_hasher_t hasher, nt_comparator_t(comparator, key_type const *)) {     \
    return (name##_t){0,                                                       \
                      0,                                                       \
                      sizeof(key_type),                                        \
                      sizeof(P_##name##_entry_t),                              \
                      offsetof(P_##name##_entry_t, key),                       \
                      0,                                                       \
                      0,                                                       \
                      0,                                                       \
                      hasher,                                                  \
                      comparator,                                              \
                      NULL,                                                    \
                      NULL};                                                   \
  }                                                                            \
                                                                               \
  static inline void name##_free(name##_t *self) { nt_free(self->P_entries); } \
                                                                               \
  static inline size_t name##_count(name##_t const *self) {                    \
    return self->P_entry_count;                                                \
  }                                                                            \
                                                                               \
  static inline void name##_reserve(name##_t *self, size_t amount) {           \
    P_nt_hash_container_reserve((P_nt_hash_container_t *)self, amount);        \
  }                                                                            \
                                                                               \
  static inline bool name##_add(name##_t *self, key_type key) {                \
    P_nt_hash_container_entry_hdr_t *entry;                                    \
    return P_nt_hash_container_add((P_nt_hash_container_t *)self, &key,        \
                                   &entry);                                    \
  }                                                                            \
                                                                               \
  static inline bool name##_add_ref(name##_t *self, key_type const *key) {     \
    P_nt_hash_container_entry_hdr_t *entry;                                    \
    return P_nt_hash_container_add((P_nt_hash_container_t *)self, key,         \
                                   &entry);                                    \
  }                                                                            \
                                                                               \
  static inline bool name##_remove(name##_t *self, key_type key) {             \
    return P_nt_hash_container_remove((P_nt_hash_container_t *)self, &key);    \
  }                                                                            \
                                                                               \
  static inline bool name##_remove_ref(name##_t *self, key_type const *key) {  \
    return P_nt_hash_container_remove((P_nt_hash_container_t *)self, key);     \
  }                                                                            \
                                                                               \
  static inline void name##_clear(name##_t *self) {                            \
    P_nt_hash_container_clear((P_nt_hash_container_t *)self);                  \
  }                                                                            \
                                                                               \
  static inline bool name##_contains(name##_t const *self, key_type key) {     \
    return P_nt_hash_container_get((P_nt_hash_container_t const *)self,        \
                                   &key) != NULL;                              \
  }                                                                            \
                                                                               \
  static inline bool name##_contains_ref(name##_t const *self,                 \
                                         key_type const *key) {                \
    return P_nt_hash_container_get((P_nt_hash_container_t const *)self,        \
                                   key) != NULL;                               \
  }                                                                            \
                                                                               \
  static inline name##_iterator_t name##_iterate(name##_t const *self,         \
                                                 key_type const *key) {        \
    return (name##_iterator_t){P_nt_hash_container_iterate(                    \
        (P_nt_hash_container_t const *)self, key)};                            \
  }                                                                            \
                                                                               \
  static inline key_type const *name##_iterator_next(                          \
      name##_iterator_t *self) {                                               \
    return (key_type const *)P_nt_hash_container_iterator_next(&self->itr);    \
  }

#endif // NTUTILS_SET_H
