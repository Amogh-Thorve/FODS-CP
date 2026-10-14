#ifndef CONNECTED_GRAPH_H
#define CONNECTED_GRAPH_H

#include "graph.h"

typedef struct {
    int componentId[MAX_VERTICES];
    int numComponents;
} ComponentResult;

ComponentResult findConnectedComponents(Graph *g);
char* serializeComponentResultJSON(ComponentResult res, Graph *g);

#endif // CONNECTED_GRAPH_H
