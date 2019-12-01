CC=gcc

CFLAGS=-Wall -Wextra --std=gnu99 -pedantic -Werror

all: head xargs

xargs: xargs.c
	$(CC) $(CFLAGS) -o xargs xargs.c

head: head.c
	$(CC) $(CFLAGS) -o head head.c

