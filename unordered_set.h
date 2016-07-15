#ifndef __CUTILS_UNORDERED_SET_H
#define __CUTILS_UNORDERED_SET_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/*
 * use linear probing j = 5*j + 1 + perturb as guaranteed to hit every slot
 * before repeating.
 */
#define CU_US_LOAD_FACTOR (2.0/3.0)

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
  struct USet_##name {                                                         \
    size_t capacity;                                                       \
    size_t size;                                                           \
    uint8_t *flags;                                                        \
    hash_t *hashes;                                                        \
    elem_t *elems;                                                         \
  } ;                                                        \



#define UNORDERED_SET_SPEC(name, scope, elem_t, hash_t)            \
  scope void uset_##name##_init(struct USet_##name *set, size_t capacity);  \
  scope void uset_##name##_del(struct USet_##name *set); \
  scope void uset_##name##_rehash(struct USet_##name *set, size_t capacity); \
  scope void uset_##name##_insert(struct USet_##name *set, elem_t elem); \
  scope size_t uset_##name##_find(struct USet_##name *set, elem_t elem) ; \
  scope bool uset_##name##_contains(struct USet_##name *set, elem_t elem) ; \
  scope bool uset_##name##_remove(struct USet_##name *set, elem_t elem) ; \
  scope void _uset_##name##_debug_print(struct USet_##name *set, void (*print_hash)(hash_t), void (*print_elem)(elem_t)) ; \
  scope size_t uset_##name##_begin(struct USet_##name *set);\
  scope size_t uset_##name##_end(struct USet_##name *set); \
  scope void uset_##name##_next(struct USet_##name *set, size_t *itr); \
  scope void uset_##name##_prev(struct USet_##name *set, size_t *itr); \
  scope bool uset_##name##_eq(struct USet_##name *a, struct USet_##name *b);



#define UNORDERED_SET_IMPL(name, scope, elem_t, hash_t, _hash_f, _eq_f)            \
                                                                           \
  scope void uset_##name##_init(struct USet_##name *set, size_t capacity) {  \
    set->capacity = capacity; \
    set->size = 0; \
    set->flags = calloc(capacity / sizeof(uint8_t), sizeof(uint8_t)); \
    set->hashes = malloc(capacity * sizeof(hash_t)); \
    set->elems = malloc(capacity * sizeof(elem_t)); \
  } \
    \
  scope void uset_##name##_del(struct USet_##name *set) { \
    free(set->flags); \
    free(set->hashes); \
    free(set->elems); \
  } \
 \
  scope void uset_##name##_insert(struct USet_##name *set, elem_t elem); \
  scope void uset_##name##_rehash(struct USet_##name *set, size_t capacity) { \
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
        uset_##name##_insert(set, old_elems[i]); \
      } \
    } \
    \
    free(old_flags); \
    free(old_hashes); \
    free(old_elems); \
  } \
  scope void uset_##name##_insert(struct USet_##name *set, elem_t elem) { \
    if (set->capacity * CU_US_LOAD_FACTOR < set->size) { \
      uset_##name##_rehash(set, set->capacity << 1); \
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
  scope size_t uset_##name##_find(struct USet_##name *set, elem_t elem) { \
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
  scope bool uset_##name##_contains(struct USet_##name *set, elem_t elem) { \
    return uset_##name##_find(set, elem) != set->capacity; \
  } \
  scope bool uset_##name##_remove(struct USet_##name *set, elem_t elem) { \
    size_t i = uset_##name##_find(set, elem); \
    if (i == set->capacity) { \
      return false; \
    } \
    size_t ix = __us_flag_index(i); \
    size_t offset = __us_flag_offset(i); \
    set->flags[ix] = __us_set_del(set->flags[ix], offset); \
    --set->size; \
    return true; \
  } \
  scope void _uset_##name##_debug_print(struct USet_##name *set, void (*print_hash)(hash_t), void (*print_elem)(elem_t)) { \
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
  scope size_t uset_##name##_begin(struct USet_##name *set) {\
    size_t last = -1; \
    uset_##name##_next(set, &last); \
    return last; \
  } \
  \
  scope size_t uset_##name##_end(struct USet_##name *set) { \
    size_t last = set->capacity; \
    return last; \
  } \
  \
  scope void uset_##name##_next(struct USet_##name *set, size_t *itr) {\
    size_t next = *itr+1; \
    while (next < set->capacity) { \
      size_t ix = __us_flag_index(next); \
      size_t offset = __us_flag_offset(next); \
      uint8_t f = __us_flag_get(set->flags[ix], offset); \
      if (f == US_FLAG_OCC) {\
        break; \
      } \
      ++next; \
    } \
    *itr = next; \
    return; \
  } \
  \
  scope void uset_##name##_prev(struct USet_##name *set, size_t *itr) { \
    size_t last = *itr; \
    while (last > 0) { \
      --last; \
      size_t ix = __us_flag_index(last); \
      size_t offset = __us_flag_offset(last); \
      uint8_t f = __us_flag_get(set->flags[ix], offset); \
      if (f == US_FLAG_OCC) {\
        break; \
      } \
    }\
    *itr = last; \
    return; \
  } \
  \
  scope bool uset_##name##_eq(struct USet_##name *a, struct USet_##name *b) { \
    if (a->size != b->size) { \
      return false; \
    } \
    for (size_t itr = uset_##name##_begin(a); \
        itr != uset_##name##_end(a); \
        uset_##name##_next(a, &itr) \
        ) { \
      elem_t *elem = &a->elems[itr]; \
      if (!uset_##name##_contains(a, *elem)) { \
        return false; \
      } \
    } \
    return true; \
  } \



#define UNORDERED_SET_DECLARE(name, scope, elem_t, hash_t) \
  UNORDERED_SET_TYPE(name, elem_t, hash_t) \
  UNORDERED_SET_SPEC(name, scope, elem_t, hash_t)


#define UNORDERED_SET_INIT(name, scope, elem_t, hash_t, _hash_f, _eq_f) \
  UNORDERED_SET_DECLARE(name, scope, elem_t, hash_t) \
  UNORDERED_SET_IMPL(name, scope, elem_t, hash_t, _hash_f, _eq_f) \



#endif
