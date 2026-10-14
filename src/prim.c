#include "prim.h"

MSTResult runPrim(Graph *g) {
    MSTResult res;
    res.edgeCount = 0;
    res.totalWeight = 0;
    if (!g || g->numVertices == 0) return res;

    int parent[MAX_VERTICES];
    double key[MAX_VERTICES];
    bool inMST[MAX_VERTICES] = {false};

    for (int i = 0; i < g->numVertices; i++) {
        key[i] = INF;
        parent[i] = -1;
    }

    key[0] = 0;

    for (int count = 0; count < g->numVertices - 1; count++) {
        double minKey = INF;
        int u = -1;
        for (int v = 0; v < g->numVertices; v++) {
            if (!inMST[v] && key[v] < minKey) {
                minKey = key[v];
                u = v;
            }
        }
        if (u == -1) break;
        inMST[u] = true;

        EdgeNode *curr = g->adjList[u];
        while (curr) {
            int v = curr->dest;
            if (!inMST[v] && curr->weight < key[v]) {
                parent[v] = u;
                key[v] = curr->weight;
            }
            curr = curr->next;
        }
    }

    for (int i = 1; i < g->numVertices; i++) {
        if (parent[i] != -1) {
            res.src[res.edgeCount] = g->vertices[parent[i]].id;
            res.dest[res.edgeCount] = g->vertices[i].id;
            res.weight[res.edgeCount] = key[i];
            res.totalWeight += key[i];
            res.edgeCount++;
        }
    }

    return res;
}

char* serializePrimJSON(MSTResult res, Graph *g) {
    static char jsonBuf[4096];
    jsonBuf[0] = '\0';
    snprintf(jsonBuf, sizeof(jsonBuf),
             "{\"totalWeight\":%.2f,\"edgeCount\":%d,\"edges\":[",
             res.totalWeight, res.edgeCount);

    for (int i = 0; i < res.edgeCount; i++) {
        char eBuf[256];
        snprintf(eBuf, sizeof(eBuf),
                 "{\"src\":%d,\"dest\":%d,\"weight\":%.2f}",
                 res.src[i], res.dest[i], res.weight[i]);
        strcat(jsonBuf, eBuf);
        if (i < res.edgeCount - 1) strcat(jsonBuf, ",");
    }
    strcat(jsonBuf, "]}");
    return jsonBuf;
}
