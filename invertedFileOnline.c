/*
Filename: invertedFileOnline.c
Author: Benjamin Baird
Date Created: April 2, 2016
Last Updated: April 7, 2016
Description: Loads the 3 files created during the "online" mode into memory and 
             allows user to search for relevant documents given a query.
                - dictionary.txt
                    <total number of terms>
                    <term1> <document-frequency1>
                    <term2> <document-frequency2>
                      
                - postings.txt
                    <total number of entries>
                    <docno1> <term-frequency1>
                    <docno2> <term-frequency2>
                    
                - docids.txt
                    <total number of documents>
                    <docid1> <start-position1>
                    <docid2> <start-position2>
                    
TODO: Change from test files to real files in getTitle here and offline
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

#ifndef INDEXES_H_INCLUDED
#define INDEXES_H_INCLUDED
#include "indexes.h"
#endif

#ifndef MATH_H_INCLUDED
#define MATH_H_INCLUDED
#include <math.h>
#endif


/***
    Compare function for qsort
***/
int cmp (const void *pa, const void *pb) {
    const double *a = *(double**)pa;
    const double *b = *(double**)pb;
    if (a[1] > b[1])
        return -1;
    if (a[1] < b[1])
        return 1;
    return 0;
}

/***
    Multiply two vectors of the same length
***/
double dotProduct (double vOne[], double vTwo[], int size) {
    double product = 0;

    for (int i = 0; i < size; i++) {
        product += vOne[i] * vTwo[i];  
    }
    
    return product;
}

/***
    Normalize/find magitude of same length vectors
***/
double normalize (double vectorOne[], double vectorTwo[], int size) {
    double result = dotProduct(vectorOne, vectorTwo, size);
    result = sqrt(result);
    return result;
}

/***
    Laplace's law to determine smoothed probability of term in document
***/
/*double laplace (int tf, int totalTerms, int doclen) {*/
/*    return ((tf + 1)/(doclen + totalTerms));*/
/*}*/

double tfidf (double tf, int totalDocs, int df) {
    if (df == 0)
        return 100;
    return ((double)tf * log2((double)totalDocs/(double)df));
}

/***
    Binary search to locate a string in a dictionary index
***/
int searchIndex ( DictIndex index[], int length, char *term) {
    int middle = (int)(length/2);
    int max = length;    
    int min = 0;

    while (1) {
        int cmp = strcmp(term, index[middle].term);
        if (cmp == 0) {
            return middle;
        } else if ( min == max || (min == middle && max == middle)) {
            // No term found
            break;
        } else if ( cmp < 0 ) {
            // BinarySearch moving down
            max = middle;
            middle = (min+max)/2;

        } else if (cmp > 0 ) {
            // BinarySearch moving up
            min = middle + 1;
            middle = (max +middle)/2 + 1;
        }
    }
    
    return -1;
}


