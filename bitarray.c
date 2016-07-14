#include "bitarray.h"

#include <math.h>
#include <stdio.h>

#define BA_MODULUS (8 * sizeof(int))

static inline int ba_slot(size_t i) {
  return i / BA_MODULUS;
}

static inline int ba_offset(size_t i) {
  return i % BA_MODULUS;
}

static inline int max(int a, int b) {
  return a >= b ? a : b;
}


void ba_init(struct BitArray *ba, size_t capacity) {
  ba->size = capacity;
  size_t bytes = ceil(capacity / (double) BA_MODULUS);
  ba->bits = calloc(bytes, sizeof(char));
}

void ba_del(struct BitArray *ba) {
  free(ba->bits);
}

void ba_resize(struct BitArray *ba, size_t new_capacity) {
  int* old = ba->bits;
  size_t bytes = ceil(new_capacity / (double) BA_MODULUS);
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

void ba_set_grow(struct BitArray *ba, size_t i, bool b, double growth_factor) {
  if (i >= ba->size) {
    ba_resize(ba, max(i+1, ceil(growth_factor * ba->size)));
  }
  ba_set(ba, i, b);
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

void ba_print(struct BitArray *ba) {
  printf("|");
  for (size_t i=0; i<ba->size; ++i) {
    if (ba_get(ba, i))
      printf("x|");
    else
      printf("_|");
  }
  printf("\n");
}
