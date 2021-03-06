/*
Filename: invertedFileOffline.c
Author: Benjamin Baird
Date Created: April 2, 2016
Last Updated: April 8, 2016
Description: Implements the inverted file data structure to be used "Offline".
             Creates 3 files based on the given input file.
                - dictionary.txt: contains all terms in the binary search tree
                                  along with the number of documents in which they occur.
                                  Sorted alphabetically.
                    <total number of terms>
                    <term1> <document-frequency1>
                    <term2> <document-frequency2>

                - postings.txt: contains the document's id and it's term frequency
                    <total number of entries>
                    <docno1> <term-frequency1>
                    <docno2> <term-frequency2>

                - docids.txt: docid's with their starting positions from the input file
                    <total number of documents>
                    <docid1> <start-position1>
                    <docid2> <start-position2>
Tested: 0 memory leaks or errors
*/

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

#ifndef TREE_H_INCLUDED
#define TREE_H_INCLUDED
#include "tree.h"
#endif

typedef struct DocNode {
    char *docId;
    int start;
}DocNode;

/***
    Initialize a DocNode with docno and start
    @return pointer to a DocNode
***/
DocNode *initDocNode (char *docId, int start) {
    DocNode *node = malloc(sizeof(DocNode));
    node->docId = malloc(sizeof(char)*((int)strlen(docId)+1));
    node->docId = strcpy(node->docId, docId);
    node->start = start;
    return node;
}

/***
    Frees up a DocNode and it's contents
***/
void freeDocNode ( DocNode *node ) {
    free(node->docId);
    free(node);
}

/***
    Generates the dictionary file. Sorted alphabetically.
        <total number of terms>
        <term1> <document-frequency1>
        <term2> <document-frequency2>
***/
int genDictionary ( FILE *fp, TreeNode *termTree ) {
    int retVal = 0;
    // Left Side
    while (termTree->left != NULL && retVal == 0)
        retVal = genDictionary(fp, termTree->left);

    // Itself
    int df = 0;
    Node *temp = termTree->dictionary;
    while (temp != NULL) {
        df++;
        temp = temp->next;
    }
    fprintf(fp,"%s %d\n", termTree->term, df);
    retVal = 0;

    // Right Side
    while (termTree->right != NULL && retVal == 0)
        retVal = genDictionary(fp, termTree->right);

    return 1;
}

/****
    Processes the files to create dictionary, postings, and docids files
    @call fp : pointer to file that is to be read
    @return >0 : number of terms read
****/
int processDocs(TreeNode **termTree, DocNode **docs, char *filename){
    int metaTags = 0;
    int numTerms = 0;
    char *docId = malloc(sizeof(char)*200);
    int docCount = 0;
    int docLine = 0;

    // Load file to process
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        free(docId);
        return -1;
    }
    int lineNum = 0;

    // Grab the first word in the file
    char *buffer = malloc(sizeof(char)*200);
    char letter [2] = "\0\0";
    buffer = strcpy(buffer,"\0");
    letter[0] = fgetc(fp);
    while ((letter[0] != ' ') && (letter[0] != '\n')){
        if (letter[0] == EOF) {
            free(buffer);
            buffer = NULL;
            break;
        }
        buffer = strcat(buffer, letter);
        letter[0] = fgetc(fp);
    }

    // Read words from file based on the space deliminator
    while (buffer != NULL) {
        if (strncmp(buffer, "$", 1) == 0) {
                if (strcmp(buffer, "$DOC") == 0) {
                    metaTags = 1;
                } else {
                    // $TITLE or $BODY
                    metaTags++;
                }

        } else if (metaTags == 1) {
            // Load docid
            docId = strcpy(docId, buffer);
            docLine = lineNum;

        } else if (metaTags > 1) {
            // Update the tree
            if (strlen(buffer) >= 1 && strncmp(buffer, "0",1) > 0){
                if ((*termTree) != NULL ) {
                    (*termTree) = addTerm ((*termTree), buffer, docId);
                } else {
                    (*termTree) = initTreeNode(buffer, docId);
                }
            }

            // Making sure document is not empty
            if (metaTags == 2) {
                freeDocNode(docs[docCount]); // Free the temp node
                docs[docCount] = initDocNode(docId, docLine);
                docCount++;
                metaTags++;
            }
            numTerms++;
        }

        // Found a newline
        if (letter[0] == '\n')
            lineNum++;

        // Grab next word
        buffer = strcpy(buffer,"\0");
        letter[0] = fgetc(fp);
        while ((letter[0] != ' ') && (letter[0] != '\n')){
            if (letter[0] == EOF) {
                free(buffer);
                buffer = NULL;
                break;
            }
            buffer = strcat(buffer, letter);
            letter[0] = fgetc(fp);
        }

    }

    fclose(fp);
    free(docId);
    return numTerms;
}

