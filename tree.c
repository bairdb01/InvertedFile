/***
    Filename: tree.c
    Date Created: April 1, 2016
    Author Benjamin Baird
    Date Updated: April 1, 2016
    Description: Implementation of a binary search tree where each node contains
                 a string and a linked list of Nodes (contain strings). 

    Tested: 0 memory leaks and errors
***/

#ifndef TREE_H_INCLUDED
#define TREE_H_INCLUDED
#include "tree.h"
#endif

TreeNode *initTreeNode (char *term, char *docId) {
    TreeNode *node = malloc(sizeof(TreeNode));
    node->freq = 1;
    node->term = malloc(sizeof(char)*((int)strlen(term)+1));
    node->term = strcpy(node->term, term);
    node->dictionary = initNode(docId);
    node->left = NULL;
    node->right = NULL;
    return node;
}

void freeTree (TreeNode *tree) {
    TreeNode * temp = tree;
    while (temp != NULL) {
        if (temp->left != NULL)    
            freeTree(temp->left);
        if (temp->right != NULL) 
            freeTree(temp->right);
        freeTreeNode(temp);
        temp = NULL;
    }
}

void freeTreeNode (TreeNode *node) {
    freeNodeList(node->dictionary);
    free(node->term);
    free(node);
}

int addTerm (TreeNode *tree, char *term, char *docId) {
    TreeNode *treeNode = searchTree(tree, term);

    // Check if term exists already
    if (treeNode == NULL) {
        // Term doesn't exist, create and add it to the tree
        TreeNode *temp = tree;
        while (1) {
            if (strcmp(term, temp->term) < 0) {
                if (temp->left != NULL) {
                    temp = temp->left;
                    continue;
                } else {
                    temp->left = initTreeNode(term, docId);
                    return 1;
                }
            } else if (strcmp(term, temp->term) > 0) {
                if (temp->right != NULL) {
                    temp = temp->right;
                    continue;
                } else {
                    temp->right = initTreeNode(term, docId);
                    return 1;
                }
            } else if (strcmp(term, temp->term) == 0) {
                // Should never be equal since term doesn't exist
                return 0;
            }
        }
        
    } else {
        // Check if term exists in document, increment term frequency in relevant document
        treeNode->freq++;
        Node *node = searchNodes( treeNode->dictionary, docId );
        if (node == NULL) {
            // Create a node for the document
            node = initNode(docId);
            appendNode(treeNode->dictionary, node);
            return 1;
        } else {
            node->freq++;
            return 1;
        }
    }
    return 0;
}

int printTreeNode(TreeNode *node) {
    if (node == NULL) {
        printf("Null node\n");
        return 0;
    }
    printf("~~~~~~~~~~~~~~~~~~~\n");
    printf("TreeNode Term: %s\n", node->term);
    printf("Term Frequency: %d\n", node->freq);
    printNodeList(node->dictionary);
    if (node->left != NULL)
        printf("Left Child: %s\n", node->left->term);
    if (node->right != NULL)
        printf("Right Child %s\n", node->right->term);
    printf("~~~~~~~~~~~~~~~~~~~\n");
    return 1;
}

int printTree (TreeNode *tree) {
    int retVal = 0;
    // Left Side
    while (tree->left != NULL && retVal == 0)    
        retVal = printTree(tree->left);
    
    // Itself    
    printTreeNode(tree);
    retVal = 0;

    // Right Side
    while (tree->right != NULL && retVal == 0) 
        retVal = printTree(tree->right);
    
    return 1;
}

TreeNode *searchTree(TreeNode *tree, char *term) {
    TreeNode *temp = tree;
    
    // Traverse tree to find the node
    while (temp != NULL) {
        if (strcmp(term, temp->term) < 0) {
            temp = temp->left;
        } else if (strcmp(term, temp->term) > 0) {
            temp = temp->right;
        } else if (strcmp(term, temp->term) == 0) {
            // Term Found!
            return temp;
        }
    }
    return NULL;
}

int countTreeNodes (TreeNode *tree) {
    if (tree == NULL)
        return 0;
        
    int numNodes = 1;
    int retVal = 0;
    // Left Side
    while (tree->left != NULL && retVal == 0)    
        retVal = countTreeNodes(tree->left);
    numNodes += retVal;
    
    // Itself    
    retVal = 0;

    // Right Side
    while (tree->right != NULL && retVal == 0) 
        retVal = countTreeNodes(tree->right);
    numNodes += retVal;
    return numNodes;
}
/*// Test*/
/*int main () {*/
/*    printf("Testing Begins\n");*/
/*    TreeNode *tree = initTreeNode("hey\0", "doc1");*/
/*    addTerm(tree, "yo\0", "doc2");*/
/*    addTerm(tree, "abra\0", "doc1");*/
/*    addTerm(tree, "yo\0", "doc2");*/
/*    addTerm(tree, "abra\0", "doc1");*/
/*    addTerm(tree, "hey\0", "doc1");*/
/*    addTerm(tree, "yo\0", "doc1");*/
/*    addTerm(tree, "abra\0", "doc2");*/
/*    printTreeNode(searchTree(tree, "hey"));*/
/*    printTree(tree);*/
/*    freeTree(tree);*/
/*    return 0;*/
/*}*/
