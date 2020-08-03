#include "nt/list.h"
#include "nt/assert.h"
#include <string.h>

NT_LIST(void_list, char)

_Static_assert(
    sizeof(size_t) == 4 || sizeof(size_t) == 8,
    "Only architectures with sizeof(size_t) == 4 or 8 are supported");

static size_t align_capacity(size_t n) {
  if (n <= 4) {
    return 4;
  }

  if (sizeof(size_t) == 4) {
    return 1u << (32u - __builtin_clz(n - 1));
  }

  return 1ull << (64u - __builtin_clzll(n - 1));
}

void P_nt_list_reserve(void *self, size_t amount, size_t item_size) {
  void_list_t *self2 = self;

  size_t new_capacity = self2->P_count + amount;
  if (self2->P_capacity >= new_capacity) {
    return;
  }

  new_capacity = align_capacity(new_capacity);
  size_t new_capacity_bytes = new_capacity * item_size;

  if (self2->P_capacity == 0) {
    self2->P_items = nt_alloc(char, new_capacity_bytes);
  } else {
    self2->P_items = nt_realloc(self2->P_items, new_capacity_bytes);
  }

  self2->P_capacity = new_capacity;
}

void P_nt_list_shrink(void *self, size_t item_size) {
  void_list_t *self2 = self;

  if (self2->P_capacity == self2->P_count) {
    return;
  }

  if (self2->P_count == 0) {
    nt_free(self2->P_items);
    self2->P_items = NULL;
  } else {
    size_t new_capacity_bytes = self2->P_count * item_size;
    self2->P_items = nt_realloc(self2->P_items, new_capacity_bytes);
  }

  self2->P_capacity = self2->P_count;
}

size_t P_nt_list_insert(void *self, size_t index, void const *item,
                        size_t item_size) {
  void_list_t *self2 = self;
  nt_assert(index <= self2->P_count,
            "Invalid insertion index %zu for list containing %zu items", index,
            self2->P_count);

  if (self2->P_count == self2->P_capacity) {
    P_nt_list_reserve(self, 1, item_size);
  }

  if (index < self2->P_count) {
    memmove(&self2->P_items[(index + 1) * item_size],
            &self2->P_items[index * item_size],
            (self2->P_count - index) * item_size);
  }

  memcpy(&self2->P_items[index * item_size], item, item_size);

  ++self2->P_count;
  return index;
}

void P_nt_list_remove(void *self, size_t index, size_t item_size) {
  void_list_t *self2 = self;
  nt_assert(index < self2->P_count,
            "Invalid index %zu for list containing %zu items", index,
            self2->P_count);

  if (index < self2->P_count - 1) {
    memmove(&self2->P_items[index * item_size],
            &self2->P_items[(index + 1) * item_size],
            (self2->P_count - index - 1) * item_size);
  }

  --self2->P_count;
}

void const *P_nt_list_get(void const *self, size_t index, size_t item_size) {
  void_list_t const *self2 = self;
  nt_assert(index < self2->P_count,
            "Invalid index %zu for list containing %zu items", index,
            self2->P_count);

  return &self2->P_items[index * item_size];
}
