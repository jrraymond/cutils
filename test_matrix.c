#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>


#include "matrix.h"

MATRIX_INIT(u8, uint8_t)

MATRIX_INIT(char, char)


void test_get_set() {
  struct Matrix_u8_t m;
  matrix_u8_init(&m, 8, 8);
  for (uint8_t i=0; i<8; ++i) {
    for (uint8_t j=0; j<8; ++j) {
      matrix_u8_set(&m, i, j, i*j);
    }
  }
  for (uint8_t i=0; i<8; ++i) {
    for (uint8_t j=0; j<8; ++j) {
      uint8_t x;
      matrix_u8_get(&m, i, j, &x);
      assert(x == i*j);
    }
  }
  matrix_u8_del(&m);
  printf("TEST SET/GET COMPLETE\n");
}

void test_resize() {
  struct Matrix_char_t m;
  matrix_char_init(&m, 4, 4);

  for (uint8_t i=0; i<4; ++i) {
    for (uint8_t j=0; j<4; ++j) {
      matrix_char_set(&m, i, j, 'A');
    }
  }

  printf("TEST RESIZING UP\n");
  matrix_char_resize(&m, 8, 8);

  for (uint8_t i=0; i<4; ++i) {
    for (uint8_t j=0; j<4; ++j) {
      char x;
      matrix_char_get(&m, i, j, &x);
      assert(x == 'A');
    }
  }

  printf("TEST RESIZING DOWN\n");
  matrix_char_resize(&m, 4, 4);

  for (uint8_t i=0; i<4; ++i) {
    for (uint8_t j=0; j<4; ++j) {
      char x;
      matrix_char_get(&m, i, j, &x);
      assert(x == 'A');
    }
  }

  matrix_char_del(&m);

  printf("TEST RESIZING COMPLETE\n");
}



int main(int argc, char** argv) {

  test_get_set();

  test_resize();

  return EXIT_SUCCESS;
}
