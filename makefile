IDIR=.
BUILD=build
CC=gcc
CFLAGS=-lm -I$(IDIR) -Wall -std=c99 -g -DDEBUG=1 -Wno-missing-braces
OUTPUT=libcutils.a

.PHONY: all
all: *.c *.o
	$(MAKE) $(OUTPUT)

.c.o: 
	$(CC) -c $(CFLAGS) $<

$(OUTPUT): dynamic_array.o hashtable.o hash_functions.o
	ar ru $@ $^
	ranlib $@

.PHONY: tests
tests: all
	$(MAKE) $(OUTPUT)
	$(MAKE) -C tests

.PHONY: run_tests
run_tests: all
	$(MAKE) run_tests -C tests

.PHONY: clean
clean:
	rm -f *.o *.a *.out
	$(MAKE) clean -C tests
