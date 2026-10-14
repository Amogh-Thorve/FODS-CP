#include "bipartite.h"

BipartiteResult checkBipartite(Graph *g) {
    BipartiteResult res;
    res.isBipartite = true;
    for (int i = 0; i < MAX_VERTICES; i++) res.colors[i] = -1;
    if (!g || g->numVertices == 0) return res;

    int queue[MAX_VERTICES];
    for (int i = 0; i < g->numVertices; i++) {
        if (res.colors[i] == -1) {
            res.colors[i] = 1;
            int front = 0, rear = 0;
            queue[rear++] = i;

            while (front < rear) {
                int u = queue[front++];
                EdgeNode *curr = g->adjList[u];
                while (curr) {
                    int v = curr->dest;
                    if (res.colors[v] == -1) {
                        res.colors[v] = 1 - res.colors[u];
                        queue[rear++] = v;
                    } else if (res.colors[v] == res.colors[u]) {
                        res.isBipartite = false;
                    }
                    curr = curr->next;
                }
            }
        }
    }
    return res;
}

char* serializeBipartiteJSON(BipartiteResult res, Graph *g) {
    static char jsonBuf[4096];
    jsonBuf[0] = '\0';
    strcat(jsonBuf, "{\"isBipartite\":");
    strcat(jsonBuf, res.isBipartite ? "true," : "false,");
    strcat(jsonBuf, "\"setA\":[");

    int firstA = 1, firstB = 1;
    for (int i = 0; i < g->numVertices; i++) {
        if (res.colors[i] == 1) {
            if (!firstA) strcat(jsonBuf, ",");
            firstA = 0;
            char nBuf[128];
            snprintf(nBuf, sizeof(nBuf), "{\"id\":%d,\"name\":\"%s\"}", g->vertices[i].id, g->vertices[i].name);
            strcat(jsonBuf, nBuf);
        }
    }
    strcat(jsonBuf, "],\"setB\":[");
    for (int i = 0; i < g->numVertices; i++) {
        if (res.colors[i] == 0) {
            if (!firstB) strcat(jsonBuf, ",");
            firstB = 0;
            char nBuf[128];
            snprintf(nBuf, sizeof(nBuf), "{\"id\":%d,\"name\":\"%s\"}", g->vertices[i].id, g->vertices[i].name);
            strcat(jsonBuf, nBuf);
        }
    }
    strcat(jsonBuf, "]}");
    return jsonBuf;
}
