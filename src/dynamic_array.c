#include "dynamic_array.h"

void mk_darray(darray_t* arr, size_t sz, size_t elem_sz) {
  arr->size = 0;
  arr->capacity = sz;
  arr->elem_size = elem_sz;
  arr->elems = malloc(sz*elem_sz);
}


void rm_darray(darray_t* arr) {
  free(arr->elems);
}

void append(darray_t* arr, void* elem) {
  if (arr->size == arr->capacity) {
    arr->capacity *= GROWTHFACTOR;
    void* new = realloc(arr->elems, arr->capacity*arr->elem_size);
    if (!new) {//realloc failed
      return;
    }
    arr->elems = new;
  }
  memcpy(arr->elems + arr->size*arr->elem_size, elem, arr->elem_size);
  ++arr->size;
}

void pop(darray_t* arr) {
  --arr->size;
  if (arr->size / (double) arr->capacity <= SHRINKTHRESHOLD) {
    arr->capacity *= SHRINKFACTOR;
    if (arr->capacity < 1)
      arr->capacity = 1;
    void* new = realloc(arr->elems, arr->capacity*arr->elem_size);
    if (!new)
      return;
    arr->elems = new;
  }
}

void get(darray_t* arr, int index, void* elem) {
   memcpy(elem, arr->elems + index*arr->elem_size, arr->elem_size);
}

void set(darray_t* arr, int index, void* elem) {
  memcpy(arr->elems + index*arr->elem_size, elem, arr->elem_size);
}
