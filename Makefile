CC = gcc
CFLAGS = -Wall -std=c99 -g

default: offline

# Merge binary tree and linked list objects with invertedFile
offline: invertedFileOffline.c list.o tree.o
	$(CC) list.o tree.o invertedFileOffline.c $(CFLAGS) -o bairdb_a4_off.out

# Compile the binary tree object
tree.o: list.h tree.c tree.h list.c
	$(CC) $(CFLAGS) -c tree.c 

#Compile the linked list object
list.o: list.c list.h
	$(CC) $(CFLAGS) -c list.c

#online: invertedFileOnline.c 
#	$(CC) $(CFLAGS) -o bairdb_a4_on.out

# Clean up created object files
clean:
	rm *.o
