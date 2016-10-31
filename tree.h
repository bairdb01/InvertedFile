/***
    Filename: tree.h
    Author: Benjamin Baird
    Description: Header file for tree.c, a binary tree converted to AVL
***/

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


#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED
#include "list.h"
#endif

typedef struct TreeNode {
    int freq;
    int height;
    char *term;
    Node *dictionary;
    struct TreeNode *right;
    struct TreeNode *left;
}TreeNode;

/***
    Initializes a node with a given id
    @return : pointer to the created node
***/
TreeNode *initTreeNode (char *term, char *docId);

/***
    Frees the entire tree
***/
void freeTree (TreeNode *tree);

/***
    Frees up a TreeNode
***/
void freeTreeNode (TreeNode *node);

/***
    Adds node using binary search method
    @return 1 : successful
    @return 0 : failure
***/
TreeNode * addTerm (TreeNode *tree, char *term, char *docId);

/***
    Prints a tree node including left and right terms
    @return 1 : succesful print
    @return 0 : Null node
***/
int printTreeNode (TreeNode *node);

/***
    Print the linked list alphabetically (left-depth-first)
    @return 1 : successful print
***/
int printTree (TreeNode *tree);

/**
    Prints the tree inorder, helpful to debug AVL tree
**/
int printInOrder(TreeNode *tree);

/***
    Count the number of nodes in the tree
    @return >0 : number of nodes in the tree
***/
int countTreeNodes (TreeNode *tree);

/***
    Search the tree for a term
    @return : pointer to term's TreeNode in the tree
              NULL if not found
***/
TreeNode *searchTree(TreeNode *tree, char *term);

/**
    Returns the height of a node
**/
int getHeight(TreeNode *node);

/**
    Checks if a node is balanced
    @return > 1 left heavy, < -1 right heavy
**/
int getBalance(TreeNode *node);

/**
    Rotate a sub tree left
**/
TreeNode *rotateLeft(TreeNode *node);

/**
    Rotate a subtree right
**/
TreeNode *rotateRight(TreeNode *node);

/**
    Balances out a subtree
**/
TreeNode *selfBalance(TreeNode *node, char *term);
