#ifndef STDIO_H_INCLUDED
#define STDIO_H_INCLUDED
#include <stdio.h>
#endif

#ifndef STDLIB_H_INCLUDED
#define STDLIB_H_INCLUDED
#include <stdlib.h>
#endif

#ifndef STRING_H_INCLUDED
#define STRING_H_INCLUDED
#include <string.h>
#endif

typedef struct Node {
    int freq;
    char *docId;
    struct Node *next;
}Node;

/***
    Initializes a node with a given id
    @return : pointer to the created node
***/
Node *initNode (char *docId);

/***
    Frees up a linked list
***/
void freeNodeList (Node *node);

/***
    Adds node to end of the list
***/
void appendNode (Node *list, Node *nodeToAdd);

/***
    Print the linked list
***/
void printNodeList (Node *node);

/***
    Search by docId, return node index
***/
Node *searchNodes(Node *list, char *id);
