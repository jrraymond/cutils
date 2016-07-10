#include "unordered_set.h"
#include <stdlib.h>

uint32_t uint32_hash(uint32_t a) {
    a = (a ^ 61) ^ (a >> 16);
    a = a + (a << 3);
    a = a ^ (a >> 4);
    a = a * 0x27d4eb2d;
    a = a ^ (a >> 15);
    return a;
}

int uint32_eq(int a, int b) {
  return a < b ? -1 : a > b;
}

UNORDERED_SET_INIT(uint32, static, uint32_t, &uint32_hash, &uint32_eq)

int main(int argc, char** argv) {
  return EXIT_SUCCESS;
}




