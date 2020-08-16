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
    *bucket = NULL;
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
  entry->next_has_same_key = false;

  return index;
}

static uint32_t find_entry_in_bucket(P_nt_hash_container_t *self,
                                     uint32_t bucket, uint32_t hash,
                                     void const *key) {
  uint32_t index = bucket;

  while (index != NIL) {
    P_nt_hash_container_entry_hdr_t *entry = get_entry(self, index);

    if (entry->hash == hash &&
        self->P_comparator(key, get_key(self, entry)) == NT_COMPARE_EQUAL) {
      return index;
    }

    index = entry->next;
  }

  return NIL;
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
    uint32_t next_index =
        find_entry_in_bucket(self, *bucket, entry->hash, get_key(self, entry));

    if (next_index == NIL) {
      entry->prev = NIL;
      entry->next = *bucket;
      entry->next_has_same_key = false;
      *bucket = i;
    } else {
      P_nt_hash_container_entry_hdr_t *next_entry = get_entry(self, next_index);

      entry->next_has_same_key = true;
      entry->prev = next_entry->prev;
      entry->next = next_index;

      if (next_entry->prev == NIL) {
        *bucket = i;
      } else {
        get_entry(self, next_entry->prev)->next = i;
      }

      next_entry->prev = i;
    }
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

void P_nt_hash_container_add_multi(P_nt_hash_container_t *self, void const *key,
                                   P_nt_hash_container_entry_hdr_t **entry) {
  if (self->P_entry_count == self->P_entry_capacity) {
    P_nt_hash_container_reserve(self, 1);
  }

  uint32_t hash;
  uint32_t *bucket;
  P_nt_hash_container_entry_hdr_t *existing_entry =
      get_entry_and_bucket(self, key, &hash, &bucket);

  uint32_t entry_index = mark_as_used(self);
  *entry = get_entry(self, entry_index);
  (*entry)->hash = hash;
  memcpy(get_key(self, *entry), key, self->P_key_size);

  if (existing_entry) {
    (*entry)->next_has_same_key = true;
    (*entry)->prev = existing_entry->prev;
    (*entry)->next = get_entry_index(self, existing_entry);

    if (existing_entry->prev == NIL) {
      *bucket = entry_index;
    } else {
      get_entry(self, existing_entry->prev)->next = entry_index;
    }

    existing_entry->prev = entry_index;
  } else {
    (*entry)->prev = NIL;
    (*entry)->next = *bucket;

    *bucket = entry_index;
  }

  ++self->P_entry_count;
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
    P_nt_hash_container_entry_hdr_t *prev_entry = get_entry(self, entry->prev);
    prev_entry->next = entry->next;

    if (prev_entry->next_has_same_key) {
      prev_entry->next_has_same_key = entry->next_has_same_key;
    }
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
      if (entry) {
        self->P_index = get_entry_index(container, entry);
      } else {
        self->P_index = NIL;
      }
    } else {
      P_nt_hash_container_entry_hdr_t *prev_entry =
          get_entry(container, self->P_index);

      if (prev_entry->next_has_same_key) {
        self->P_index = prev_entry->next;
        entry = get_entry(container, self->P_index);
      } else {
        self->P_index = NIL;
      }
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
