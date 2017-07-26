# Generic make file for off development.

CC=cc
CFLAGS=-s -Wall -Werror
LIB=$(wildcard ../*.c)

off-examples:
	$(CC) $(CFLAGS) $(LIB) dumpoff.c -o dumpoff
	$(CC) $(CFLAGS) $(LIB) readoff.c -o readoff
	$(CC) $(CFLAGS) $(LIB) simple.c -o simple

all: off-examples

clean:
	rm -f dumpoff readoff simple

.PHONY: dumpoff readoff
