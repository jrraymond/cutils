IDIR=.
BUILD=build
CC=clang
CFLAGS=-I$(IDIR) -Wall -std=c99 -g -DDEBUG=1 -Wno-missing-braces
OUTPUT=cutils.a
.PHONY: all
	all: $(OUTPUT)

.c.o: 
	$(CC) -c $(CFLAGS) $<

$(OUTPUT): dynamic_array.o hashtable.o
	ar ru $@ $^
	ranlib $@


.PHONY: clean
clean:
	rm -f *.o *.a
