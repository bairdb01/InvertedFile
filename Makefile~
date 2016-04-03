HEADERS = list.h tree.h

default: offline

offline: invertedFileOffline.c tree.c list.c $(HEADERS)
	gcc -o bairdb_a4_off.out list.c tree.c invertedFileOffline.c

#online: invertedFileOnline.c 
#	gcc -o bairdb_a4_on.out

