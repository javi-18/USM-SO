CFLAGS = -g -Wall

all: programa

programa: main.o
	gcc -o programa main.o

main.o: main.c
	gcc $(CFLAGS) -c main.c