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

/*
    Processes the files to create dictionary, postings, and docids files
    @call fp : pointer to file that is to be read
    @return 1 : successful read
*/
int processDoc(FILE *fp){
    char *buffer = malloc(sizeof(char)*200);
    int metaTags = 0;
    size_t maxWordLen = 200;
/*    Tree *termTree = malloc(sizeof(TreeNode));*/

    // Read words from file based on the space deliminator
    while (getdelim(&buffer, &maxWordLen, ' ', fp) != -1) {
        printf("%s ", buffer);
        if (metaTags == 0) {
            if (strcmp(buffer, "$DOC") == 0)
                // $DOC
                metaTags = 1;
        } else if (metaTags == 1) {
            if (strncmp(buffer, "$", 1) == 0) {
                // $TITLE
                metaTags = 2;
            } else {
                // Load docid
/*                node->docId = strcpy(node->docId, buffer);*/
            }
        } else {
            if (strncmp(buffer, "$", 1) == 0) {
                // $BODY
                metaTags++;
                if (metaTags == 4) {
                    metaTags = 1;
                }
            } else {
                // Update the tree
/*                if (insertTerm(termTree, buffer) == 1) {*/
/*                    return 1;*/
/*                }*/
            }
        }
    }
    

    free(buffer);
    return 0;
}


int main (int argc, char * argv[]){
    char *buffer = malloc(sizeof(char)*200);

    // Command loop
    while (1) {
        printf("What would you like to do?\n \
                1 - Process a file\n \
                q - Quit\n");
        scanf("%s", buffer);
        if (strcmp(buffer, "q") == 0){
            break;
        }

        printf("Which file would you like to process?\n");
        scanf("%s", buffer);
        FILE *fp = fopen(buffer, "r");
        
        if (fp == NULL) {
            printf("Error opening file. File may not exist.\n");    
        } else {
            if (processDoc(fp) == 1) {
                printf("Error processing file.\n");
                fclose(fp);
                return 1;
            }
            fclose(fp);
        }
    }
    free(buffer);
    return 0;
}
