#include "nt/hash_container.h"
#include "nt/mem.h"
#include <string.h>

enum { NIL = 0xFFFFFFFFu };

enum { CAPACITY_COUNT = 10 };

static const uint32_t capacities[CAPACITY_COUNT] = {
    3, 7, 17, 37, 79, 163, 331, 701, 1499, 3011,
};

static uint32_t align_capacity(size_t *entry_capacity) {
  uint32_t bucket_capacity = *entry_capacity * 3 / 4;

  int i = 0;
  while (true) {
    if (i == CAPACITY_COUNT) {
      bucket_capacity = bucket_capacity * 2 - 1;
      break;
    }

    if (bucket_capacity < capacities[i]) {
      bucket_capacity = capacities[i];
      break;
    }

    ++i;
  }

  *entry_capacity = bucket_capacity * 4 / 3;
  return bucket_capacity;
}

static P_nt_hash_container_entry_hdr_t *get_entry(P_nt_hash_container_t *self,
                                                  uint32_t index) {
  return (P_nt_hash_container_entry_hdr_t *)&self
      ->P_entries[self->P_entry_size * index];
}

static uint32_t get_entry_index(P_nt_hash_container_t const *self,
                                P_nt_hash_container_entry_hdr_t const *entry) {
  return (((uint8_t const *)entry) - self->P_entries) / self->P_entry_size;
}

static void *get_key(P_nt_hash_container_t const *self,
                     P_nt_hash_container_entry_hdr_t *entry) {
  return &((uint8_t *)entry)[self->P_key_off];
}

static P_nt_hash_container_entry_hdr_t *
get_entry_and_bucket(P_nt_hash_container_t *self, void const *key,
                     uint32_t *hash, uint32_t **bucket) {
  if (self->P_entry_capacity == 0) {
    return NULL;
  }

  *hash = self->P_hasher(key, self->P_key_size);
  *bucket = &self->P_buckets[*hash % self->P_buckets_capacity];

  uint32_t next = **bucket;
  while (next != NIL) {
    P_nt_hash_container_entry_hdr_t *entry = get_entry(self, next);
    next = entry->next;

    if (entry->hash != *hash) {
      continue;
    }

    if (self->P_comparator(key, get_key(self, entry)) == NT_COMPARE_EQUAL) {
      return entry;
    }
  }

  return NULL;
}

static void mark_as_free_ex(P_nt_hash_container_t *self,
                            P_nt_hash_container_entry_hdr_t *entry,
                            uint32_t index) {
  entry->prev = NIL;
  entry->next = self->P_free_entries_start;
  entry->used = false;

  if (self->P_free_entries_start != NIL) {
    get_entry(self, self->P_free_entries_start)->prev = index;
  }

  self->P_free_entries_start = index;
}

static void mark_as_free(P_nt_hash_container_t *self,
                         P_nt_hash_container_entry_hdr_t *entry) {
  mark_as_free_ex(self, entry, get_entry_index(self, entry));
}

static void mark_as_free2(P_nt_hash_container_t *self, uint32_t index) {
  mark_as_free_ex(self, get_entry(self, index), index);
}

static uint32_t mark_as_used(P_nt_hash_container_t *self) {
  uint32_t index = self->P_free_entries_start;
  P_nt_hash_container_entry_hdr_t *entry = get_entry(self, index);

  self->P_free_entries_start = entry->next;
  if (entry->next != NIL) {
    get_entry(self, entry->next)->prev = NIL;
  }
  entry->used = true;

  return index;
}

void P_nt_hash_container_reserve(P_nt_hash_container_t *self, size_t amount) {
  size_t new_entry_capacity = self->P_entry_count + amount;
  if (new_entry_capacity <= self->P_entry_capacity) {
    return;
  }

  if (self->P_entry_capacity == 0) {
    self->P_free_entries_start = NIL;
  }

  uint32_t new_bucket_capacity = align_capacity(&new_entry_capacity);
  size_t new_size = new_entry_capacity * self->P_entry_size +
                    new_bucket_capacity * sizeof(uint32_t);
  if (self->P_entry_capacity == 0) {
    self->P_entries = nt_alloc(uint8_t, new_size);
  } else {
    self->P_entries = nt_realloc(self->P_entries, new_size);
  }

  for (uint32_t i = self->P_entry_capacity; i < new_entry_capacity; ++i) {
    mark_as_free2(self, i);
  }

  self->P_buckets =
      (uint32_t *)&self->P_entries[new_entry_capacity * self->P_entry_size];
  memset(self->P_buckets, (int)NIL, new_bucket_capacity * sizeof(uint32_t));

  for (uint32_t i = 0; i < self->P_entry_capacity; ++i) {
    P_nt_hash_container_entry_hdr_t *entry = get_entry(self, i);
    if (!entry->used) {
      continue;
    }

    uint32_t *bucket = &self->P_buckets[entry->hash % new_bucket_capacity];
    entry->prev = NIL;
    entry->next = *bucket;
    *bucket = i;
  }

  self->P_entry_capacity = new_entry_capacity;
  self->P_buckets_capacity = new_bucket_capacity;
}

