#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#include "bitarray.h"



void test_bitarray() {
  struct BitArray ba;

  ba_init(&ba, 50);

  for (int i=0; i<50; ++i)
    ba_set(&ba, i, (bool) i%3);

  for (int i=0; i<50; ++i) 
    assert(ba_get(&ba, i) == (bool) i%3);

  ba_resize(&ba, 100);

  for (int i=50; i<100; ++i)
    ba_set(&ba, i, (bool) i%3);

  for (int i=0; i<100; ++i) 
    assert(ba_get(&ba, i) == (bool) i%3);

  ba_del(&ba);
  printf("DONE\n");
}


int main(int argc, char **argv) {

  test_bitarray();

  return EXIT_SUCCESS;
}
