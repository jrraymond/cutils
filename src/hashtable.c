#include "hashtable.h"
#include <math.h>
#include <string.h>
#include <stdio.h>

/* A bucket can be in one of three states: empty, dummy, active.
 *  empty: no value in the bucket
 *  dummy: no value in bucket but should continue probing
 *  active: value in the bucket
 */

int ht_status(int* flags, int i) {
  return (flags[2*i/sizeof(int)]>>(2*i%sizeof(int)))&0x3;
}
void ht_setstatus(int* flags, int i, int st) {
  int mask = 0x3 << 2*i%sizeof(int);
  st = st << 2*i%sizeof(int);
  flags[2*i/sizeof(int)] = (flags[2*i/sizeof(int)] & ~mask) | (st & mask);
}

void ht_mk(hashtable_t* htable, int (*hash_fn)(void*), size_t key_sz, size_t value_sz, int capacity) {
  htable->hash_fn = hash_fn;
  htable->capacity = capacity;
  htable->size = 0;
  htable->key_sz = key_sz;
  htable->value_sz = value_sz;
  htable->flags = calloc(2*ceil((double)capacity/sizeof(int)), sizeof(int));
  htable->keys = malloc(capacity*key_sz);
  htable->values = malloc(capacity*value_sz);
}


void ht_rm(hashtable_t* htable) {
  free(htable->flags);
  free(htable->keys);
  free(htable->values);
}


bool ht_lookup(hashtable_t* htable, void* key, void** value) {
  int k, ix, found;
  k = htable->hash_fn(key);
  found = false;
  for (int i=0; i<htable->capacity && !found; ++i) {
    ix = (k + i*i)%htable->capacity;
    int status = ht_status(htable->flags, ix);
    if (status == ht_active && !memcmp(htable->keys+ix*htable->key_sz, key, htable->key_sz)) {
      found = true;
    } else if (status == ht_empty) {
      return false;
    }
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
    if (ht_status(htable->flags, ix) != ht_active)
      break;
  }
  ht_setstatus(htable->flags, ix, ht_active);
  memcpy(htable->keys+ix*htable->key_sz, key, htable->key_sz);
  memcpy(htable->values+ix*htable->value_sz, value, htable->value_sz);
  ++htable->size;
  if ((double)htable->capacity*0.75 <= (double) htable->size)
    ht_rehash(htable, htable->capacity*2);
  //printf("I: %d(%d):%d[%d|%d]\n", ix, k, (htable->flags[ix/sizeof(int)]>>ix%sizeof(int))&1, *(int*)value, *(int*)(htable->values+ix*htable->value_sz));
}


void ht_del(hashtable_t* htable, void* key) {
  int k, ix, found;
  k = htable->hash_fn(key);
  found = false;
  for (int i=0; i<htable->capacity && !found; ++i) {
    ix = (k + i*i)%htable->capacity;
    if (ht_status(htable->flags, ix) == ht_active &&
       !memcmp(htable->keys+ix*htable->key_sz, key, htable->key_sz)) {
      ht_setstatus(htable->flags, ix, ht_dummy);
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
    if (ht_status(htable->flags, ix) == ht_active &&
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
