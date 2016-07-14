#include "bitarray.h"

#include <math.h>

static inline int ba_slot(size_t i) {
  return i / sizeof(int);
}

static inline int ba_offset(size_t i) {
  return i % sizeof(int);
}


void ba_init(struct BitArray *ba, size_t capacity) {
  ba->size = capacity;
  size_t bytes = ceil(capacity / (8.0 * (double) sizeof(int)));
  ba->bits = calloc(bytes, sizeof(char));
}

void ba_del(struct BitArray *ba) {
  free(ba->bits);
}

void ba_resize(struct BitArray *ba, size_t new_capacity) {
  int* old = ba->bits;
  size_t bytes = ceil(new_capacity / (8.0 * (double) sizeof(int)));
  ba->bits = calloc(bytes, sizeof(char));
  size_t bound = ba->size <= new_capacity ? ba->size : new_capacity;
  for (size_t i=0; i<bound; ++i) {
    size_t slot = ba_slot(i);
    size_t offset = ba_offset(i);
    bool b = (old[slot] >> offset)&1;
    ba->bits[slot] &= ~(1U << offset);
    ba->bits[slot] |= b << offset;
  }
  ba->size = new_capacity;
  free(old);
}

void ba_set(struct BitArray *ba, size_t i, bool b) {
  size_t slot = ba_slot(i);
  size_t offset = ba_offset(i);
  ba->bits[slot] &= ~(1U << offset);
  ba->bits[slot] |= b << offset;
}

bool ba_get(struct BitArray *ba, size_t i) {
  size_t slot = ba_slot(i);
  size_t offset = ba_offset(i);
  return (ba->bits[slot] >> offset)&1;
}
