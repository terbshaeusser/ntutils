#include "nt/circular.h"
#include "nt/assert.h"
#include <string.h>

NT_CIRC_BUF(void_buff, char)

static void *get_item_ptr(void_buff_t *self, size_t index, size_t item_size) {
  return self->P_items + (self->P_first + index) % self->P_capacity * item_size;
}

void P_nt_circ_buf_push(void *self, void const *item, size_t item_size) {
  void_buff_t *self2 = self;

  nt_assert(self2->P_count < self2->P_capacity,
            "Cannot push on a full circular buffer");

  void *ptr = get_item_ptr(self2, self2->P_count, item_size);
  memcpy(ptr, item, item_size);

  ++self2->P_count;
}

void P_nt_circ_buf_pop(void *self, void *item, size_t item_size) {
  void_buff_t *self2 = self;

  nt_assert(self2->P_count > 0, "Cannot pop from an empty circular buffer");

  if (item) {
    void *ptr = get_item_ptr(self2, 0, item_size);
    memcpy(item, ptr, item_size);
  }

  self2->P_first = (self2->P_first + 1) % self2->P_capacity;
  --self2->P_count;
}

void const *P_nt_circ_buf_get(void const *self, size_t index,
                              size_t item_size) {
  void_buff_t const *self2 = self;

  nt_assert(
      index < self2->P_count,
      "Index %zu is out of range of a circular buffer containing %zu items",
      index, self2->P_count);

  return get_item_ptr((void_buff_t *)self2, index, item_size);
}
