#include "nt/queue.h"
#include "nt/assert.h"
#include <string.h>

NT_QUEUE(void_queue, uint8_t)

enum { NIL = 0xFFFFFFFFu };

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

static P_nt_queue_entry_hdr_t *get_entry(void_queue_t *self, uint32_t index) {
  return (P_nt_queue_entry_hdr_t *)&self
      ->P_entries[index * self->P_entity_size];
}

static uint32_t get_entry_index(void_queue_t const *self,
                                P_nt_queue_entry_hdr_t const *entry) {
  return (((uint8_t const *)entry) - self->P_entries) / self->P_entity_size;
}

static void *get_value_ptr(void_queue_t const *self,
                           P_nt_queue_entry_hdr_t const *entry) {
  return &((uint8_t *)entry)[self->P_value_off];
}

static void mark_as_free(void_queue_t *self, P_nt_queue_entry_hdr_t *entry,
                         bool unlink) {
  uint32_t index = get_entry_index(self, entry);

  if (unlink) {
    if (entry->prev == NIL) {
      self->P_first_used = entry->next;
    } else {
      get_entry(self, entry->prev)->next = entry->next;
    }

    if (entry->next == NIL) {
      self->P_last_used = entry->prev;
    } else {
      get_entry(self, entry->next)->prev = entry->prev;
    }
  }

  entry->prev = NIL;
  entry->next = self->P_first_free;

  if (self->P_first_free != NIL) {
    get_entry(self, self->P_first_free)->prev = index;
  }
  self->P_first_free = index;
}

static uint32_t mark_as_used(void_queue_t *self) {
  uint32_t index = self->P_first_free;
  P_nt_queue_entry_hdr_t *entry = get_entry(self, index);

  if (entry->prev == NIL) {
    self->P_first_free = entry->next;
  } else {
    get_entry(self, entry->prev)->next = entry->next;
  }

  if (entry->next != NIL) {
    get_entry(self, entry->next)->prev = entry->prev;
  }

  entry->prev = self->P_last_used;
  entry->next = NIL;

  if (self->P_last_used == NIL) {
    self->P_first_used = index;
  } else {
    get_entry(self, self->P_last_used)->next = index;
  }
  self->P_last_used = index;
  return index;
}

void P_nt_queue_reserve(void *self, size_t amount) {
  void_queue_t *self2 = self;

  size_t new_capacity = self2->P_count + amount;
  if (self2->P_capacity >= new_capacity) {
    return;
  }

  new_capacity = align_capacity(new_capacity);
  size_t new_capacity_bytes = new_capacity * self2->P_entity_size;

  if (self2->P_capacity == 0) {
    self2->P_entries = nt_alloc(uint8_t, new_capacity_bytes);
  } else {
    self2->P_entries = nt_realloc(self2->P_entries, new_capacity_bytes);
  }

  if (self2->P_capacity == 0) {
    self2->P_first_used = NIL;
    self2->P_last_used = NIL;
    self2->P_first_free = NIL;
  }

  for (uint32_t i = self2->P_capacity; i < new_capacity; ++i) {
    mark_as_free(self2, get_entry(self2, i), false);
  }

  self2->P_capacity = new_capacity;
}

void P_nt_queue_add(void *self, void const *value) {
  void_queue_t *self2 = self;

  if (self2->P_capacity == self2->P_count) {
    P_nt_queue_reserve(self, 1);
  }

  uint32_t index = mark_as_used(self2);
  P_nt_queue_entry_hdr_t *entry = get_entry(self2, index);
  memcpy(get_value_ptr(self2, entry), value, self2->P_value_size);

  ++self2->P_count;
}

void const *P_nt_queue_peek(void const *self) {
  void_queue_t const *self2 = self;

  nt_assert(self2->P_count > 0, "Cannot peek on empty queue");

  return get_value_ptr(self2,
                       get_entry((void_queue_t *)self2, self2->P_first_used));
}

void P_nt_queue_take(void *self, void *dst) {
  void_queue_t *self2 = self;

  nt_assert(self2->P_count > 0, "Cannot take next value from empty queue");

  P_nt_queue_entry_hdr_t *entry = get_entry(self2, self2->P_first_used);
  memcpy(dst, get_value_ptr(self2, entry), self2->P_value_size);
  mark_as_free(self2, entry, true);
  --self2->P_count;
}

void P_nt_queue_clear(void *self) {
  void_queue_t *self2 = self;

  self2->P_first_used = NIL;
  self2->P_last_used = NIL;
  self2->P_first_free = NIL;
  self2->P_count = 0;

  for (uint32_t i = 0; i < self2->P_capacity; ++i) {
    mark_as_free(self2, get_entry(self2, i), false);
  }
}

void const *P_nt_queue_iterator_next(void *self) {
  void_queue_iterator_t *self2 = self;
  void_queue_t const *container = self2->P_container;

  if (self2->P_index == NIL) {
    return NULL;
  }

  P_nt_queue_entry_hdr_t const *entry =
      get_entry((void_queue_t *)container, self2->P_index);
  self2->P_index = entry->next;

  return get_value_ptr(container, entry);
}
