/***
    Filename: tree.c
    Date Created: April 1, 2016
    Author Benjamin Baird
    Date Updated: October 29, 2016
    Description: Implementation of a binary search tree where each node contains
                 a string and a linked list of Nodes (contain strings).

    Tested: 0 memory leaks and errors
***/

#ifndef TREE_H_INCLUDED
#define TREE_H_INCLUDED
#include "tree.h"
#endif

#ifndef max
#define max(a,b)(((a) > (b)) ? (a) : (b))
#endif

TreeNode *initTreeNode (char *term, char *docId) {
    TreeNode *node = malloc(sizeof(TreeNode));
    node->freq = 1;
    node->height = 1;
    node->term = malloc(sizeof(char)*((int)strlen(term)+1));
    node->term = strcpy(node->term, term);
    node->dictionary = initNode(docId);
    node->left = NULL;
    node->right = NULL;
    return node;
}

int getBalance(TreeNode *node){
    if (node != NULL)
        return (getHeight(node->left) - getHeight(node->right));
    return 0;
}

int getHeight(TreeNode *node) {
  if (node != NULL)
      return node->height;
  return 0;
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

TreeNode *insert(TreeNode *node, char *term, char *docId) {
    if (node == NULL)
        return initTreeNode(term, docId);

    if (strcmp(term, node->term) < 0) {
        node->left = insert(node->left, term, docId);
    } else if (strcmp(term, node->term) > 0) {
        node->right = insert(node->right, term, docId);
    }

    node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
    node = selfBalance(node, term);
    return node;
}

TreeNode * addTerm (TreeNode *tree, char *term, char *docId) {
    TreeNode *treeNode = searchTree(tree, term);

    // Check if term exists already
    if (treeNode == NULL) {
        // Term doesn't exist, create and add it to the tree
        tree = insert(tree, term, docId);
    } else {
        // Check if term exists in document, increment term frequency in relevant document
        treeNode->freq++;
        Node *node = searchNodes( treeNode->dictionary, docId );
        if (node == NULL) {
            // Create a node for the document
            node = initNode(docId);
            appendNode(treeNode->dictionary, node);
            // return 1;
        } else {
            node->freq++;
            // return 1;
        }
    }
    return tree;
}

int printTreeNode(TreeNode *node) {
    if (node == NULL) {
        printf("Null node\n");
        return 0;
    }
    printf("~~~~~~~~~~~~~~~~~~~\n");
    printf("TreeNode Term: %s\n", node->term);
    printf("Term Frequency: %d\n", node->freq);
    printf("Node Height: %d\n", node->height);
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

    // Current Node
    printTreeNode(tree);
    retVal = 0;

    // Right Side
    while (tree->right != NULL && retVal == 0)
        retVal = printTree(tree->right);

    return 1;
}

int printInOrder (TreeNode *tree) {
    // Current Node
    if (tree->term != NULL)
        printf(" %s ", tree->term);
    // Left Side
    if (tree->left != NULL)
        printInOrder(tree->left);
    // Right Side
    if (tree->right != NULL)
        printInOrder(tree->right);
    return 0;
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

TreeNode *rotateRight (TreeNode *node) {
    TreeNode * temp = node->left;
    TreeNode * temp2 = temp->right;

    // Rotate
    temp->right = node;
    node->left = temp2;

    // Height update
    node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
    temp->height = max(getHeight(temp->left), getHeight(temp->right)) + 1;
    return temp;
}

TreeNode *rotateLeft (TreeNode *node){
    TreeNode * temp = node->right;
    TreeNode * temp2 = temp->left;

    // Rotate
    temp->left = node;
    node->right = temp2;

    // Height update
    node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
    temp->height = max(getHeight(temp->left), getHeight(temp->right)) + 1;
    return temp;
}

TreeNode *selfBalance(TreeNode *node,  char *term){
    int heightDif = getBalance(node);

    if (heightDif > 1 && strcmp(term, node->left->term) < 0) {
        return rotateRight(node);
    } else if (heightDif < -1 && strcmp(term, node->right->term) > 0) {
        return rotateLeft(node);
    } else if (heightDif > 1 && strcmp(term, node->left->term) > 0) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    } else if (heightDif < -1 && strcmp(term, node->right->term) < 0) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }
    return node;
}
// Test
// int main () {
//     printf("Testing Begins\n");
//     TreeNode *tree = initTreeNode("10\0", "doc1");
//     tree = addTerm(tree, "20\0", "doc2");
//     tree = addTerm(tree, "30\0", "doc1");
//     tree = addTerm(tree, "40\0", "doc2");
//     tree = addTerm(tree, "50\0", "doc1");
//     printInOrder(tree);
//     printf("\n");
//     tree = addTerm(tree, "25\0", "doc1");
//     // addTerm(tree, "14\0", "doc1");
//     // addTerm(tree, "11\0", "doc2");
//     // addTerm(tree, "10\0", "doc2");
//     // addTerm(tree, "20\0", "doc2");
//     printInOrder(tree);
//     printf("\n");
//     freeTree(tree);
//     return 0;
// }
