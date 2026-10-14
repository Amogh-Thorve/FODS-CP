#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "graph.h"

typedef struct {
    int srcId;
    int destId;
    double dist[MAX_VERTICES];
    int parent[MAX_VERTICES];
    int path[MAX_VERTICES];
    int pathLength;
    double totalCost;
} DijkstraResult;

DijkstraResult runDijkstra(Graph *g, int srcId, int destId);
char* serializeDijkstraJSON(DijkstraResult res, Graph *g);

#endif // DIJKSTRA_H
