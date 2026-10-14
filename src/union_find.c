#include "union_find.h"

void makeSet(DisjointSet *ds, int n) {
    if (!ds) return;
    ds->size = n;
    for (int i = 0; i < n; i++) {
        ds->parent[i] = i;
        ds->rank[i] = 0;
    }
}

int findSet(DisjointSet *ds, int i) {
    if (!ds || i < 0 || i >= ds->size) return -1;
    if (ds->parent[i] == i)
        return i;
    // Path compression
    return ds->parent[i] = findSet(ds, ds->parent[i]);
}

void unionSet(DisjointSet *ds, int i, int j) {
    if (!ds) return;
    int rootI = findSet(ds, i);
    int rootJ = findSet(ds, j);
    if (rootI != rootJ) {
        if (ds->rank[rootI] < ds->rank[rootJ]) {
            ds->parent[rootI] = rootJ;
        } else if (ds->rank[rootI] > ds->rank[rootJ]) {
            ds->parent[rootJ] = rootI;
        } else {
            ds->parent[rootJ] = rootI;
            ds->rank[rootI]++;
        }
    }
}
