SRCDIR=..
BUILD=build
CC=gcc
CFLAGS=-lm -I$(SRCDIR) -Wall -std=c99 -g -DDEBUG=1 -Wno-missing-braces
OUTPUT=$(SRCDIR)/libcutils.a


.PHONY: all
all:
	$(MAKE) test_unordered_set
	$(MAKE) test_matrix
	$(MAKE) test_bitarray
	$(MAKE) test_dense_graph
	$(MAKE) test_hashtable
	$(MAKE) test_dynamic_array

.PHONY: run_tests
run_tests: all
	./test_bitarray.out
	./test_matrix.out
	./test_dense_graph.out
	./test_unordered_set.out


test_unordered_set: $(OUTPUT)
	$(CC) test_unordered_set.c -o test_unordered_set.out $(OUTPUT) $(CFLAGS)

test_matrix: $(SRCDIR)/matrix.h
	$(CC) test_matrix.c -o test_matrix.out $(CFLAGS)

test_bitarray: $(OUTPUT)
	$(CC) test_bitarray.c -o test_bitarray.out $(OUTPUT) $(CFLAGS)

test_dense_graph: $(OUTPUT)
	$(CC) test_dense_graph.c -o test_dense_graph.out $(OUTPUT) $(CFLAGS)

test_dynamic_array: $(OUTPUT)
	$(CC) test_dynamic_array.c -o test_dynamic_array.out $(OUTPUT) $(CFLAGS)

test_hashtable: $(OUTPUT)
	$(CC) test_hashtable.c -o test_hashtable.out $(OUTPUT) $(CFLAGS)

test_array:
	$(CC) test_array.c -o test_array.out $(CFLAGS)

.PHONY: clean
clean:
	rm -rf *.o *.a *.out *.dSYM
