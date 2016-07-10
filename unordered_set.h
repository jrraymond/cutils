#ifndef __UNORDERED_SET_H
#define __UNORDERED_SET_H

#include <stdlib.h>


#define UNORDERED_SET_INIT(name, scope, elem_t, _hash_f, _eq_f)            \
  typedef struct {                                                         \
    size_t capacity;                                                       \
    size_t size;                                                           \
    int *flags;                                                            \
    elem_t *elems;                                                         \
  } us_##name##_t ;                                                        \
                                                                           \
  scope inline void us_##name##_init(us_##name##_t *set, size_t capacity) {  \
    set->capacity = capacity; \
    set->size = 0; \
    set->flags = malloc(capacity / sizeof(int)); \
    set->elems = malloc(capacity * sizeof(elem_t)); \
  } \
    \
  scope inline void us_##name##_del(us_##name##_t *set) { \
    free(set->flags); \
    free(set->elems); \
  } \
    \
  scope inline void us_##name##_insert(us_##name##_t *set, elem_t *elem) { \
    /*TODO*/ \
  } \
  scope inline void us_##name##_contains(us_##name##_t *set, elem_t *elem) { \
    /* TODO */ \
  } \
  scope inline void us_##name##_remove(us_##name##_t *set, elem_t *elem) { \
    /* TODO */ \
  } \
  scope inline void us_##name##_rehash(us_##name##_t *set) { \
    /* TODO */ \
  }




#endif
