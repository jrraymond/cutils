#ifndef __CUTILS_ARRAY_H
#define __CUTILS_ARRAY_H

#include <strings.h>

/* Dynamically resizing arrays that know their size. This is a macro version of
 * the void** dynamic_array library.
 */
#define CU_ARRAY_GROWTH_FACTOR 2
#define CU_ARRAY_SHRINK_THRESHOLD 0.3
#define CU_ARRAY_SHRINK_FACTOR 0.75

#define ARRAY_STRUCT(name, elem_t) \
  struct Array_##name { \
    size_t size; \
    size_t capacity; \
    elem_t *elems; \
  };

#define ARRAY_SIGNATURE(name, scope, elem_t) \
  \
  ARRAY_STRUCT(name, elem_t) \
  \
  typedef size_t Array_##name##_Itr; \
  \
  scope void array_##name##_init(struct Array_##name *arr, size_t capacity) ; \
  \
  void array_##name##_init_with(struct Array_##name *arr, size_t capacity, elem_t el) ; \
  \
  void array_##name##_del(struct Array_##name *arr) ; \
  \
  scope void array_##name##_cpy(struct Array_##name *to, struct Array_##name *from) ; \
  \
  scope void array_##name##_reserve(struct Array_##name *arr, size_t capacity) ; \
  \
  scope void array_##name##_shrink_to_fit(struct Array_##name *arr) ; \
  \
  scope size_t array_##name##_size(struct Array_##name *arr) ; \
  \
  scope void array_##name##_clear(struct Array_##name *arr) ; \
  \
  scope void array_##name##_append(struct Array_##name *arr, elem_t x) ; \
  \
  scope void array_##name##_append_ref(struct Array_##name *arr, elem_t *x) ; \
  \
  scope elem_t array_##name##_get(struct Array_##name *arr, size_t i) ; \
  \
  scope elem_t *array_##name##_get_ref(struct Array_##name *arr, size_t i) ; \
  \
  scope void array_##name##_set(struct Array_##name *arr, size_t i, elem_t x) ; \
  \
  scope void array_##name##_set_ref(struct Array_##name *arr, size_t i, elem_t *x) ; \
  \
  scope void array_##name##_pop(struct Array_##name *arr) ; \
  \
  scope Array_##name##_Itr array_##name##_begin(struct Array_##name *arr) ; \
  \
  scope Array_##name##_Itr array_##name##_end(struct Array_##name *arr) ; \
  \
  scope void array_##name##_next(struct Array_##name *arr, Array_##name##_Itr *itr) ; \
  \
  scope void array_##name##_prev(struct Array_##name *arr, Array_##name##_Itr *itr) ; \
  \
  scope void array_##name##_print(struct Array_##name *arr, void (*print_elem)(elem_t)) ; \


#define ARRAY_IMPLEMENTATION(name, scope, elem_t) \
  \
  scope void array_##name##_init(struct Array_##name *arr, size_t capacity) { \
    arr->size = 0; \
    arr->capacity = capacity; \
    if (capacity) \
      arr->elems = malloc(capacity * sizeof(elem_t)); \
    else \
      arr->elems = NULL; \
  } \
  void array_##name##_init_with(struct Array_##name *arr, size_t capacity, elem_t el) { \
    array_##name##_init(arr, capacity); \
    for (size_t i=0; i<capacity; ++i) \
      arr->elems[i] = el; \
  } \
  \
  scope void array_##name##_del(struct Array_##name *arr) { \
    free(arr->elems); \
  } \
  \
  scope void array_##name##_cpy(struct Array_##name *to, struct Array_##name *from) { \
    array_##name##_init(to, from->capacity); \
    to->size = from->size; \
    memcpy((void*) to->elems, (void*) from->elems, (size_t) from->size * sizeof(elem_t)); \
  } \
  \
  scope void array_##name##_reserve(struct Array_##name *arr, size_t capacity) { \
    if (capacity <= arr->capacity) \
      return; \
    elem_t *old = arr->elems; \
    arr->elems = malloc(capacity*sizeof(elem_t)); \
    arr->capacity = capacity; \
    memcpy((void*) arr->elems, (void*) old, (size_t) arr->size * sizeof(elem_t)); \
    free(old); \
  } \
  \
  scope size_t array_##name##_size(struct Array_##name *arr) { \
    return arr->size; \
  } \
  \
  scope void array_##name##_clear(struct Array_##name *arr) { \
    arr->size = 0; \
  } \
  \
  scope void array_##name##_shrink_to_fit(struct Array_##name *arr) { \
    if (arr->size < arr->capacity) { \
      arr->capacity = arr->size; \
      arr->elems = realloc(arr->elems, arr->capacity * sizeof(elem_t)); \
    } \
  } \
  \
  scope void array_##name##_append(struct Array_##name *arr, elem_t x) { \
    array_##name##_append_ref(arr, &x); \
  } \
  \
  scope void array_##name##_append_ref(struct Array_##name *arr, elem_t *x) { \
    if (arr->size >= arr->capacity) { \
      if (!arr->capacity) \
        ++arr->capacity; \
      size_t new_capacity = arr->capacity; \
      if (!CU_ARRAY_GROWTH_FACTOR) \
        new_capacity = arr->capacity + 1; \
      else \
        new_capacity = arr->capacity * CU_ARRAY_GROWTH_FACTOR; \
      array_##name##_reserve(arr, new_capacity); \
    } \
    arr->elems[arr->size] = *x; \
    ++arr->size; \
  } \
  \
  scope elem_t array_##name##_get(struct Array_##name *arr, size_t i) { \
    return *array_##name##_get_ref(arr, i); \
  } \
  \
  scope elem_t *array_##name##_get_ref(struct Array_##name *arr, size_t i) { \
    assert(i >= 0); \
    assert(i < arr->size); \
    return &arr->elems[i]; \
  } \
  \
  scope void array_##name##_set(struct Array_##name *arr, size_t i, elem_t x) { \
    array_##name##_set_ref(arr, i, &x); \
  } \
  \
  scope void array_##name##_set_ref(struct Array_##name *arr, size_t i, elem_t *x) { \
    assert(i >= 0); \
    assert(i < arr->size); \
    arr->elems[i] = *x; \
  } \
  \
  scope void array_##name##_pop(struct Array_##name *arr) { \
    assert(arr->size > 0); \
    --arr->size; \
  } \
  \
  scope Array_##name##_Itr array_##name##_begin(struct Array_##name *arr) { \
    return 0; \
  } \
  \
  scope Array_##name##_Itr array_##name##_end(struct Array_##name *arr) { \
    return arr->size; \
  } \
  \
  scope void array_##name##_next(struct Array_##name *arr, Array_##name##_Itr *itr) { \
    ++(*itr); \
  } \
  \
  scope void array_##name##_prev(struct Array_##name *arr, Array_##name##_Itr *itr) { \
    --(*itr); \
  } \
  \
  scope void array_##name##_print(struct Array_##name *arr, void (*print_elem)(elem_t)) { \
    printf("{%zu|%zu@%p}[", arr->size, arr->capacity, arr->elems); \
    for (Array_##name##_Itr itr = array_##name##_begin(arr); \
        itr != array_##name##_end(arr); \
        array_##name##_next(arr, &itr) \
        ) { \
      print_elem(arr->elems[itr]); \
      printf(","); \
    } \
    printf("]\n"); \
  }


#define CUTILS_ARRAY(name, scope, elem_t) \
  ARRAY_SIGNATURE(name, scope, elem_t) \
  ARRAY_IMPLEMENTATION(name, scope, elem_t)

#endif
