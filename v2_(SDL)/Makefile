# Define compiler and flags
CC = gcc
CFLAGS = -c -Wall -Werror -pedantic -std=c99

all: tron

tron: main.o
	$(CC) main.o -o tron

main.o: util.o main.c definitions.h util.h
	$(CC) $(CFLAGS) main.c

util.o: util.h util.c
	$(CC) $(CFLAGS) util.c

clean:
	rm -f *.o
	rm -f tron

.PHONY: clean
