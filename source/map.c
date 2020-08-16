#include "nt/map.h"
#include <string.h>

NT_MAP(void_map, uint8_t, uint8_t)

static void *get_value(void_map_t *self,
                       P_nt_hash_container_entry_hdr_t *entry) {
  return ((uint8_t *)entry) + self->P_value_off;
}

bool P_nt_map_add(void *self, void const *key, void const *value) {
  void_map_t *self2 = self;

  P_nt_hash_container_entry_hdr_t *entry;
  bool added = P_nt_hash_container_add(self, key, &entry);

  if (added) {
    memcpy(get_value(self2, entry), value, self2->P_value_size);
  }

  return added;
}

void P_nt_map_add_multi(void *self, void const *key, void const *value) {
  void_map_t *self2 = self;

  P_nt_hash_container_entry_hdr_t *entry;
  P_nt_hash_container_add_multi(self, key, &entry);

  memcpy(get_value(self2, entry), value, self2->P_value_size);
}

void P_nt_map_put(void *self, void const *key, void const *value) {
  void_map_t *self2 = self;

  P_nt_hash_container_entry_hdr_t *entry;
  P_nt_hash_container_add(self, key, &entry);

  memcpy(get_value(self2, entry), value, self2->P_value_size);
}

void const *P_nt_map_get(void const *self, void const *key) {
  void_map_t const *self2 = self;

  P_nt_hash_container_entry_hdr_t const *entry =
      P_nt_hash_container_get(self, key);

  if (!entry) {
    return NULL;
  }

  return get_value((void_map_t *)self2,
                   (P_nt_hash_container_entry_hdr_t *)entry);
}
