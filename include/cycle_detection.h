#ifndef CYCLE_DETECTION_H
#define CYCLE_DETECTION_H

#include "graph.h"

typedef struct {
    bool hasCycle;
    int cyclePath[MAX_VERTICES];
    int pathLength;
} CycleResult;

CycleResult detectCycle(Graph *g);
char* serializeCycleResultJSON(CycleResult res, Graph *g);

#endif // CYCLE_DETECTION_H
