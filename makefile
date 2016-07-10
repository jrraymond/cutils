IDIR=.
BUILD=build
CC=clang
CFLAGS=-I$(IDIR) -Wall -std=c99 -g -DDEBUG=1 -Wno-missing-braces
OUTPUT=libcutils.a

.PHONY: all
	all: $(OUTPUT)

.c.o: 
	$(CC) -c $(CFLAGS) $<

$(OUTPUT): dynamic_array.o hashtable.o
	ar ru $@ $^
	ranlib $@

test: unordered_set.h
	$(CC) test_unordered_set.c $(CFLAGS)


.PHONY: clean
clean:
	rm -f *.o *.a
