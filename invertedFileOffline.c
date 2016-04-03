/*
Filename: invertedFileOffline.c
Author: Benjamin Baird
Date Created: April 2, 2016
Last Updated: April 2, 2016
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
    char *docno;
    int start;
}DocNode;

/***
    Initialize a DocNode with docno and start
    @return pointer to a DocNode
***/
DocNode *initDocNode (char *docno, int start) {
    DocNode *node = malloc(sizeof(DocNode));
    node->docno = malloc(sizeof(char)*((int)strlen(docno)+1));
    node->docno = strcpy(node->docno, docno);
    node->start = start;
    return node;
}

/***
    Frees up a DocNode and it's contents
***/
void freeDocNode ( DocNode *node ) {
    free(node->docno);
    free(node);
}


/***
    Generates the dictionary file
    - dictionary.txt: contains all terms in the binary search tree
                  along with the number of documents in which they occur.
                  Sorted alphabetically.
        <total number of terms>
        <term1> <document-frequency1>
        <term2> <document-frequency2>
***/


/****
    Processes the files to create dictionary, postings, and docids files
    @call fp : pointer to file that is to be read
    @return 1 : successful read
****/
int processDocs(TreeNode **termTree, DocNode *docs[100000]){
    int metaTags = 0;

    char *docId = malloc(sizeof(char)*200);
/*    char filename[28] = "DataFiles/a4documents.txt_\0\0";*/
    char filename[28] = "DataFiles/test\0\0";
    int docCount = 0;
    // Loop through the 5 a4documents.txt_#
    for (int i = 1; i < 3;i++) { //Change 3 to 6 when testing real data
/*        filename[26] = i + 48;*/
        filename[14] = i + 48;
        FILE *fp = fopen(filename, "r");
        if (fp == NULL) {
            free(docId);
            return 1;
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
/*            printf("%d|%s|%s\n", metaTags, buffer, docId);*/
            if (strncmp(buffer, "$", 1) == 0) {
                    if (strcmp(buffer, "$DOC") == 0) 
                        metaTags = 1;
                    else 
                        // $TITLE or $BODY
                        metaTags = 2;
                    
            } else if (metaTags == 1) {
                    // Load docid
                    docId = strcpy(docId, buffer);
                    printf("File: %d | B: %s\n", i, buffer);
                    freeDocNode(docs[docCount]); // Free the temp node
                    docs[docCount] = initDocNode(buffer, lineNum);
                    docCount++;

            } else {
                // Update the tree
                if ((*termTree) != NULL) {
                    if (addTerm ((*termTree), buffer, docId) == 0) {
                        // Error adding term
                        free(buffer);
                        free(docId);
                        fclose(fp);
                        return 1;
                    }
                } else {
                    (*termTree) = initTreeNode(buffer, docId);
                }
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
    }
    

    free(docId);
    return 0;
}

int main (int argc, char *argv[]){
    char *buffer = malloc(sizeof(char)*200);
    TreeNode *termTree = NULL;
    DocNode (**docs) = malloc(sizeof(DocNode)*100000);
    for (int i = 0; i < 100000; i++) {
        docs[i] = initDocNode("replace", -1);
    }
    // Command loop
    while (1) {
        printf("What would you like to do?\n \
                1 - Process datafiles\n \
                2 - Print Current Tree Alphabetically\n \
                3 - Print Current Array\n \
                q - Quit\n");
        scanf("%s", buffer);
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
                    printf("DocNo: %s | LineNumber: %d\n", docs[i]->docno, docs[i]->start);
            }
            continue;
        } else {
            if (processDocs(&termTree, docs) == 1) {
                printf("Error processing files.\n");
                return 1;
            }
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
