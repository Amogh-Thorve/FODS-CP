#ifndef KRUSKAL_H
#define KRUSKAL_H

#include "graph.h"
#include "prim.h" // Reuses MSTResult

typedef struct {
    int src;
    int dest;
    double weight;
    bool selected;
} KruskalEdgeStep;

typedef struct {
    MSTResult mst;
    KruskalEdgeStep edgeSteps[100];
    int totalSteps;
} KruskalResult;

KruskalResult runKruskal(Graph *g);
char* serializeKruskalJSON(KruskalResult res, Graph *g);

#endif // KRUSKAL_H
