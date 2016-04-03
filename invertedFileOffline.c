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
int processDoc(FILE *fp, TreeNode **termTree){
    int metaTags = 0;
    char *buffer = malloc(sizeof(char)*200);
    char *docId = malloc(sizeof(char)*200);

    // Grab the first word in the file
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
        
/*        printf("%d|%s|%s\n", metaTags,buffer, docId);*/
        if (metaTags == 0) {
            if (strcmp(buffer, "$DOC") == 0)
                // $DOC
                metaTags = 1;
        } else if (metaTags == 1) {
            if (strcmp(buffer, "$TITLE") == 0) {
                // $TITLE
                metaTags = 2;
            } else {
                // Load docid
                docId = strcpy(docId, buffer);
            }
        } else {
            if (strncmp(buffer, "$", 1) == 0) {
                // $BODY
                metaTags++;             
                if (strcmp(buffer, "$DOC") == 0)
                    // Next $DOC
                    metaTags = 1;
                
            } else {
                // Update the tree
/*                printf("Added\n");*/
                if ((*termTree) != NULL) {
                    if (addTerm ((*termTree), buffer, docId) == 0) {
                        free(buffer);
                        free(docId);
                        return 1;
                    }
                } else {
                    (*termTree) = initTreeNode(buffer, docId);
                    printTree((*termTree));
                }

            }
        }
        
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
    if (buffer != NULL)
        free(buffer);
    free(docId);
    return 0;
}


int main (int argc, char * argv[]){
    char *buffer = malloc(sizeof(char)*200);
    TreeNode *termTree = NULL;
    // Command loop
    while (1) {
        printf("What would you like to do?\n \
                1 - Process a file\n \
                2 - Print Current Tree (Unsorted)\n \
                q - Quit\n");
        scanf("%s", buffer);
        if (strcmp(buffer, "q") == 0){
            break;
        }
        
        if (strcmp(buffer, "2") == 0) {
            printTree(termTree);
            continue;
        }
        
        printf("Which file would you like to process?\n");
        scanf("%s", buffer);
        FILE *fp = fopen(buffer, "r");
        
        if (fp == NULL) {
            printf("Error opening file. File may not exist.\n");    
        } else {
            if (processDoc(fp, &termTree) == 1) {
                printf("Error processing file.\n");
                fclose(fp);
                return 1;
            }
            fclose(fp);
        }
    }
    free(buffer);
    if (termTree != NULL)
        freeTree(termTree);
    return 0;
}
