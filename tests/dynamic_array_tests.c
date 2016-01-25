#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "dynamic_array.h"

void test() {
  darray_t arr;
  mk_darray(&arr, 1, sizeof(int));
  for (int i=0; i<1000; ++i) {
    if (i < 500)
      append(&arr, &i);
    else
      pop(&arr);
  }
  assert(arr.size == 0);
  rm_darray(&arr);
  mk_darray(&arr, 8, sizeof(char));
  for (int i=0; i<1000; ++i) {
    char c = 'J';
    if (!(i%2))
      append(&arr, &c);
    else
      pop(&arr);
  }
  assert(arr.size == 0);
  rm_darray(&arr);
}

int main(int argc, char** argv) {
  test();
}