/***
    Creates the docids.txt file based off of the docs array.
        <total number of documents>
        <docid1> <start-position1>
        <docid2> <start-position2>
***/

int genDocid(FILE *fp, DocNode **docs) {
    int numDocs = 0;
    fprintf(fp,"      \n");

    for (int i = 0; i < 100000; i++) {
        if (docs[i]->start != -1) {
            fprintf(fp, "%s %d\n", docs[i]->docId, docs[i]->start);
            numDocs++;
        }
    }
    fseek(fp, 0, SEEK_SET);
    fprintf(fp,"%.6d\n", numDocs);
    return 0;
}

/***
    Generates postings.txt. Contains the document's number, based off
        of its index in the docs array, and it's term frequency. Ordered by term.
        Depth-First traversal of termTree
    <docno1> <term-frequency1>
    <docno2> <term-frequency2>
***/
int genPostings(FILE *fp, TreeNode *termTree, DocNode **docs) {
    if (termTree == NULL || docs == NULL)
        return 0;

    int totalEntries = 0;
    int retVal = 0;

    // Left Side Traversal
    while (termTree->left != NULL && retVal == 0) {
        retVal = genPostings(fp, termTree->left, docs);
    }
    totalEntries += retVal;

    // Print the node's posting
    Node *node = termTree->dictionary;
    while (node != NULL) {
        int index = 0;
        for (index = 0; index < 10000000; index++) {
            if (strcmp(docs[index]->docId, node->docId) == 0)
                break;
        }
        fprintf(fp,"%d %d\n", index, node->freq);
        totalEntries++;
        node = node->next;
    }

    retVal = 0;

    // Right Side Traversal
    while (termTree->right != NULL && retVal == 0) {
        retVal = genPostings(fp, termTree->right, docs);
    }
    totalEntries += retVal;

    return totalEntries;
}

int main (int argc, char *argv[]){
    char *buffer = malloc(sizeof(char)*200);
    TreeNode *termTree = NULL;
    DocNode (**docs) = malloc(sizeof(DocNode)*999999);
    int numTerms = 0;

    for (int i = 0; i < 999999; i++) {
        docs[i] = initDocNode("replace", -1);
    }
    // Command loop
    while (1) {
        printf("What would you like to do?\n \
                1 - Process datafiles\n \
                2 - Print Current Tree Alphabetically\n \
                3 - Print Document Index\n \
                q - Quit\n");
        int ret = scanf("%s", buffer);
        if (ret == 0) {
            free(buffer);
            for (int i = 100000-1; i >= 0; i--) {
                freeDocNode(docs[i]);
            }
            free(docs);
            return 1;
        }

        if (strcmp(buffer, "q") == 0){
            break;

        } else if (strcmp(buffer, "2") == 0) {
            if (termTree == NULL) {
                printf("Tree is empty\n");
                continue;
            }
            printTree(termTree);

        } else if (strcmp(buffer, "3") == 0) {
            for (int i = 0; i < 100000; i++) {
                if (docs[i]->start != -1)
                    printf("DocNo: %s | LineNumber: %d\n", docs[i]->docId, docs[i]->start);
            }
            continue;

        } else if (strcmp(buffer, "1") == 0){
            printf("Enter the filename of file to process...\n");
            char *filename = malloc(sizeof(char)*500);
            if (scanf("%s", filename) == 0) {
                free(filename);
                free(buffer);
                for (int i = 100000-1; i >= 0; i--) {
                    freeDocNode(docs[i]);
                }
                free(docs);
            }
            numTerms = processDocs(&termTree, docs, filename);
            free(filename);
            if ( numTerms == -1) {
                printf("Error processing files.\n");
                return 1;
            }

            // Generate dictionary.txt
            FILE *fp = fopen("dictionary.txt", "w+");
            fprintf(fp, "%d\n", countTreeNodes(termTree));
            genDictionary( fp, termTree );
            fclose(fp);

            //Generate Postings.txt
            fp = fopen("postings.txt", "w+");
            fprintf(fp, "               \n");
            int numEntries = genPostings(fp, termTree, docs);
            fseek(fp, 0, SEEK_SET);
            fprintf(fp, "%.15d\n", numEntries);
            fclose(fp);

            //Generate DocIds.txt
            fp= fopen("docids.txt","w+");
            genDocid(fp, docs);
            fclose(fp);

        }
    }

    free(buffer);
    if (termTree != NULL)
        freeTree(termTree);
    for (int i = 100000-1; i >= 0; i--) {
        freeDocNode(docs[i]);
    }
    free(docs);
    return 0;
}
