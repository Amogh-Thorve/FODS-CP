#include "dfs.h"

static void dfsRecursiveHelper(Graph *g, int u, bool visited[], DFSResult *res) {
    visited[u] = true;
    res->visitedOrder[res->count] = g->vertices[u].id;
    strcpy(res->stackStates[res->count], "(Call Stack)");
    res->count++;

    EdgeNode *curr = g->adjList[u];
    while (curr) {
        int v = curr->dest;
        if (!visited[v]) {
            dfsRecursiveHelper(g, v, visited, res);
        }
        curr = curr->next;
    }
}

DFSResult runDFS(Graph *g, int startId) {
    DFSResult res;
    res.count = 0;
    if (!g) return res;

    int startIdx = findVertexIndex(g, startId);
    if (startIdx == -1) startIdx = 0;

    bool visited[MAX_VERTICES] = {false};
    dfsRecursiveHelper(g, startIdx, visited, &res);
    return res;
}

DFSResult runDFSStack(Graph *g, int startId) {
    DFSResult res;
    res.count = 0;
    if (!g) return res;

    int startIdx = findVertexIndex(g, startId);
    if (startIdx == -1) startIdx = 0;

    bool visited[MAX_VERTICES] = {false};
    int stack[MAX_VERTICES];
    int top = -1;

    stack[++top] = startIdx;

    while (top >= 0) {
        int u = stack[top--];
        if (!visited[u]) {
            visited[u] = true;
            res.visitedOrder[res.count] = g->vertices[u].id;

            char sState[128] = "[";
            for (int i = 0; i <= top; i++) {
                char idStr[16];
                snprintf(idStr, sizeof(idStr), "%d", g->vertices[stack[i]].id);
                strcat(sState, idStr);
                if (i < top) strcat(sState, ", ");
            }
            strcat(sState, "]");
            strncpy(res.stackStates[res.count], sState, sizeof(res.stackStates[res.count]) - 1);
            res.count++;

            EdgeNode *curr = g->adjList[u];
            while (curr) {
                int v = curr->dest;
                if (!visited[v]) {
                    stack[++top] = v;
                }
                curr = curr->next;
            }
        }
    }
    return res;
}

char* serializeDFSJSON(Graph *g, DFSResult res) {
    static char jsonBuf[4096];
    jsonBuf[0] = '\0';
    strcat(jsonBuf, "{\"count\":");
    char cStr[16];
    snprintf(cStr, sizeof(cStr), "%d,", res.count);
    strcat(jsonBuf, cStr);
    strcat(jsonBuf, "\"steps\":[");

    for (int i = 0; i < res.count; i++) {
        char sBuf[256];
        snprintf(sBuf, sizeof(sBuf),
                 "{\"visitedId\":%d,\"stackState\":\"%s\"}",
                 res.visitedOrder[i], res.stackStates[i]);
        strcat(jsonBuf, sBuf);
        if (i < res.count - 1) strcat(jsonBuf, ",");
    }
    strcat(jsonBuf, "]}");
    return jsonBuf;
}
