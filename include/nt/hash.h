#ifndef NTUTILS_HASH_H
#define NTUTILS_HASH_H

#include <stddef.h>
#include <stdint.h>

typedef uint32_t (*nt_hasher_t)(void const *value, size_t len);

uint32_t nt_murmur3_hasher(void const *value, size_t len);

static inline uint32_t nt_combine_hashes(uint32_t a, uint32_t b) {
  return a ^ (b + 0x9e3779b9u + (a << 6u) + (a >> 2u));
}

#endif // NTUTILS_HASH_H
