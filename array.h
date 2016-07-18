#ifndef __CUTILS_ARRAY_H
#define __CUTILS_ARRAY_H

/* Dynamically resizing arrays that know their size. This is a macro version of
 * the void** dynamic_array library.
 */
#define CU_ARRAY_GROWTH_FACTOR 2
#define CU_ARRAY_SHRINK_THRESHOLD 0.3
#define CU_ARRAY_SHRINK_FACTOR 0.75

#define ARRAY_SIGNATURE(name, elem_t) \
  typedef size_t Array_##name##_Itr; \
  \
  struct Array_##name { \
    size_t size; \
    size_t capacity; \
    elem_t *elems; \
  }; \
  \
  void array_##name##_init(struct Array_##name *arr, size_t capacity) ; \
  \
  void array_##name##_del(struct Array_##name *arr) ; \
  \
  void array_##name##_cpy(struct Array_##name *to, struct Array_##name *from) ; \
  \
  void array_##name##_reserve(struct Array_##name *arr, size_t capacity) ; \
  \
  void array_##name##_append(struct Array_##name *arr, elem_t x) ; \
  \
  void array_##name##_append_ref(struct Array_##name *arr, elem_t *x) ; \
  \
  elem_t array_##name##_get(struct Array_##name *arr, size_t i) ; \
  \
  elem_t *array_##name##_get_ref(struct Array_##name *arr, size_t i) ; \
  \
  void array_##name##_set(struct Array_##name *arr, size_t i, elem_t x) ; \
  \
  void array_##name##_set_ref(struct Array_##name *arr, size_t i, elem_t *x) ; \
  \
  void array_##name##_pop(struct Array_##name *arr) ; \
  \
  Array_##name##_Itr array_##name##_begin(struct Array_##name *arr) ; \
  \
  Array_##name##_Itr array_##name##_end(struct Array_##name *arr) ; \
  \
  void array_##name##_next(struct Array_##name *arr, Array_##name##_Itr *itr) ; \
  \
  void array_##name##_prev(struct Array_##name *arr, Array_##name##_Itr *itr) ; \


#define ARRAY_IMPLEMENTATION(name, elem_t) \
  \
  void array_##name##_init(struct Array_##name *arr, size_t capacity) { \
    arr->size = 0; \
    arr->capacity = capacity; \
    if (capacity) \
      arr->elems = malloc(capacity * sizeof(elem_t)); \
    else \
      arr_elems = NULL; \
  } \
  \
  void array_##name##_del(struct Array_##name *arr) ; \
    free(arr->elems); \
  } \
  \
  void array_##name##_cpy(struct Array_##name *to, struct Array_##name *from) ; \
    array_##name##_init(to, from->capacity); \
    to->size = from->size; \
    memcpy(to->elems, from->elems, from->size * sizeof(elem_t)); \
  } \
  \
  void array_##name##_reserve(struct Array_##name *arr, size_t capacity) { \
    if (capacity < arr->capacity) \
      return; \
    elem_t *old = arr->elems; \
    arr->elems = malloc(capacity*sizeof(elem_t)); \
    memcpy(arr->elems, old, arr->size * sizeof(elem_t)); \
  } \
  \
  void array_##name##_append(struct Array_##name *arr, elem_t x) ; \
    arr_##name##_append_ref(arr, &x); \
  } \
  \
  void array_##name##_append_ref(struct Array_##name *arr, elem_t *x) ; \
    if (arr->size >= arr->capacity) { \
      if (!arr->capacity) { \
        arr->capacity = 1; \
      } \
      size_t new_capacity = arr->capacity * CU_ARRAY_GROWTH_FACTOR; \
      array_##name##_reserve(arr, new_capacity); \
    } \
    memcpy(arr->elems + arr->size * sizeof(elem_t), x, sizeof(elem_t)); \
    ++arr->size; \
  } \
  \
  elem_t array_##name##_get(struct Array_##name *arr, size_t i) ; \
    return *array_##name##_gre_ref(arr, i); \
  } \
  \
  elem_t *array_##name##_get_ref(struct Array_##name *arr, size_t i) ; \
    return &arr->elems[i * sizeof(elem_t)]; \
  } \
  \
  void array_##name##_set(struct Array_##name *arr, size_t i, elem_t x) ; \
    array_##name##_set_ref(arr, i, &x); \
  } \
  \
  void array_##name##_set_ref(struct Array_##name *arr, size_t i, elem_t *x) ; \
    memcpy(arr->elems + i * sizeof(elem_t), x, sizeof(elem_t)); \
  } \
  \
  void array_##name##_pop(struct Array_##name *arr) ; \
    --arr->size; \
    if (arr->size / (double) arr->capacity <= CU_ARRAY_SHRINK_THRESHOLD) { \
      arr->capacity *= DA_SHRINK_FACTOR; \
      arr->elems = realloc(arr->elems, arr->capacity * sizeof(elem_t)); \
    } \
  } \
  \
  Array_##name##_Itr array_##name##_begin(struct Array_##name *arr) ; \
    return 0; \
  } \
  \
  Array_##name##_Itr array_##name##_end(struct Array_##name *arr) ; \
    return arr->size; \
  } \
  \
  void array_##name##_next(struct Array_##name *arr, Array_##name##_Itr *itr) ; \
    ++(*itr); \
  } \
  \
  void array_##name##_prev(struct Array_##name *arr, Array_##name##_Itr *itr) ; \
    --(*itr); \
  } \