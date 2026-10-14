#ifndef JOSEPHUS_H
#define JOSEPHUS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int totalCount;
    int stepSize;
    int survivorId;
    int eliminationOrder[100];
    int eliminatedCount;
} JosephusResult;

JosephusResult solveJosephus(int n, int k);
char* serializeJosephusJSON(JosephusResult res);

#endif // JOSEPHUS_H
