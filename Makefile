CC = gcc
CFLAGS = -Wall -pedantic -std=c99

default = s2s

$(default): main.o
	$(CC) $(CFLAGS) -o $(default) main.o

main.o: main.c main.h
	$(CC) $(CFLAGS) -c main.c

clean:
	$(RM) $(default) *.o *~
