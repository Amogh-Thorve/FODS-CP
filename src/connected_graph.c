#include "connected_graph.h"

static void compDFS(Graph *g, int u, int compId, ComponentResult *res, bool visited[]) {
    visited[u] = true;
    res->componentId[u] = compId;
    EdgeNode *curr = g->adjList[u];
    while (curr) {
        int v = curr->dest;
        if (!visited[v]) {
            compDFS(g, v, compId, res, visited);
        }
        curr = curr->next;
    }
}

ComponentResult findConnectedComponents(Graph *g) {
    ComponentResult res;
    res.numComponents = 0;
    if (!g) return res;

    bool visited[MAX_VERTICES] = {false};
    for (int i = 0; i < g->numVertices; i++) {
        if (!visited[i]) {
            res.numComponents++;
            compDFS(g, i, res.numComponents, &res, visited);
        }
    }
    return res;
}

char* serializeComponentResultJSON(ComponentResult res, Graph *g) {
    static char jsonBuf[4096];
    jsonBuf[0] = '\0';
    strcat(jsonBuf, "{\"numComponents\":");
    char cStr[16];
    snprintf(cStr, sizeof(cStr), "%d,", res.numComponents);
    strcat(jsonBuf, cStr);
    strcat(jsonBuf, "\"clusters\":[");

    for (int c = 1; c <= res.numComponents; c++) {
        char clusBuf[512];
        snprintf(clusBuf, sizeof(clusBuf), "{\"clusterId\":%d,\"nodes\":[", c);
        strcat(jsonBuf, clusBuf);
        int firstNode = 1;
        for (int i = 0; i < g->numVertices; i++) {
            if (res.componentId[i] == c) {
                if (!firstNode) strcat(jsonBuf, ",");
                firstNode = 0;
                char nBuf[128];
                snprintf(nBuf, sizeof(nBuf), "{\"id\":%d,\"name\":\"%s\"}", g->vertices[i].id, g->vertices[i].name);
                strcat(jsonBuf, nBuf);
            }
        }
        strcat(jsonBuf, "]}");
        if (c < res.numComponents) strcat(jsonBuf, ",");
    }
    strcat(jsonBuf, "]}");
    return jsonBuf;
}
