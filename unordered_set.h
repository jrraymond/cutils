#ifndef __UNORDERED_SET_H
#define __UNORDERED_SET_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/*
 * use linear probing j = 5*j + 1 + perturb as guaranteed to hit every slot
 * before repeating.
 */
#define US_LOAD_FACTOR (2.0/3.0)

/* macros for checking flags. we need to keep track of 3 states: empty,
 * occupied, deleted. This requires 2 bits. 00 -> empty, 01->occupied,
 * 10->deleted. A flag is 8 bits so it holds 4 subflags.
 */
#define US_FLAG_EMP 0
#define US_FLAG_OCC 1
#define US_FLAG_DEL 2
#define __us_flag_clear(flag, i) (flag & ~(3<<(2*i)))
#define __us_flag_get(flag, i) ((flag >> (2*i))&3)
#define __us_flag_index(i) (i/sizeof(uint8_t))
#define __us_flag_offset(i) (i%sizeof(uint8_t))
#define __us_is_emp(flag, i) (((flag >> (2*i)) & 1) == US_FLAG_EMP)
#define __us_is_occ(flag, i) (((flag >> (2*i)) & 1) == US_FLAG_OCC)
#define __us_is_del(flag, i) (((flag >> (2*i)) & 1) == US_FLAG_DEL)
#define __us_set_emp(flag, i) __us_flag_clear(flag, i)
#define __us_set_occ(flag, i) (__us_flag_clear(flag, i) | (US_FLAG_OCC << (2*i)))
#define __us_set_del(flag, i) (__us_flag_clear(flag, i) | (US_FLAG_DEL << (2*i)))



#define UNORDERED_SET_TYPE(name, elem_t, hash_t) \
  typedef struct {                                                         \
    size_t capacity;                                                       \
    size_t size;                                                           \
    uint8_t *flags;                                                        \
    hash_t *hashes;                                                        \
    elem_t *elems;                                                         \
  } us_##name##_t ;                                                        \



#define UNORDERED_SET_SPEC(name, scope, elem_t, hash_t)            \
  scope void us_##name##_init(us_##name##_t *set, size_t capacity);  \
  scope void us_##name##_del(us_##name##_t *set); \
  scope void us_##name##_rehash(us_##name##_t *set, size_t capacity); \
  scope void us_##name##_insert(us_##name##_t *set, elem_t elem); \
  scope size_t us_##name##_find(us_##name##_t *set, elem_t elem) ; \
  scope bool us_##name##_contains(us_##name##_t *set, elem_t elem) ; \
  scope bool us_##name##_remove(us_##name##_t *set, elem_t elem) ; \
  scope void _us_##name##_debug_print(us_##name##_t *set, void (*print_hash)(hash_t), void (*print_elem)(elem_t)) ; \
  scope size_t us_##name##_begin(us_##name##_t *set);\
  scope size_t us_##name##_end(us_##name##_t *set); \
  scope void us_##name##_next(us_##name##_t *set, size_t *itr); \
  scope void us_##name##_prev(us_##name##_t *set, size_t *itr);



