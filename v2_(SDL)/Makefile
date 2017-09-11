# Define the compiler
CC = gcc

# Define the compilation flags (-g -O0 are for Valgrind)
CFLAGS = -c -g -O0 -Wall -Werror -pedantic -std=c99

all: tron

tron: main.o util.o
	$(CC) main.o util.o -o tron

main.o: main.c definitions.h util.h
	$(CC) $(CFLAGS) main.c

util.o: util.c util.h
	$(CC) $(CFLAGS) util.c

clean:
	rm -f *.o
	rm -f tron

.PHONY: clean
