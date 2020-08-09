#ifndef NTUTILS_MAP_H
#define NTUTILS_MAP_H

#include "nt/compare.h"
#include "nt/hash.h"
#include "nt/hash_container.h"
#include "nt/mem.h"
#include <stddef.h>

#define NT_MAP(name, key_type, value_type)                                     \
  typedef struct {                                                             \
    P_nt_hash_container_entry_hdr_t hdr;                                       \
    key_type key;                                                              \
    value_type value;                                                          \
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
    size_t P_value_size;                                                       \
    uint32_t P_value_off;                                                      \
  } name##_t;                                                                  \
                                                                               \
  typedef struct {                                                             \
    P_nt_hash_container_iterator_t itr;                                        \
  } name##_iterator_t;                                                         \
                                                                               \
  typedef struct {                                                             \
    key_type const *key;                                                       \
    value_type *value;                                                         \
  } name##_pair_t;                                                             \
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
                      NULL,                                                    \
                      sizeof(value_type),                                      \
                      offsetof(P_##name##_entry_t, value)};                    \
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
  static inline bool name##_add(name##_t *self, key_type key,                  \
                                value_type value) {                            \
    return P_nt_map_add(self, &key, &value);                                   \
  }                                                                            \
                                                                               \
  static inline bool name##_add_ref(name##_t *self, key_type const *key,       \
                                    value_type const *value) {                 \
    return P_nt_map_add(self, key, value);                                     \
  }                                                                            \
                                                                               \
  static inline void name##_put(name##_t *self, key_type key,                  \
                                value_type value) {                            \
    P_nt_map_put(self, &key, &value);                                          \
  }                                                                            \
                                                                               \
  static inline void name##_put_ref(name##_t *self, key_type const *key,       \
                                    value_type const *value) {                 \
    P_nt_map_put(self, key, value);                                            \
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
  static inline value_type const *name##_get(name##_t const *self,             \
                                             key_type key) {                   \
    return P_nt_map_get(self, &key);                                           \
  }                                                                            \
                                                                               \
  static inline value_type const *name##_get_ref(name##_t const *self,         \
                                                 key_type const *key) {        \
    return P_nt_map_get(self, key);                                            \
  }                                                                            \
                                                                               \
  static inline name##_iterator_t name##_iterate(name##_t const *self,         \
                                                 key_type const *key) {        \
    return (name##_iterator_t){P_nt_hash_container_iterate(                    \
        (P_nt_hash_container_t const *)self, key)};                            \
  }                                                                            \
                                                                               \
  static inline name##_pair_t name##_iterator_next(name##_iterator_t *self) {  \
    name##_t const *container = (name##_t const *)self->itr.P_container;       \
    key_type const *key =                                                      \
        (key_type const *)P_nt_hash_container_iterator_next(&self->itr);       \
    value_type *value =                                                        \
        key ? (value_type *)(((uint8_t *)key) - container->P_key_off +         \
                             container->P_value_off)                           \
            : NULL;                                                            \
                                                                               \
    return (name##_pair_t){key, value};                                        \
  }

bool P_nt_map_add(void *self, void const *key, void const *value);

void P_nt_map_put(void *self, void const *key, void const *value);

void const *P_nt_map_get(void const *self, void const *key);

#endif // NTUTILS_MAP_H
