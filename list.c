/***
    Filename: list.c
    Author: Benjamin Baird
    Date Created: April 2, 2016
    Date Updated: April 2, 2016
    Description: Linked List where each node contains an integer and a string id
    Tested: 0 memory leaks and errors
***/

#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED
#include "list.h"
#endif

Node *initNode (char *docId) {
    Node *node = malloc(sizeof(Node));
    node->freq = 1;
    node->docId = malloc(sizeof(char)*(int)strlen(docId)+1);
    node->docId = strcpy(node->docId, docId);
    node->next = NULL;
    return node;
}

void freeNodeList (Node *node) {
    Node *temp = NULL;
    while ( node->next != NULL ) {
        temp = node->next;
        free(node->docId);
        free(node);
        node = temp;
    }
    free(node->docId);
    free(node);
}

void appendNode (Node *list, Node *nodeToAdd) {
    Node *temp = list;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = nodeToAdd;
}

void printNodeList (Node *node) {
    Node *temp = node;
    int i = 0;
    while (temp != NULL) {
        printf("Node %d: %s %d\n", i, temp->docId, temp->freq);
        temp = temp->next;
        i++;
    }
}

Node *searchNodes(Node *list, char *id){
    Node *temp = list;
    while (temp != NULL) {
        if (strcmp(temp->docId, id) == 0){
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}
