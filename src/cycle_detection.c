#include "cycle_detection.h"

static bool cycleDFS(Graph *g, int u, int visited[], int recStack[], CycleResult *res) {
    visited[u] = 1;
    recStack[u] = 1;
    res->cyclePath[res->pathLength++] = g->vertices[u].id;

    EdgeNode *curr = g->adjList[u];
    while (curr) {
        int v = curr->dest;
        if (!visited[v]) {
            if (cycleDFS(g, v, visited, recStack, res)) return true;
        } else if (recStack[v]) {
            res->cyclePath[res->pathLength++] = g->vertices[v].id;
            res->hasCycle = true;
            return true;
        }
        curr = curr->next;
    }

    recStack[u] = 0;
    res->pathLength--;
    return false;
}

CycleResult detectCycle(Graph *g) {
    CycleResult res;
    res.hasCycle = false;
    res.pathLength = 0;
    if (!g) return res;

    int visited[MAX_VERTICES] = {0};
    int recStack[MAX_VERTICES] = {0};

    for (int i = 0; i < g->numVertices; i++) {
        if (!visited[i]) {
            if (cycleDFS(g, i, visited, recStack, &res)) {
                break;
            }
        }
    }
    return res;
}

char* serializeCycleResultJSON(CycleResult res, Graph *g) {
    static char jsonBuf[2048];
    jsonBuf[0] = '\0';
    strcat(jsonBuf, "{\"hasCycle\":");
    strcat(jsonBuf, res.hasCycle ? "true," : "false,");
    strcat(jsonBuf, "\"cyclePath\":[");
    for (int i = 0; i < res.pathLength; i++) {
        char val[16];
        snprintf(val, sizeof(val), "%d", res.cyclePath[i]);
        strcat(jsonBuf, val);
        if (i < res.pathLength - 1) strcat(jsonBuf, ",");
    }
    strcat(jsonBuf, "]}");
    return jsonBuf;
}
