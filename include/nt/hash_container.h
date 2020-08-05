#ifndef NTUTILS_HASH_CONTAINER_H
#define NTUTILS_HASH_CONTAINER_H

#include "nt/compare.h"
#include "nt/hash.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct {
  uint32_t prev;
  uint32_t next;
  uint32_t hash;
  bool used;
} P_nt_hash_container_entry_hdr_t;

typedef struct {
  uint32_t P_entry_capacity;
  uint32_t P_entry_count;
  size_t P_key_size;
  size_t P_entry_size;
  uint32_t P_key_off;
  uint32_t P_free_entries_start;
  uint32_t P_buckets_capacity;
  nt_hasher_t P_hasher;
  nt_comparator_t(P_comparator, void const *);
  uint8_t *P_entries;
  uint32_t *P_buckets;
} P_nt_hash_container_t;

void P_nt_hash_container_reserve(P_nt_hash_container_t *self, size_t amount);

bool P_nt_hash_container_add(P_nt_hash_container_t *self, void const *key,
                             P_nt_hash_container_entry_hdr_t **entry);

bool P_nt_hash_container_remove(P_nt_hash_container_t *self, void const *key);

void P_nt_hash_container_clear(P_nt_hash_container_t *self);

P_nt_hash_container_entry_hdr_t const *
P_nt_hash_container_get(P_nt_hash_container_t const *self, void const *key);

typedef struct {
  P_nt_hash_container_t const *P_container;
  void const *P_key;
  uint32_t P_index;
  uint32_t P_hash;
} P_nt_hash_container_iterator_t;

P_nt_hash_container_iterator_t
P_nt_hash_container_iterate(P_nt_hash_container_t const *self, void const *key);

void const *
P_nt_hash_container_iterator_next(P_nt_hash_container_iterator_t *self);

#endif // NTUTILS_HASH_CONTAINER_H