#define UNORDERED_SET_IMPL(name, scope, elem_t, hash_t, _hash_f, _eq_f)            \
                                                                           \
  scope void us_##name##_init(us_##name##_t *set, size_t capacity) {  \
    set->capacity = capacity; \
    set->size = 0; \
    set->flags = calloc(capacity / sizeof(uint8_t), sizeof(uint8_t)); \
    set->hashes = malloc(capacity * sizeof(hash_t)); \
    set->elems = malloc(capacity * sizeof(elem_t)); \
  } \
    \
  scope void us_##name##_del(us_##name##_t *set) { \
    free(set->flags); \
    free(set->hashes); \
    free(set->elems); \
  } \
 \
  scope void us_##name##_insert(us_##name##_t *set, elem_t elem); \
  scope void us_##name##_rehash(us_##name##_t *set, size_t capacity) { \
    uint8_t *old_flags = set->flags; \
    hash_t *old_hashes = set->hashes; \
    elem_t *old_elems = set->elems; \
    size_t old_cap = set->capacity; \
    \
    set->flags = calloc(capacity / sizeof(uint8_t), sizeof(uint8_t)); \
    set->hashes = malloc(capacity * sizeof(hash_t)); \
    set->elems = malloc(capacity * sizeof(elem_t)); \
    set->capacity = capacity; \
    \
    for (int i=0; i<old_cap; ++i) { \
      size_t ix = __us_flag_index(i); \
      size_t offset = __us_flag_offset(i); \
      uint8_t bucket_status = __us_flag_get(old_flags[ix], offset); \
      if (bucket_status == US_FLAG_OCC) { \
        us_##name##_insert(set, old_elems[i]); \
      } \
    } \
    \
    free(old_flags); \
    free(old_hashes); \
    free(old_elems); \
  } \
  scope void us_##name##_insert(us_##name##_t *set, elem_t elem) { \
    if (set->capacity * US_LOAD_FACTOR < set->size) { \
      us_##name##_rehash(set, set->capacity << 1); \
    } \
    hash_t hsh = _hash_f(elem); \
    size_t j = hsh % set->capacity; \
    for(;;) { \
      size_t jx = __us_flag_index(j); \
      size_t offset = __us_flag_offset(j); \
      uint8_t bucket_status = __us_flag_get(set->flags[jx], offset); \
      if (bucket_status == US_FLAG_OCC) { \
        j = (5 * j + 1) % set->capacity; \
      } else { \
        set->flags[jx] = __us_set_occ(set->flags[jx], offset); \
        set->elems[j] = elem; \
        set->hashes[j] = hsh; \
        ++set->size; \
        return; \
      } \
    } \
  } \
  scope size_t us_##name##_find(us_##name##_t *set, elem_t elem) { \
    hash_t hsh = _hash_f(elem); \
    size_t j = hsh % set->capacity; \
    for(;;) { \
      size_t jx = __us_flag_index(j); \
      size_t offset = __us_flag_offset(j); \
      uint8_t bucket_status = __us_flag_get(set->flags[jx], offset); \
      switch (bucket_status) { \
        case US_FLAG_EMP: \
          return set->capacity; \
        case US_FLAG_OCC: \
          if (set->hashes[j] == hsh && _eq_f(set->elems[j], elem)) { \
            return j; \
          } \
        case US_FLAG_DEL: \
          j = (5 * j + 1) % set->capacity; \
      } \
    } \
  } \
  scope bool us_##name##_contains(us_##name##_t *set, elem_t elem) { \
    return us_##name##_find(set, elem) != set->capacity; \
  } \
  scope bool us_##name##_remove(us_##name##_t *set, elem_t elem) { \
    size_t i = us_##name##_find(set, elem); \
    if (i == set->capacity) { \
      return false; \
    } \
    size_t ix = __us_flag_index(i); \
    size_t offset = __us_flag_offset(i); \
    set->flags[ix] = __us_set_del(set->flags[ix], offset); \
    --set->size; \
    return true; \
  } \
  scope void _us_##name##_debug_print(us_##name##_t *set, void (*print_hash)(hash_t), void (*print_elem)(elem_t)) { \
    for (int i=0; i<set->capacity; ++i) { \
      size_t ix = __us_flag_index(i); \
      size_t offset = __us_flag_offset(i); \
      uint8_t buffer_status = __us_flag_get(set->flags[ix], offset); \
      char *flag; \
      switch (buffer_status) { \
        case US_FLAG_EMP: \
          flag = "emp"; \
          break; \
        case US_FLAG_DEL: \
          flag = "del"; \
          break; \
        case US_FLAG_OCC: \
          flag = "occ"; \
          break; \
        default: \
          flag = "WTF"; \
      } \
      printf("%d: %s | ", i, flag); \
      if (buffer_status != US_FLAG_EMP) { \
        print_hash(set->hashes[i]); \
        printf(" | "); \
        print_elem(set->elems[i]); \
      } \
      printf("\n"); \
    } \
  } \
  \
  scope size_t us_##name##_begin(us_##name##_t *set) {\
    return 0; \
  } \
  \
  scope size_t us_##name##_end(us_##name##_t *set) { \
    size_t last = set->capacity - 1; \
    us_##name##_prev(set, &last); \
    return last; \
  } \
  \
  scope void us_##name##_next(us_##name##_t *set, size_t *itr) {\
    size_t last = *itr + 1; \
    while (last < set->capacity) { \
      size_t ix = __us_flag_index(last); \
      size_t offset = __us_flag_offset(last); \
      uint8_t f = __us_flag_get(set->flags[ix], offset); \
      if (f == US_FLAG_OCC) {\
        *itr = last; \
        return; \
      } \
      ++last; \
    } \
    return; \
  } \
  \
  scope void us_##name##_prev(us_##name##_t *set, size_t *itr) { \
    size_t last = *itr - 1; \
    while (last > 0) { \
      size_t ix = __us_flag_index(last); \
      size_t offset = __us_flag_offset(last); \
      uint8_t f = __us_flag_get(set->flags[ix], offset); \
      if (f == US_FLAG_OCC) {\
        *itr = last; \
        return; \
      } \
      --last; \
    } \
    return; \
  } \




#define UNORDERED_SET_DECLARE(name, scope, elem_t, hash_t) \
  UNORDERED_SET_TYPE(name, elem_t, hash_t) \
  UNORDERED_SET_SPEC(name, scope, elem_t, hash_t)



#endif
