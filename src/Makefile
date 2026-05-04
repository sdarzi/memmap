CC = gcc
CFLAGS = -Wall -Wextra

.PHONY: clean

memmap: main.o regions.o
	$(CC) $(CFLAGS) -o memmap main.o regions.o


main.o: main.c regions.h
	$(CC) $(CFLAGS) -c main.c


regions.o: regions.c regions.h
	$(CC) $(CFLAGS) -c regions.c 

clean: 
	rm -f *.o memmap

