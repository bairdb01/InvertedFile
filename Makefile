CC = gcc
CFLAGS = -Wall -std=c99 -g

default: offline


offline: invertedFileOffline.c list.o tree.o
	$(CC) list.o tree.o invertedFileOffline.c $(CFLAGS) -o bairdb_a4_off.out

tree.o: list.h tree.c tree.h list.c
	$(CC) -c tree.c 

list.o: list.c list.h
	$(CC) -c list.c
	
#online: invertedFileOnline.c 
#	$(CC) -o bairdb_a4_on.out

clean:
	rm *.o *.out