bool P_nt_hash_container_add(P_nt_hash_container_t *self, void const *key,
                             P_nt_hash_container_entry_hdr_t **entry) {
  if (self->P_entry_count == self->P_entry_capacity) {
    P_nt_hash_container_reserve(self, 1);
  }

  uint32_t hash;
  uint32_t *bucket;
  *entry = get_entry_and_bucket(self, key, &hash, &bucket);

  if (*entry) {
    return false;
  }

  uint32_t entry_index = mark_as_used(self);
  *entry = get_entry(self, entry_index);
  (*entry)->prev = NIL;
  (*entry)->next = *bucket;
  (*entry)->hash = hash;
  memcpy(get_key(self, *entry), key, self->P_key_size);

  *bucket = entry_index;
  ++self->P_entry_count;
  return true;
}

bool P_nt_hash_container_remove(P_nt_hash_container_t *self, void const *key) {
  uint32_t hash;
  uint32_t *bucket;
  P_nt_hash_container_entry_hdr_t *entry =
      get_entry_and_bucket(self, key, &hash, &bucket);

  if (!entry) {
    return false;
  }

  if (entry->prev != NIL) {
    get_entry(self, entry->prev)->next = entry->next;
  } else {
    *bucket = entry->next;
  }

  if (entry->next != NIL) {
    get_entry(self, entry->next)->prev = entry->prev;
  }

  mark_as_free(self, entry);
  --self->P_entry_count;
  return true;
}

void P_nt_hash_container_clear(P_nt_hash_container_t *self) {
  if (self->P_entry_count == 0) {
    return;
  }

  self->P_free_entries_start = NIL;
  for (uint32_t i = 0; i < self->P_entry_capacity; ++i) {
    mark_as_free2(self, i);
  }
  memset(self->P_buckets, (int)NIL,
         sizeof(uint32_t) * self->P_buckets_capacity);
  self->P_entry_count = 0;
}

P_nt_hash_container_entry_hdr_t const *
P_nt_hash_container_get(P_nt_hash_container_t const *self, void const *key) {
  uint32_t hash;
  uint32_t *bucket;
  return get_entry_and_bucket((P_nt_hash_container_t *)self, key, &hash,
                              &bucket);
}

P_nt_hash_container_iterator_t
P_nt_hash_container_iterate(P_nt_hash_container_t const *self,
                            void const *key) {
  return (P_nt_hash_container_iterator_t){self, key, NIL, 0};
}

void const *
P_nt_hash_container_iterator_next(P_nt_hash_container_iterator_t *self) {
  P_nt_hash_container_t *container = (P_nt_hash_container_t *)self->P_container;
  P_nt_hash_container_entry_hdr_t *entry = NULL;

  if (self->P_key) {
    if (self->P_index == NIL) {
      uint32_t *bucket;
      entry =
          get_entry_and_bucket(container, self->P_key, &self->P_hash, &bucket);
      self->P_index = entry->next;
    } else {
      do {
        entry = get_entry(container, self->P_index);
        self->P_index = entry->next;
      } while (entry->hash != self->P_hash ||
               container->P_comparator(
                   self->P_key, get_key(container, entry)) != NT_COMPARE_EQUAL);
    }

    if (self->P_index == NIL) {
      self->P_key = NULL;
      self->P_index = container->P_entry_capacity;
    }
  } else {
    if (self->P_index == NIL) {
      self->P_index = 0;
    }

    while (self->P_index < container->P_entry_capacity && !entry) {
      entry = get_entry(container, self->P_index);
      if (!entry->used) {
        entry = NULL;
      }

      ++self->P_index;
    }
  }

  return entry ? get_key(self->P_container, entry) : NULL;
}
