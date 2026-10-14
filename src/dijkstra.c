#include "dijkstra.h"

DijkstraResult runDijkstra(Graph *g, int srcId, int destId) {
    DijkstraResult res;
    memset(&res, 0, sizeof(res));
    res.srcId = srcId;
    res.destId = destId;
    if (!g) return res;

    int srcIdx = findVertexIndex(g, srcId);
    int destIdx = findVertexIndex(g, destId);
    if (srcIdx == -1) srcIdx = 0;
    if (destIdx == -1) destIdx = g->numVertices - 1;

    bool visited[MAX_VERTICES] = {false};
    for (int i = 0; i < g->numVertices; i++) {
        res.dist[i] = INF;
        res.parent[i] = -1;
    }
    res.dist[srcIdx] = 0;

    for (int count = 0; count < g->numVertices - 1; count++) {
        double minD = INF;
        int u = -1;
        for (int v = 0; v < g->numVertices; v++) {
            if (!visited[v] && res.dist[v] <= minD) {
                minD = res.dist[v];
                u = v;
            }
        }
        if (u == -1 || u == destIdx) break;
        visited[u] = true;

        EdgeNode *curr = g->adjList[u];
        while (curr) {
            int v = curr->dest;
            if (!visited[v] && res.dist[u] + curr->weight < res.dist[v]) {
                res.dist[v] = res.dist[u] + curr->weight;
                res.parent[v] = u;
            }
            curr = curr->next;
        }
    }

    // Reconstruction of path
    int curr = destIdx;
    int tempPath[MAX_VERTICES];
    int len = 0;
    while (curr != -1) {
        tempPath[len++] = curr;
        curr = res.parent[curr];
    }
    for (int i = 0; i < len; i++) {
        res.path[i] = g->vertices[tempPath[len - 1 - i]].id;
    }
    res.pathLength = len;
    res.totalCost = (res.dist[destIdx] == INF) ? -1 : res.dist[destIdx];

    return res;
}

char* serializeDijkstraJSON(DijkstraResult res, Graph *g) {
    static char jsonBuf[4096];
    jsonBuf[0] = '\0';
    snprintf(jsonBuf, sizeof(jsonBuf),
             "{\"srcId\":%d,\"destId\":%d,\"totalCost\":%.2f,\"path\":[",
             res.srcId, res.destId, res.totalCost);

    for (int i = 0; i < res.pathLength; i++) {
        char val[16];
        snprintf(val, sizeof(val), "%d", res.path[i]);
        strcat(jsonBuf, val);
        if (i < res.pathLength - 1) strcat(jsonBuf, ",");
    }
    strcat(jsonBuf, "]}");
    return jsonBuf;
}
