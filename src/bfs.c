#include "bfs.h"
#include "queue.h"

BFSResult runBFS(Graph *g, int startId) {
    BFSResult res;
    res.count = 0;
    if (!g) return res;

    int startIdx = findVertexIndex(g, startId);
    if (startIdx == -1) startIdx = 0;

    bool visited[MAX_VERTICES] = {false};
    int queue[MAX_VERTICES];
    int front = 0, rear = 0;

    visited[startIdx] = true;
    queue[rear++] = startIdx;

    while (front < rear) {
        int u = queue[front++];
        res.visitedOrder[res.count] = g->vertices[u].id;

        // Record queue state
        char qState[128] = "[";
        for (int i = front; i < rear; i++) {
            char idStr[16];
            snprintf(idStr, sizeof(idStr), "%d", g->vertices[queue[i]].id);
            strcat(qState, idStr);
            if (i < rear - 1) strcat(qState, ", ");
        }
        strcat(qState, "]");
        strncpy(res.queueStates[res.count], qState, sizeof(res.queueStates[res.count]) - 1);
        res.count++;

        EdgeNode *curr = g->adjList[u];
        while (curr) {
            int v = curr->dest;
            if (!visited[v]) {
                visited[v] = true;
                queue[rear++] = v;
            }
            curr = curr->next;
        }
    }
    return res;
}

char* serializeBFSJSON(Graph *g, BFSResult res) {
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
                 "{\"visitedId\":%d,\"queueState\":\"%s\"}",
                 res.visitedOrder[i], res.queueStates[i]);
        strcat(jsonBuf, sBuf);
        if (i < res.count - 1) strcat(jsonBuf, ",");
    }
    strcat(jsonBuf, "]}");
    return jsonBuf;
}
