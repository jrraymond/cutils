#ifndef __CUTILS_BITARRAY_C
#define __CUTILS_BITARRAY_C

#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define BA_BYTE 8
#define BA_MODULUS (BA_BYTE * sizeof(int))


/** BitArray
 * size in # of bits
 */
struct BitArray {
  size_t size;
  int *bits;
};

/** Initialize a BitArray with a number of bits initially set to zero.
 */
void ba_init(struct BitArray *ba, size_t capacity) ;

/** Free memory held by a BitArray.
 */
void ba_del(struct BitArray *ba) ;

/** Resize a BitArray to a new number of bits, which may be smaller or larger
 * than the previous number of bits. The previous bits are maintained, and new
 * bits are zeroed.
 */
void ba_resize(struct BitArray *ba, size_t new_capacity);

/** Set bit at index i to b. No bounds checking.
 */
inline void ba_set(struct BitArray *ba, size_t i, bool b) ;

/** Set all bits to true or false
 */
void ba_set_all(struct BitArray *ba, bool b) ;

/** Set a bit, growing the capacity of the bitarray if the index is out of
 * bounds by growth_factor.
 */
inline void ba_set_grow(struct BitArray *ba, size_t i, bool b, double growth_factor) ;

/** Get the value of bit at index i. No bounds checking.
 */
inline bool ba_get(struct BitArray *ba, size_t i) ;

/** Retrieve the number of bits held in bit array.
 */
inline size_t ba_size(struct BitArray *ba) ;

/** Print the bit array.
 * Ex: |_|x|_|x|_|x|
 * x indicates bit set, _ indicates otherwise.
 */
void ba_print(struct BitArray *ba) ;

inline int ba_slot(size_t i) {
  return i / BA_MODULUS;
}

inline int ba_offset(size_t i) {
  return i % BA_MODULUS;
}

inline int max(int a, int b) {
  return a >= b ? a : b;
}


void ba_init(struct BitArray *ba, size_t capacity) {
  ba->size = capacity;
  size_t ints = ceil(capacity / (double) BA_MODULUS);
  ba->bits = calloc(ints, sizeof(int));
}

void ba_del(struct BitArray *ba) {
  free(ba->bits);
}

void ba_resize(struct BitArray *ba, size_t new_capacity) {
  int* old = ba->bits;
  size_t ints = ceil(new_capacity / (double) BA_MODULUS);
  ba->bits = calloc(ints, sizeof(int));
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

inline void ba_set_grow(struct BitArray *ba, size_t i, bool b, double growth_factor) {
  if (i >= ba->size) {
    ba_resize(ba, max(i+1, ceil(growth_factor * ba->size)));
  }
  ba_set(ba, i, b);
}


inline void ba_set(struct BitArray *ba, size_t i, bool b) {
  size_t slot = ba_slot(i);
  size_t offset = ba_offset(i);
  ba->bits[slot] &= ~(1U << offset);
  ba->bits[slot] |= b << offset;
}

void ba_set_all(struct BitArray *ba, bool b) {
  size_t ints = ceil(ba->size / (double) BA_MODULUS);
  unsigned char c = b ? 0xFF : 0x0;
  memset(ba->bits, c, ints * sizeof(int));
}

inline bool ba_get(struct BitArray *ba, size_t i) {
  assert(i > 0);
  assert(i < ba->size);
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

size_t ba_size(struct BitArray *ba) {
  return ba->size;
}


#endif
