#include "graph.h"

void initGraph(Graph *g) {
    if (!g) return;
    g->numVertices = 0;
    for (int i = 0; i < MAX_VERTICES; i++) {
        g->adjList[i] = NULL;
        for (int j = 0; j < MAX_VERTICES; j++) {
            g->adjMatrix[i][j] = (i == j) ? 0 : INF;
            g->relMatrix[i][j] = REL_OPTIONAL;
        }
    }
}

int addVertex(Graph *g, int id, const char *name, const char *category) {
    if (!g || g->numVertices >= MAX_VERTICES) return -1;
    int idx = findVertexIndex(g, id);
    if (idx != -1) return idx;

    idx = g->numVertices++;
    g->vertices[idx].id = id;
    strncpy(g->vertices[idx].name, name ? name : "", sizeof(g->vertices[idx].name) - 1);
    g->vertices[idx].name[sizeof(g->vertices[idx].name) - 1] = '\0';
    strncpy(g->vertices[idx].category, category ? category : "", sizeof(g->vertices[idx].category) - 1);
    g->vertices[idx].category[sizeof(g->vertices[idx].category) - 1] = '\0';
    return idx;
}

int findVertexIndex(Graph *g, int id) {
    if (!g) return -1;
    for (int i = 0; i < g->numVertices; i++) {
        if (g->vertices[i].id == id) return i;
    }
    return -1;
}

void addEdge(Graph *g, int srcId, int destId, RelType rel, double weight) {
    if (!g) return;
    int u = findVertexIndex(g, srcId);
    int v = findVertexIndex(g, destId);
    if (u == -1 || v == -1) return;

    // Update Adjacency Matrix
    g->adjMatrix[u][v] = weight;
    g->relMatrix[u][v] = rel;

    // Update Adjacency List
    EdgeNode *node = (EdgeNode*)malloc(sizeof(EdgeNode));
    if (node) {
        node->dest = v;
        node->rel = rel;
        node->weight = weight;
        node->next = g->adjList[u];
        g->adjList[u] = node;
    }
}

void removeEdge(Graph *g, int srcId, int destId) {
    if (!g) return;
    int u = findVertexIndex(g, srcId);
    int v = findVertexIndex(g, destId);
    if (u == -1 || v == -1) return;

    g->adjMatrix[u][v] = INF;
    g->relMatrix[u][v] = REL_OPTIONAL;

    EdgeNode *curr = g->adjList[u];
    EdgeNode *prev = NULL;
    while (curr) {
        if (curr->dest == v) {
            if (prev) prev->next = curr->next;
            else g->adjList[u] = curr->next;
            free(curr);
            break;
        }
        prev = curr;
        curr = curr->next;
    }
}

void displayGraph(Graph *g) {
    if (!g) return;
    printf("Graph with %d vertices:\n", g->numVertices);
    for (int i = 0; i < g->numVertices; i++) {
        printf("Vertex %d (%s): ", g->vertices[i].id, g->vertices[i].name);
        EdgeNode *curr = g->adjList[i];
        while (curr) {
            printf("-> [%d, w:%.1f] ", g->vertices[curr->dest].id, curr->weight);
            curr = curr->next;
        }
        printf("\n");
    }
}

void freeGraph(Graph *g) {
    if (!g) return;
    for (int i = 0; i < g->numVertices; i++) {
        EdgeNode *curr = g->adjList[i];
        while (curr) {
            EdgeNode *temp = curr;
            curr = curr->next;
            free(temp);
        }
        g->adjList[i] = NULL;
    }
}

static const char* relTypeToString(RelType rel) {
    switch (rel) {
        case REL_COMPATIBLE: return "COMPATIBLE";
        case REL_INCOMPATIBLE: return "INCOMPATIBLE";
        case REL_REQUIRES: return "REQUIRES";
        case REL_OPTIONAL: return "OPTIONAL";
        case REL_CONFLICTS: return "CONFLICTS";
        default: return "UNKNOWN";
    }
}

char* serializeGraphJSON(Graph *g) {
    static char jsonBuf[8192];
    jsonBuf[0] = '\0';
    if (!g) return "{}";

    strcat(jsonBuf, "{\"numVertices\":");
    char numStr[16];
    snprintf(numStr, sizeof(numStr), "%d,", g->numVertices);
    strcat(jsonBuf, numStr);

    // Nodes
    strcat(jsonBuf, "\"vertices\":[");
    for (int i = 0; i < g->numVertices; i++) {
        char vBuf[256];
        snprintf(vBuf, sizeof(vBuf),
                 "{\"index\":%d,\"id\":%d,\"name\":\"%s\",\"category\":\"%s\"}",
                 i, g->vertices[i].id, g->vertices[i].name, g->vertices[i].category);
        strcat(jsonBuf, vBuf);
        if (i < g->numVertices - 1) strcat(jsonBuf, ",");
    }
    strcat(jsonBuf, "],\"edges\":[");

    // Edges
    int firstEdge = 1;
    for (int i = 0; i < g->numVertices; i++) {
        EdgeNode *curr = g->adjList[i];
        while (curr) {
            if (!firstEdge) strcat(jsonBuf, ",");
            firstEdge = 0;
            char eBuf[256];
            snprintf(eBuf, sizeof(eBuf),
                     "{\"src\":%d,\"srcId\":%d,\"dest\":%d,\"destId\":%d,\"rel\":\"%s\",\"weight\":%.2f}",
                     i, g->vertices[i].id, curr->dest, g->vertices[curr->dest].id, relTypeToString(curr->rel), curr->weight);
            strcat(jsonBuf, eBuf);
            curr = curr->next;
        }
    }
    strcat(jsonBuf, "]}");
    return jsonBuf;
}
