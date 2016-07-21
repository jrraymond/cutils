#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>

#include <time.h>

#include "array.h"

CUTILS_ARRAY(u8, static inline, uint8_t)


void test_array_append() {
  struct Array_u8 arr;
  array_u8_init(&arr, 0);

  assert(array_u8_size(&arr) == 0);

  for (uint8_t i=0; i<UINT8_MAX; ++i)
    array_u8_append(&arr, i);

  assert(array_u8_size(&arr) == UINT8_MAX);

  for (uint8_t i=0; i<UINT8_MAX; ++i)
    assert(array_u8_get(&arr, i) == i);

  array_u8_del(&arr);
}

int main(int argc, char **argv)
{
  test_array_append();

  return EXIT_SUCCESS;
}

