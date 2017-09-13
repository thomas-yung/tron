# Define the compiler
CC = gcc

# Define the compilation flags (-g -O0 are for Valgrind)
CFLAGS = -c -g -O0 -Wall -Werror -pedantic -std=c99 -lSDL2

# Define linking flags (-lSDL2 is for the graphics library, SDL)
LFLAGS = -lSDL2

all: tron

tron: main.o util.o human.o robot.o board.o
	$(CC) $^ $(LFLAGS) -o $@

main.o: main.c definitions.h util.o
	$(CC) $(CFLAGS) main.c

util.o: util.c util.h
	$(CC) $(CFLAGS) util.c

human.o: human.c human.h util.o
	$(CC) $(CFLAGS) human.c

robot.o: robot.c robot.h util.o
	$(CC) $(CFLAGS) robot.c

board.o: board.c board.h util.o
	$(CC) $(CFLAGS) board.c

clean:
	rm -f *.o
	rm -f tron

.PHONY: clean
