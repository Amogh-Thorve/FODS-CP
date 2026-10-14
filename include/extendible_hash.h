#ifndef EXTENDIBLE_HASH_H
#define EXTENDIBLE_HASH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUCKET_CAPACITY 3

typedef struct ExtBucket {
    int localDepth;
    int keys[BUCKET_CAPACITY];
    int count;
} ExtBucket;

typedef struct {
    int globalDepth;
    ExtBucket **dir;
    int dirSize;
} ExtendibleHashTable;

ExtendibleHashTable* createExtendibleHash(int initialGlobalDepth);
void insertExtendibleHash(ExtendibleHashTable *eh, int key);
void freeExtendibleHash(ExtendibleHashTable *eh);
char* serializeExtendibleHashJSON(ExtendibleHashTable *eh);

#endif // EXTENDIBLE_HASH_H
