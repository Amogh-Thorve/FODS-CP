#ifndef PERFECT_HASH_H
#define PERFECT_HASH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char key[16];
    char name[64];
    int hashVal;
} PerfectHashEntry;

typedef struct {
    PerfectHashEntry slots[16];
    int count;
} PerfectHashTable;

PerfectHashTable buildPerfectHashTable(void);
char* serializePerfectHashJSON(PerfectHashTable *ht);

#endif // PERFECT_HASH_H
