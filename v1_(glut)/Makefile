CC      = gcc
CFLAGS  = -Wall -Werror -pedantic -std=c99
LFLAGS  = -lGL -lGLU -lglut -lm

.SUFFIXES: .c .o

.PHONY: clean

tron: board.o main.o player.o game.o
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

player.o: player.h board.h

board.o: board.h definitions.h

main.o: player.h game.h definitions.h board.h

game.o: game.h definitions.h

clean:
	rm -f $(wildcard *.o)
	rm -f $(wildcard .nfs*)
	rm -f tron
