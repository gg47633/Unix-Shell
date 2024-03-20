CC=gcc
CFLAGS=-I.
SOURCES=wish.c utility.c builtIn.c

all: wish

wish: wish.c utility.c builtIn.c
	$(CC) $(CFLAGS) wish.c utility.c builtIn.c -o wish
clean:
	rm -f wish