/***
    Perform a weighted retrieval of relevant documents
    @return : array of relevant documents
***/
double **retrieveResults (char *query, double docTermVector[], DictIndex dictIndex[], PostIndex postIndex[], 
                    DocIndex docIndex[], int dictSize, int postSize, int numDocs) {

    // Calculate weighted vector of query
    double *queryVector = malloc(sizeof(double)*(int)strlen(query));
    char *buffer;
    char *delims = " \n";
    char *token = malloc(sizeof(char)*(int)strlen(query)+1);
    int queryCounter = 0;
    char *uniqueTokens = malloc(sizeof(char)*(int)strlen(query)+1);
    uniqueTokens = strcpy(uniqueTokens, "\0");
    double maxTf = 0;
    
    // Count the most frequent word and remove duplicates
    token = strcpy(token, query);
    buffer = strtok(token, delims);
    while (buffer != NULL) {
        double tf = 0;

        // Count the tf in the query
        char *temp = query;
        temp = strstr(temp, buffer);
        while (temp != NULL) {
            tf++;
            temp++;
            temp = strstr(temp, buffer);
        }
        
        char *tmp2 = strstr(uniqueTokens, buffer);
        if ( tmp2 == NULL) {
            uniqueTokens = strcat(uniqueTokens, buffer);
            uniqueTokens = strcat(uniqueTokens, " ");
            queryVector[queryCounter] = tf;
            queryCounter++;
        }
        
        if (tf > maxTf)
            maxTf = tf;
        buffer = strtok(NULL, delims);
    }
    uniqueTokens = strcat(uniqueTokens, "\0");
    token = strcpy(token, uniqueTokens);
    buffer = strtok(token, delims);

    // Go through all the words for the query
    for (int i = 0; i < queryCounter; i++) {
        int result = searchIndex( dictIndex, dictSize - 1, buffer); 
        
        // Assign vector weights
        if (result >= 0) {
            queryVector[i] =  tfidf (queryVector[i]/maxTf, numDocs, dictIndex[result].df);
        } else {
            queryVector[i] = 0;
        }
        buffer = strtok(NULL, delims);
    }
        
    queryVector = realloc(queryVector, sizeof(double)*queryCounter);
    token = strcpy(token, uniqueTokens);
    
    // Calculate the weighted vectors of each document
    double docMatrix[numDocs];
    
    buffer = strtok(token, delims);
    
    // Initialize vectors with 0 (Better way?)
    for (int i = 0; i < numDocs; i++) 
        docMatrix[i] = 0;

    // Calculate document vectors
    for (int i = 0; i < queryCounter; i++) {
        int result = searchIndex( dictIndex, dictSize - 1, buffer); 
        // Check if the query word exists in a document
        if (result >= 0) {
            int df = dictIndex[result].df;
            int pindice = dictIndex[result].postIndex;
            
            // Loop through all docnos word appears in
            for (int k = pindice; (k < pindice + df) ; k++) {
                    //Locate word in dictionary and calculate it's cumulative dot product
                    double tfidfResult = tfidf((double)postIndex[k].tf, numDocs, df);
/*                    printf("%lf\n", tfidfResult);*/
                    docMatrix[postIndex[k].docno] +=  tfidfResult * queryVector[i];
            }
        }
        buffer = strtok(NULL, delims);
    }
    
    // Cosine the vectors to get weighted results
    double queryMagn = normalize(queryVector, queryVector, queryCounter);

/*    printf("|Q| = %lf\n", queryMagn);*/
    for (int i = 0; i < numDocs; i++) {
/*        printf("Before:Vector[%d]: %lf\n", i, docMatrix[i]);*/
        double denominator = docTermVector[i] * queryMagn;
        if (denominator == 0)
            docMatrix[i] = 0;   // No term(s)
        else
            docMatrix[i] /= denominator;
/*        printf("After: Vector[%d]: %lf\n", i, docMatrix[i]);*/
    }
    
    // Sort the results and flag the first one without any match
    double **sorted = malloc(sizeof(double**)*numDocs);
    for (int i = 0; i < numDocs; i++) {
        sorted[i] = malloc(sizeof(double*)*2);
        sorted[i][0] = i;
        sorted[i][1] = docMatrix[i];
    }
    
    qsort( sorted, numDocs, sizeof(double*), cmp );
    
    for (int i = 0; i < numDocs; i++) {
        printf("%lf %lf\n",sorted[i][0], sorted[i][1]);
        if (sorted[i][1] == 0.0) {
            sorted[i][1] = -1.0;
        }
    }
    
    free(uniqueTokens);
    free(queryVector);
/*    free(buffer);*/
    free(token);
    return sorted;
}



