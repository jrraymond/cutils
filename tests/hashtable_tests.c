#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "hashtable.h"

int hash(void* p) {
	int x = * (int*) p;
	x = ((x >> 16) ^ x) * 0x45d9f3b;
	x = ((x >> 16) ^ x) * 0x45d9f3b;
	x = ((x >> 16) ^ x);
	return x;
}
void test() {
	size_t sz = 100;
	int* checker = malloc(sz*sizeof(int));
  for (int i=0; i<sz; ++i)
    checker[i] = -1;
  hashtable_t ht;
  ht_mk(&ht, &hash, sizeof(int), sizeof(int), sz);
  printf("HASHTABLE: size[%d], capacity[%d]\n", ht.size, ht.capacity);
  printf("INSERTIONS\n");
	for (int i=0; i<sz; i+=3) { 	//insert multiples of 3
    int** vp;
		ht_insert(&ht, &i, &i);
    //bool found = ht_lookup(&ht, &i, (void**) vp);
		checker[i] = i;
	}
	for (int i=0; i<sz; ++i) { 		//check insertions and lookup
		int* vp;
		bool found = ht_lookup(&ht, &i, (void**) &vp);
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
		bool found = ht_lookup(&ht, &i, (void**) &vp);
		assert((!found && checker[i] == -1) || (found && *vp == checker[i]));
	}
  printf("HASHTABLE: size[%d], capacity[%d]\n", ht.size, ht.capacity);
  printf("DEL\n");
	for (int i=0; i<sz/10; i += 3) { 	//del everything
		int v = 0;
		ht_del(&ht, &i);
		checker[i] = -1;
	}
  printf("HASHTABLE: size[%d], capacity[%d]\n", ht.size, ht.capacity);
	for (int i=0; i<sz; ++i) {			//check deletes
		int* vp;
		bool found = ht_lookup(&ht, &i, (void**) &vp);
		assert((!found && checker[i] == -1) || (found && *vp == checker[i]));
	}
  ht_rm(&ht);
}

int main(int argc, char** argv) {
  test();
}
