#ifndef PRIM_H
#define PRIM_H

#include "graph.h"

typedef struct {
    int src[MAX_VERTICES];
    int dest[MAX_VERTICES];
    double weight[MAX_VERTICES];
    int edgeCount;
    double totalWeight;
} MSTResult;

MSTResult runPrim(Graph *g);
char* serializePrimJSON(MSTResult res, Graph *g);

#endif // PRIM_H