/***
    Grabs the title from the datafile
***/
char *getTitle(int docno, DocIndex docIndex[], int numDocs) {
    char *title = malloc(sizeof(char)*2000);
    title = strcpy (title, "\0");
/*    char filename[28] = "DataFiles/a4documents.txt_\0\0";*/
    char filename[28] = "DataFiles/test\0\0";
    char *docId = malloc(sizeof(char)*200);
    char letter [2] = "\0\0";
        
    // Loop through the 5 a4documents.txt_#
    for (int i = 1; i < 2;i++) { 
/*        filename[26] = i + 48;*/
        filename[14] = i + 48;
        FILE *fp = fopen(filename, "r");
        if (fp == NULL) {
            free(docId);
            free(title);
            return NULL;
        }
        
        // Navigate to document's starting line        
        letter[0] = fgetc(fp);
        int counter = 0;
        while (letter[0] != EOF && counter < docIndex[docno].line) {
            if (letter[0] == '\n')
                counter++;
            letter[0] = fgetc(fp);
        }
        
        // Grab the first word in the file
        char *buffer = malloc(sizeof(char)*200);
        buffer = strcpy(buffer,letter);
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

        int docFound = 0;
        // Read words from file based on the space deliminator
        while (buffer != NULL) {
            if (strncmp(buffer, "$", 1) == 0) {
            
                if (strcmp(buffer, "$DOC") == 0) {
                    // Check if the docid is correct
                    buffer = strcpy(buffer, "\0");
                    letter[0] = fgetc(fp);
                    while (letter[0] != EOF && letter[0] !='\n') {
                        buffer = strcat(buffer, letter);
                        letter[0] = fgetc(fp);
                    }
                    if (strcmp(docIndex[docno].docid, buffer) != 0) {
                        break;
                    }
                    docFound = 1;
                    
                } else if (strcmp(buffer, "$TITLE") == 0) {
                    // Grab the title
                    letter[0] = fgetc(fp);
                    buffer = strcpy(buffer, "\0");
                    while (letter[0] != '$' && letter[0] != EOF) {
                        title = strcat(title, letter);
                        letter[0] = fgetc(fp);
                    }
                    free(buffer);
                    free(docId);
                    fclose(fp);
                    return title;
                } else {
                    if (docFound) {
                        title = strcpy(title, "<No Title>\0");
                        free(buffer);
                        free(docId);
                        fclose(fp);
                        return title;
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
        fclose(fp);
    }
    
    free(docId);
    return title;
}


int main (int argc, char * argv[]){
    // Load the dictionary.txt into memory
    FILE *dictionary = fopen("dictionary.txt", "r");
    if (dictionary == NULL) {
        printf("Error loading dictionary.txt");
        return 1;
    }    
    int dictSize = 0;
    int ret = fscanf(dictionary, "%d", &dictSize);
    if ( ret == 0 ){
        fclose(dictionary);
        return 1;
    }
    int cur = 0;
    DictIndex dictIndex[dictSize];
    char *term = malloc(sizeof(char)*200);
    for (int i = 0; i < dictSize; i++) {
        int df = 0;
        ret = fscanf( dictionary, "%s %d\n", term, &df); 
        if (ret == 0) {
            fclose(dictionary);
            free(term);
            return 1;
        }
        dictIndex[i] = initDictIndex(term, df, cur);
        cur += df;
    }
    free(term);
    fclose(dictionary);

    // Load the docid
    FILE *docfp = fopen("docids.txt", "r");
    if (docfp == NULL) {
        printf("Error loading docids.txt");
        return 1;
    }
    int numDocs = 0;
    ret = fscanf(docfp, "%d", &numDocs);
    if (ret == 0) {
        fclose(docfp);
        return 1;
    }
    DocIndex docIndex[numDocs];    
    for (int i = 0; i < numDocs; i++) {
        int line = 0;
        char *docid = malloc(sizeof(char)*200);
        ret = fscanf( docfp, "%s %d\n", docid, &line); 
        if (ret == 0) {
            free(docid);
            fclose(docfp);
            return 1;
        }
        docIndex[i] = initDocIndex(docid, line);
        free(docid);
    }  
    fclose(docfp);
    
    // Store the number of words in each document
    double *docTermVector= malloc(sizeof(double) * numDocs);
    for (int i = 0; i < numDocs; i++) {
        docTermVector[i] = 0;
    }
    
    // Load the posting file 
    FILE *posting = fopen("postings.txt", "r");  
    if (posting == NULL) {
        printf("Error loading postings.txt");
        return 1;
    }    
    int postSize = 0;
    ret = fscanf(posting, "%d", &postSize);
    if (ret == 0) {
        fclose(posting);
        return 1;
    }
    PostIndex postIndex[postSize];    
    int index = 0;
    int pIndex = dictIndex[0].postIndex + dictIndex[0].df - 1;
    for (int i = 0; i < postSize; i++) {
        if (i > pIndex) {
            index++;
            pIndex = dictIndex[index].postIndex + dictIndex[index].df - 1;
        }
        int tf = 0;
        int docno = 0;
        ret = fscanf( posting, "%d %d\n", &docno, &tf); 
        if (ret == 0) {
            fclose(posting);
            return 1;
        }
        postIndex[i] = initPostIndex(docno, tf);
        docTermVector[docno] += pow(tfidf((double)tf, (double)numDocs, (double)dictIndex[index].df), 2);
    }   
    fclose(posting);
    
    for (int i = 0; i < numDocs; i++) { 
        docTermVector[i] = sqrt(docTermVector[i]);
    }
    
    // Command Loop
    while (1) {
        char *input = malloc(sizeof(char)*500);
        printf("Please enter a query. Seperate each keyword by a space. q to quit:\n");    
        input = fgets(input, 500, stdin);
        if (strcmp(input, "q\n") == 0) {
            free(input);
            break;
        } else {
            double **results = retrieveResults(input, docTermVector, dictIndex, \
                                    postIndex, docIndex, dictSize, postSize, numDocs);
            int index = 0;
            while (strcmp(input, "q\n") != 0) {
                printf("------------------------\n");
                printf("Results for query:\n");
                for (int i = index; i < index + 10 && i < numDocs; i++) {
                    if (results[i][1] != -1.0) {
                        char *title = getTitle(results[i][0], docIndex, numDocs);
                        printf("Result %d: %s\n", i, title);
                        free(title);
                    } else {
                        break;
                    }
                }
                printf("------------------------\n");
                printf("Enter 'q' to enter a new query.\n");
                printf("Enter 'a' to view previous 10 results\n");
                printf("Enter 'd' to view next 10 results\n");
                input = fgets(input, 4, stdin);
                if (strcmp(input, "a\n") == 0)
                    index += 10;
                    if (index > numDocs)
                        index -= 10;
                else if (strcmp(input, "d\n") == 0) {
                    index -= 10;
                    if (index < 0)
                        index = 0;
                }
            }
            
            for (int i = 0; i < numDocs; i++) {
                free(results[i]);
            }
            free(results);
        }
        free(input);
    }
    
    free(docTermVector);
    freeDictArray(dictIndex, dictSize);
    freeDocArray(docIndex, numDocs);
    return 0;
}
