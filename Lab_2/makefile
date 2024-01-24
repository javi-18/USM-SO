CFLAGS = -g -Wall

all: programa

programa: main.o
	gcc -o programa main.o

main.o: main.c tablero.h
	gcc $(CFLAGS) -c main.c