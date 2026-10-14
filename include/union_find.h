#ifndef UNION_FIND_H
#define UNION_FIND_H

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int parent[100];
    int rank[100];
    int size;
} DisjointSet;

void makeSet(DisjointSet *ds, int n);
int findSet(DisjointSet *ds, int i);
void unionSet(DisjointSet *ds, int i, int j);

#endif // UNION_FIND_H
