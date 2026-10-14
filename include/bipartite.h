#ifndef BIPARTITE_H
#define BIPARTITE_H

#include "graph.h"

typedef struct {
    bool isBipartite;
    int colors[MAX_VERTICES]; // 0 or 1
} BipartiteResult;

BipartiteResult checkBipartite(Graph *g);
char* serializeBipartiteJSON(BipartiteResult res, Graph *g);

#endif // BIPARTITE_H
