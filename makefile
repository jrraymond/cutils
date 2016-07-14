IDIR=.
BUILD=build
CC=gcc
CFLAGS=-lm -I$(IDIR) -Wall -std=c99 -g -DDEBUG=1 -Wno-missing-braces
OUTPUT=libcutils.a

.PHONY: all
	all: $(OUTPUT)

.c.o: 
	$(CC) -c $(CFLAGS) $<

$(OUTPUT): dynamic_array.o hashtable.o hash_functions.o bitarray.o
	ar ru $@ $^
	ranlib $@

test: unordered_set.h
	$(CC) test_unordered_set.c -o test_unordered_set.out $(CFLAGS)

test_bitarray: bitarray.o
	$(CC) test_bitarray.c -o test_bitarray.out bitarray.o $(CFLAGS)


.PHONY: clean
clean:
	rm -f *.o *.a *.out
