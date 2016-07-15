#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <assert.h>
#include "hashtable.h"

unsigned int int_hash(void* p) {
	int x = * (int*) p;
	x = ((x >> 16) ^ x) * 0x45d9f3b;
	x = ((x >> 16) ^ x) * 0x45d9f3b;
	x = ((x >> 16) ^ x);
	return x;
}

bool int_cmp(void* a, void *b) {
  return ((int*) a) == ((int*) b);
}

void test_flags() {
  int* flags = calloc(2, sizeof(int));
  for (int i=0; i<sizeof(int); ++i)
    assert(ht_status(flags, i) == ht_empty);
  for (int i=0; i<sizeof(int); i+=2)
    ht_setstatus(flags, i, ht_active);
  for (int i=1; i<sizeof(int); i+=2)
    ht_setstatus(flags, i, ht_dummy);
  for (int i=0; i<sizeof(int); i+=2)
    assert(ht_status(flags, i) == ht_active);
  for (int i=1; i<sizeof(int); i+=2)
    assert(ht_status(flags, i) == ht_dummy);
  free(flags);
  int sz = 100;
  flags = calloc(2*ceil(sz/(double)sizeof(int)), sizeof(int));
  int n = 0;
  for (int i=0; i<sz; i+=2)
    ht_setstatus(flags, i, ht_active);
  for (int i=0; i<sz; ++i) {
    if (ht_status(flags, i) == ht_active)
      ++n;
  }
  assert(n == 50);
  free(flags);
  printf("HASHTABLE STATUS TEST COMPLETE\n");
}

void test_ops() {
	size_t sz = 100;
	int* checker = malloc(sz*sizeof(int));
  for (int i=0; i<sz; ++i)
    checker[i] = -1;
  struct Hashtable ht;
  ht_init(&ht, &int_hash, &int_cmp, sizeof(int), sizeof(int), sz);
  printf("HASHTABLE: size[%d], capacity[%d]\n", ht.size, ht.capacity);
  printf("INSERTIONS\n");
	for (int i=0; i<sz; i+=3) { 	//insert multiples of 3
		ht_insert(&ht, &i, &i);
		checker[i] = i;
	}
	for (int i=0; i<sz; ++i) { 		//check insertions and lookup
		int* vp;
		bool found = ht_get_ref(&ht, &i, (void**) &vp);
		assert((!found && checker[i] == -1) || (found && *vp == checker[i]));
	}
  printf("HASHTABLE: size[%d], capacity[%d]\n", ht.size, ht.capacity);
  printf("SET\n");
	for (int i=0; i<sz; i += 6) { 	//set multiples of 2 to 0
		int v = 0;
		ht_set(&ht, &i, &v);
    checker[i] = 0;
	}
	for (int i=0; i<sz; ++i) {			//check sets
		int* vp;
		bool found = ht_get_ref(&ht, &i, (void**) &vp);
		assert((!found && checker[i] == -1) || (found && *vp == checker[i]));
	}
  printf("HASHTABLE: size[%d], capacity[%d]\n", ht.size, ht.capacity);
  printf("DEL\n");
	for (int i=0; i<sz/10; i += 3) { 	//del everything
		ht_rm(&ht, &i);
		checker[i] = -1;
	}
  printf("HASHTABLE: size[%d], capacity[%d]\n", ht.size, ht.capacity);
	for (int i=0; i<sz; ++i) {			//check deletes
		int* vp;
		bool found = ht_get_ref(&ht, &i, (void**) &vp);
		assert((!found && checker[i] == -1) || (found && *vp == checker[i]));
	}
  ht_del(&ht);
  printf("HASHTABLE INSERT/LOOKUP/DELETE TEST COMPLETE\n");
}

void test_resizing() {
  struct Hashtable ht;
  ht_init(&ht, &int_hash, &int_cmp, sizeof(int), sizeof(int), 1);
  printf("HASHTABLE: size[%d], capacity[%d]\n", ht.size, ht.capacity);
  for (int i=0; i<100; ++i) {
    ht_insert(&ht, &i, &i);
    assert(ht.size == i+1);
  }
  printf("HASHTABLE: size[%d], capacity[%d]\n", ht.size, ht.capacity);
  for (int i=99; i>=0; --i) {
    ht_rm(&ht, &i);
    assert(ht.size == i);
  }
  printf("HASHTABLE: size[%d], capacity[%d]\n", ht.size, ht.capacity);
  ht_del(&ht);
  printf("HASHTABLE RESIZING TEST COMPLETE\n");
}

int main(int argc, char** argv) {
  test_flags();
  test_ops();
  test_resizing();
}
