#include "hashtable.h"
#include <math.h>
#include <string.h>
#include <stdio.h>


void ht_mk(hashtable_t* htable, int (*hash_fn)(void*), size_t key_sz, size_t value_sz, int capacity) {
  htable->hash_fn = hash_fn;
  htable->capacity = capacity;
  htable->size = 0;
  htable->key_sz = key_sz;
  htable->value_sz = value_sz;
  htable->mask = calloc(ceil((double)capacity/sizeof(int)), sizeof(int));
  htable->keys = malloc(capacity*key_sz);
  htable->values = malloc(capacity*value_sz);
}


void ht_rm(hashtable_t* htable) {
  free(htable->mask);
  free(htable->keys);
  free(htable->values);
}


bool ht_lookup(hashtable_t* htable, void* key, void** value) {
  int k, ix, found;
  k = htable->hash_fn(key);
  found = false;
  for (int i=0; i<htable->capacity && !found; ++i) {
    ix = (k + i*i)%htable->capacity;
    bool occupied = (htable->mask[ix/sizeof(int)]>>ix%sizeof(int))&1;
    if (occupied && !memcmp(htable->keys+ix*htable->key_sz, key, htable->key_sz))
      found = true;
  }
  if (!found)
    return false;
  *value = htable->values+ix*htable->value_sz;
  return true;
}

void ht_rehash(hashtable_t* ht, int sz) {
  printf("UNIMPLEMENTED\n");
  exit(0);
}

void ht_insert(hashtable_t* htable, void* key, void* value) {
  int k, ix;
  k = htable->hash_fn(key);
  for (int i=0; i<htable->capacity; ++i) {
    ix = (k + i*i)%htable->capacity;
    if (!((htable->mask[ix/sizeof(int)]>>ix%sizeof(int))&1))
      break;
  }
  htable->mask[ix/sizeof(int)] |= (1<<ix%sizeof(int));
  memcpy(htable->keys+ix*htable->key_sz, key, htable->key_sz);
  memcpy(htable->values+ix*htable->value_sz, value, htable->value_sz);
  ++htable->size;
  if ((double)htable->capacity*0.75 <= (double) htable->size)
    ht_rehash(htable, htable->capacity*2);
  //printf("I: %d(%d):%d[%d|%d]\n", ix, k, (htable->mask[ix/sizeof(int)]>>ix%sizeof(int))&1, *(int*)value, *(int*)(htable->values+ix*htable->value_sz));
}


void ht_del(hashtable_t* htable, void* key) {
  int k, ix, found;
  k = htable->hash_fn(key);
  found = false;
  for (int i=0; i<htable->capacity && !found; ++i) {
    ix = (k + i*i)%htable->capacity;
    if ((htable->mask[ix/sizeof(int)]>>ix%sizeof(int))&1 &&
       !memcmp(htable->keys+ix*htable->key_sz, key, htable->key_sz)) {
      htable->mask[ix/sizeof(int)] &= ~(1<<ix%sizeof(int));
      found = true;
      --htable->size;
    }
  }
}

void ht_set(hashtable_t* htable, void* key, void* value) {
  int k, ix, found;
  k = htable->hash_fn(key);
  found = 0;
  for (int i=0; i<htable->capacity && !found; ++i) {
    ix = (k + i*i)%htable->capacity;
    if ((htable->mask[ix/sizeof(int)]>>ix%sizeof(int))&1 &&
       !memcmp(htable->keys+ix*htable->key_sz, key, htable->key_sz))
      found = 1;
  }
  if (!found)
    return;
  memcpy(htable->values+ix*htable->value_sz, value, htable->value_sz);
}

void ht_iters(hashtable_t* htable, void** begin, void** end) {
  printf("UNIMPLEMENTED\n");
  exit(0);
}
