#include "unordered_set.h"
#include "hash_functions.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>


bool uint32_eq(uint32_t a, uint32_t b) {
  return a == b;
}

void uint32_print(uint32_t h) {
  printf("%d", h);
}

UNORDERED_SET_INIT(u32, static inline, uint32_t, uint32_t, uint32_hash, uint32_eq)

void test_unordered_set() {
  const int s = 1024;
  struct USet_u32 set;
  uset_u32_init(&set, 8);

  printf("INSERT\n");

  for (int i=0; i<s/2; ++i) {
    uset_u32_insert(&set, i);
  }
  
  _uset_u32_debug_print(&set, &uint32_print, &uint32_print);

  for (int i=0; i<s/2; ++i) {
    printf("%d,", i);
    assert(uset_u32_contains(&set, i));
  }
  for (int i=s/2; i<s; ++i) {
    assert(!uset_u32_contains(&set, i));
  }

  printf("\nREMOVE\n");
  for (int i=0; i<s/4; ++i) {
    uset_u32_remove(&set, i);
  }
  _uset_u32_debug_print(&set, &uint32_print, &uint32_print);

  for (int i=0; i<s/4; ++i) {
    printf("r:%d,", i);
    assert(!uset_u32_contains(&set, i));
  }
  printf("\n");
  for (int i=s/4; i<s/2; ++i) {
    assert(uset_u32_contains(&set, i));
  }

  uset_u32_del(&set);
}

int main(int argc, char** argv) {
  test_unordered_set();


  return EXIT_SUCCESS;
}




