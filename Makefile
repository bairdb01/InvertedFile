CC = gcc
CFLAGS = -Wall -std=c99 -O3 -g

default: offline online

# Merge binary tree and linked list objects with invertedFile
offline: invertedFileOffline.c list.o tree.o
	$(CC) list.o tree.o invertedFileOffline.c $(CFLAGS) -o bairdb_a4_off

# Compile the binary tree object
tree.o: list.h tree.c tree.h list.c
	$(CC) $(CFLAGS) -c tree.c 

#Compile the linked list object
list.o: list.c list.h
	$(CC) $(CFLAGS) -c list.c

# Compile the online portion
online: invertedFileOnline.c indexes.o
	$(CC) $(CFLAGS) invertedFileOnline.c indexes.o -o bairdb_a4_on -lm
	
indexes.o: indexes.c indexes.h
	$(CC) $(CFLAGS) -c indexes.c

# Remove the created indexes
reset:
	rm dictionary.txt
	rm postings.txt
	rm docids.txt

# Clean up created object files
clean:
	rm *.o
	rm bairdb_a4_on
	rm bairdb_a4_off
