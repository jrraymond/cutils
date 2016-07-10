#ifndef __UNORDERED_SET_H
#define __UNORDERED_SET_H

#include <stdlib.h>
#include <stdint.h>

/* macros for checking flags. we need to keep track of 3 states: empty,
 * occupied, deleted. This requires 2 bits. 00 -> empty, 01->occupied,
 * 10->deleted. A flag is 8 bits so it holds 4 subflags.
 */
#define US_FLAG_EMP 0
#define US_FLAG_OCC 1
#define US_FLAG_DEL 2
#define __us_is_empty(flag, i) (((flag >> (2*i)) & 1) == US_FLAG_EMP)
#define __us_is_occ(flag, i) (((flag >> (2*i)) & 1) == US_FLAG_OCC)
#define __us_is_del(flag, i) (((flag >> (2*i)) & 1) == US_FLAG_DEL)
#define __us_set_empty(flag, i) (flag & (~((US_FLAG_EMP << (2*i)

#define UNORDERED_SET_INIT(name, scope, elem_t, _hash_f, _eq_f)            \
  typedef struct {                                                         \
    size_t capacity;                                                       \
    size_t size;                                                           \
    uint8_t *flags;                                                        \
    uint64_t *hash;                                                        \
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
