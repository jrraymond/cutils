#ifndef __HASHTABLE_H
#define __HASHTABLE_H
#include <stdlib.h>
#include <stdbool.h>

typedef struct hashtable_t {
  int (*hash_fn)(void*);
  int capacity;
  int size;
  size_t key_sz;
  size_t value_sz;
  int* flags;
  void* keys;
  void* values;
} hashtable_t;

void ht_mk(hashtable_t* htable, int (*hash_fn)(void*), size_t key_sz, size_t value_sz, int capacity);

void ht_rm(hashtable_t* htable);

bool ht_lookup(hashtable_t* htable, void* key, void** value);

void ht_insert(hashtable_t* htable, void* key, void* value);

void ht_del(hashtable_t* htable, void* key);

void ht_set(hashtable_t* htable, void* key, void* value);

void ht_iters(hashtable_t* htable, void** begin, void** end);

void ht_rehash(hashtable_t* ht, int sz);

static const int ht_empty = 0;
static const int ht_dummy = 1;
static const int ht_active = 2;

int ht_status(int* flags, int i);
void ht_setstatus(int* flags, int i, int st);
#endif