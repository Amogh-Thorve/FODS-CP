#ifndef DFS_H
#define DFS_H

#include "graph.h"

typedef struct {
    int visitedOrder[MAX_VERTICES];
    int count;
    char stackStates[MAX_VERTICES][128];
} DFSResult;

DFSResult runDFS(Graph *g, int startId);
DFSResult runDFSStack(Graph *g, int startId);
char* serializeDFSJSON(Graph *g, DFSResult res);

#endif // DFS_H
