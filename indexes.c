/***
    Filename: indexes.c
    Author: Benjamin Baird
    Date Created: April 3, 2016
    Date Updated: April 3, 2016
    Description: Contains three index types of indexes, dictionary, postings, and docs.
                 To be used with invertedFileOnline.c

    Tested: No memory leaks or errors
***/

#ifndef INDEXES_H_INCLUDED
#define INDEXES_H_INCLUDED
#include "indexes.h"
#endif

DictIndex initDictIndex(char *term, long df, long postIndex){
    DictIndex index;
    index.term = malloc(sizeof(char)*(long)strlen(term)+1);
    index.term = strcpy(index.term, term);
    index.df = df;
    index.postIndex = postIndex;
    return index;
}

PostIndex initPostIndex(long docno, long tf) {
    PostIndex index;
    index.docno = docno;
    index.tf = tf;
    return index;
}

DocIndex initDocIndex(char *docid, long line) {
    DocIndex index;
    index.docid = malloc(sizeof(char)*(long)strlen(docid)+1);
    index.docid = strcpy(index.docid, docid);
    index.line = line;
    return index;
}

void printDictArray (DictIndex dIndex[], long dictSize) {
    for (long i = 0; i < dictSize; i++) {
        printf("Dictionary: %ld: %s %ld %ld\n", i , dIndex[i].term, dIndex[i].df, dIndex[i].postIndex);
    }
}

void freeDictArray ( DictIndex dIndex[], long dictSize ) {
    for (long i = 0; i < dictSize; i++) {
        free(dIndex[i].term);
    }
}

void printPostArray (PostIndex pIndex[], long pSize) {
    for (long i = 0; i < pSize; i++) {
        printf("Post: %ld: %ld %ld\n", i , pIndex[i].docno, pIndex[i].tf);
    }
}

void printDocArray (DocIndex dIndex[], long docSize) {
    for (long i = 0; i < docSize; i++) {
        printf("DocNum: %ld: %s %ld\n", i , dIndex[i].docid, dIndex[i].line);
    }
}

void freeDocArray ( DocIndex dIndex[], long docSize ) {
    for (long i = 0; i < docSize; i++) {
        free(dIndex[i].docid);
    }
}
