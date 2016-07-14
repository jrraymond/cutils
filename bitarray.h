#ifndef __BITARRAY_C
#define __BITARRAY_C

#include <stdbool.h>
#include <stdlib.h>

struct BitArray {
  size_t size;
  int *bits;
};

void ba_init(struct BitArray *ba, size_t capacity) ;

void ba_del(struct BitArray *ba) ;

void ba_resize(struct BitArray *ba, size_t new_capacity);

void ba_set_grow(struct BitArray *ba, size_t i, bool b, double growth_factor) ;

void ba_set(struct BitArray *ba, size_t i, bool b) ;

bool ba_get(struct BitArray *ba, size_t i) ;

void ba_print(struct BitArray *ba) ;

#endif
