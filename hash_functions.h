#ifndef __CUTILS_HASH_FUNCTIONS
#define __CUTILS_HASH_FUNCTIONS

#include <stdint.h>

uint32_t uint32_hash(uint32_t a);

uint32_t uint32_hash_thomas_mueller(uint32_t x);

uint64_t uint64_hash_thomas_mueller(uint64_t x);

#endif
