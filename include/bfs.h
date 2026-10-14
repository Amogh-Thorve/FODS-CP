#ifndef BFS_H
#define BFS_H

#include "graph.h"

typedef struct {
    int visitedOrder[MAX_VERTICES];
    int count;
    char queueStates[MAX_VERTICES][128];
} BFSResult;

BFSResult runBFS(Graph *g, int startId);
char* serializeBFSJSON(Graph *g, BFSResult res);

#endif // BFS_H
