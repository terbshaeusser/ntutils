#include "nt/circular.h"
#include "nt/assert.h"
#include <string.h>

NT_CIRC_BUF(void_buff, char)

static void *get_item_ptr(void_buff_t *self, size_t index) {
  return self->P_items +
         (self->P_first + index) % self->P_capacity * self->P_item_size;
}

void P_nt_circ_buf_dyn_push(void *self, void const *item) {
  void_buff_t *self2 = self;

  nt_assert(self2->P_count < self2->P_capacity,
            "Cannot push on a full circular buffer");

  void *ptr = get_item_ptr(self2, self2->P_count);
  memcpy(ptr, item, self2->P_item_size);

  ++self2->P_count;
}

void P_nt_circ_buf_dyn_pop(void *self, void *item) {
  void_buff_t *self2 = self;

  nt_assert(self2->P_count > 0, "Cannot pop from an empty circular buffer");

  if (item) {
    void *ptr = get_item_ptr(self2, 0);
    memcpy(item, ptr, self2->P_item_size);
  }

  self2->P_first = (self2->P_first + 1) % self2->P_capacity;
  --self2->P_count;
}

void const *P_nt_circ_buf_dyn_get(void const *self, size_t index) {
  void_buff_t const *self2 = self;

  nt_assert(
      index < self2->P_count,
      "Index %zu is out of range for a circular buffer containing %zu items",
      index, self2->P_count);

  return get_item_ptr((void_buff_t *)self2, index);
}

NT_CIRC_BUF(void_buff_fx, char, 1)

static void *get_item_ptr_fx(void_buff_fx_t *self, size_t capacity,
                             size_t index) {
  return self->P_items + (self->P_first + index) % capacity * self->P_item_size;
}

void P_nt_circ_buf_fx_push(void *self, size_t capacity, void const *item) {
  void_buff_fx_t *self2 = self;

  nt_assert(self2->P_count < capacity, "Cannot push on a full circular buffer");

  void *ptr = get_item_ptr_fx(self2, capacity, self2->P_count);
  memcpy(ptr, item, self2->P_item_size);

  ++self2->P_count;
}

void P_nt_circ_buf_fx_pop(void *self, size_t capacity, void *item) {
  void_buff_fx_t *self2 = self;

  nt_assert(self2->P_count > 0, "Cannot pop from an empty circular buffer");

  if (item) {
    void *ptr = get_item_ptr_fx(self2, capacity, 0);
    memcpy(item, ptr, self2->P_item_size);
  }

  self2->P_first = (self2->P_first + 1) % capacity;
  --self2->P_count;
}

void const *P_nt_circ_buf_fx_get(void const *self, size_t capacity,
                                 size_t index) {
  void_buff_fx_t const *self2 = self;

  nt_assert(
      index < self2->P_count,
      "Index %zu is out of range for a circular buffer containing %zu items",
      index, self2->P_count);

  return get_item_ptr_fx((void_buff_fx_t *)self2, capacity, index);
}
