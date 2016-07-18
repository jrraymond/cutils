#ifndef __CUTILS_BITARRAY_C
#define __CUTILS_BITARRAY_C

#include <stdbool.h>
#include <stdlib.h>

/** BitArray
 * size in # of bits
 */
struct BitArray {
  size_t size;
  int *bits;
};

/** Initialize a BitArray with a number of bits initially set to zero.
 */
void ba_init(struct BitArray *ba, size_t capacity) ;

/** Free memory held by a BitArray.
 */
void ba_del(struct BitArray *ba) ;

/** Resize a BitArray to a new number of bits, which may be smaller or larger
 * than the previous number of bits. The previous bits are maintained, and new
 * bits are zeroed.
 */
void ba_resize(struct BitArray *ba, size_t new_capacity);

/** Set bit at index i to b. No bounds checking.
 */
void ba_set(struct BitArray *ba, size_t i, bool b) ;

/** Set all bits to true or false
 */
void ba_set_all(struct BitArray *ba, bool b) ;

/** Set a bit, growing the capacity of the bitarray if the index is out of
 * bounds by growth_factor.
 */
void ba_set_grow(struct BitArray *ba, size_t i, bool b, double growth_factor) ;

/** Get the value of bit at index i. No bounds checking.
 */
bool ba_get(struct BitArray *ba, size_t i) ;

/** Retrieve the number of bits held in bit array.
 */
size_t ba_size(struct BitArray *ba) ;

/** Print the bit array.
 * Ex: |_|x|_|x|_|x|
 * x indicates bit set, _ indicates otherwise.
 */
void ba_print(struct BitArray *ba) ;

#endif
