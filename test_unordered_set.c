#include "unordered_set.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

uint32_t uint32_hash(uint32_t a) {
    a = (a ^ 61) ^ (a >> 16);
    a = a + (a << 3);
    a = a ^ (a >> 4);
    a = a * 0x27d4eb2d;
    a = a ^ (a >> 15);
    return a;
}

bool uint32_eq(uint32_t a, uint32_t b) {
  return a == b;
}

void uint32_print(uint32_t h) {
  printf("%d", h);
}

UNORDERED_SET_INIT(uint32, static, uint32_t, uint32_t, uint32_hash, uint32_eq)

void test_unordered_set() {
  const int s = 64;
  us_uint32_t set;
  us_uint32_init(&set, 32);

  printf("INSERT\n");

  for (int i=0; i<s/2; ++i)
    us_uint32_insert(&set, i);
  
  _us_uint32_debug_print(&set, &uint32_print, &uint32_print);

  for (int i=0; i<s/2; ++i) {
    printf("%d,", i);
    assert(us_uint32_contains(&set, i));
  }
  for (int i=s/2; i<s; ++i) {
    assert(!us_uint32_contains(&set, i));
  }

  printf("\nREMOVE\n");
  for (int i=0; i<s/4; ++i) {
    us_uint32_remove(&set, i);
  }
  _us_uint32_debug_print(&set, &uint32_print, &uint32_print);

  for (int i=0; i<s/4; ++i) {
    printf("r:%d,", i);
    assert(!us_uint32_contains(&set, i));
  }
  printf("\n");
  for (int i=s/4; i<s/2; ++i) {
    assert(us_uint32_contains(&set, i));
  }

  us_uint32_del(&set);
}

int main(int argc, char** argv) {
  test_unordered_set();


  return EXIT_SUCCESS;
}




