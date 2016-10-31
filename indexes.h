/***
    indexes.c header file. Complementary library for invertedFileOnline.c
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

typedef struct DictIndex {
    char *term;
    long df;
    long postIndex;
}DictIndex;

typedef struct PostIndex {
    long docno;
    long tf;
}PostIndex;

typedef struct {
    char *docid;
    long line;
}DocIndex;

/***
    Initialize a DictIndex with term and df
    @return : pointer to DictIndex created
***/
DictIndex initDictIndex(char *term, long df, long pIndex);

/***
    Initialize a PostIndex with docno and tf
    @return : pointer to PostIndex created
***/
PostIndex initPostIndex(long docno, long tf);

/***
    Initialize a DocIndex with docid and line
    @return : pointer to DocIndex created
***/
DocIndex initDocIndex(char *docid, long line);

/***
    Print the dictionary array
***/
void printDictArray (DictIndex dIndex[], long dictSize);

/***
    Free dictionary array's terms
***/
void freeDictArray ( DictIndex dIndex[], long dictSize );

/***
    Print the posting array
***/
void printPostArray (PostIndex pIndex[], long pSize);
/***
    Print the doc array
***/
void printDocArray (DocIndex dIndex[], long docSize);

/***
    Free doc array's terms
***/
void freeDocArray ( DocIndex dIndex[], long docSize );
