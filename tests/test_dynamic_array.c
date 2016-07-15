#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "dynamic_array.h"

void test() {
  struct DynArray arr;
  da_DynArray_init(&arr, 0, sizeof(int));
  for (int i=0; i<1000; ++i) {
    if (i < 500)
      da_append(&arr, &i);
    else
      da_pop(&arr);
  }
  assert(arr.size == 0);
  da_DynArray_del(&arr);
  da_DynArray_init(&arr, 8, sizeof(char));
  for (int i=0; i<1000; ++i) {
    char c = 'J';
    if (!(i%2))
      da_append(&arr, &c);
    else
      da_pop(&arr);
  }
  assert(arr.size == 0);
  da_DynArray_del(&arr);
}

int main(int argc, char** argv) {
  test();
}
