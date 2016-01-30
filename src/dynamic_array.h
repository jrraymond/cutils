#ifndef __DYNAMIC_ARRAY_H
#define __DYNAMIC_ARRAY_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct darray_t {
  size_t size;
  size_t capacity;
  size_t elem_size;
  void* elems;
} darray_t;

double GROWTHFACTOR = 1.5;
double SHRINKTHRESHOLD = 0.3;
double SHRINKFACTOR = 0.75;

void mk_darray(darray_t* arr, size_t sz, size_t elem_sz);

void rm_darray(darray_t* arr);

void append(darray_t* arr, void* elem);

void pop(darray_t* arr);

void get(darray_t* arr, int index, void* elem);

void set(darray_t* arr, int index, void* elem);
#endif
