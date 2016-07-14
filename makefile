IDIR=.
BUILD=build
CC=gcc
CFLAGS=-lm -I$(IDIR) -Wall -std=c99 -g -DDEBUG=1 -Wno-missing-braces
OUTPUT=libcutils.a

.PHONY: all
	all: $(OUTPUT)

.c.o: 
	$(CC) -c $(CFLAGS) $<

$(OUTPUT): dynamic_array.o hashtable.o hash_functions.o
	ar ru $@ $^
	ranlib $@

test_unordered_set: unordered_set.h
	$(CC) test_unordered_set.c -o test_unordered_set.out $(CFLAGS)

test_matrix: matrix.h
	$(CC) test_matrix.c -o test_matrix.out $(CFLAGS)

test_dense_graph: dense_graph.h dynamic_array.o
	$(CC) test_dense_graph.c -o test_dense_graph.out $(CFLAGS) dynamic_array.o

.PHONY: tests
tests: 
	$(MAKE) test_unordered_set
	$(MAKE) test_matrix

.PHONY: clean
clean:
	rm -f *.o *.a *.out
