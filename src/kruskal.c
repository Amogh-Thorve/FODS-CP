#include "kruskal.h"
#include "union_find.h"

typedef struct {
    int u, v;
    double weight;
} Edge;

static int compareEdges(const void *a, const void *b) {
    Edge *e1 = (Edge*)a;
    Edge *e2 = (Edge*)b;
    if (e1->weight < e2->weight) return -1;
    if (e1->weight > e2->weight) return 1;
    return 0;
}

KruskalResult runKruskal(Graph *g) {
    KruskalResult res;
    memset(&res, 0, sizeof(res));
    if (!g || g->numVertices == 0) return res;

    Edge edges[100];
    int numEdges = 0;

    for (int u = 0; u < g->numVertices; u++) {
        EdgeNode *curr = g->adjList[u];
        while (curr) {
            if (u < curr->dest) {
                edges[numEdges].u = u;
                edges[numEdges].v = curr->dest;
                edges[numEdges].weight = curr->weight;
                numEdges++;
            }
            curr = curr->next;
        }
    }

    qsort(edges, numEdges, sizeof(Edge), compareEdges);

    DisjointSet ds;
    makeSet(&ds, g->numVertices);

    for (int i = 0; i < numEdges; i++) {
        int u = edges[i].u;
        int v = edges[i].v;
        int rootU = findSet(&ds, u);
        int rootV = findSet(&ds, v);

        KruskalEdgeStep *step = &res.edgeSteps[res.totalSteps++];
        step->src = g->vertices[u].id;
        step->dest = g->vertices[v].id;
        step->weight = edges[i].weight;

        if (rootU != rootV) {
            unionSet(&ds, u, v);
            step->selected = true;

            res.mst.src[res.mst.edgeCount] = g->vertices[u].id;
            res.mst.dest[res.mst.edgeCount] = g->vertices[v].id;
            res.mst.weight[res.mst.edgeCount] = edges[i].weight;
            res.mst.totalWeight += edges[i].weight;
            res.mst.edgeCount++;
        } else {
            step->selected = false;
        }
    }

    return res;
}

char* serializeKruskalJSON(KruskalResult res, Graph *g) {
    static char jsonBuf[8192];
    jsonBuf[0] = '\0';
    snprintf(jsonBuf, sizeof(jsonBuf),
             "{\"totalWeight\":%.2f,\"totalSteps\":%d,\"steps\":[",
             res.mst.totalWeight, res.totalSteps);

    for (int i = 0; i < res.totalSteps; i++) {
        char sBuf[256];
        snprintf(sBuf, sizeof(sBuf),
                 "{\"src\":%d,\"dest\":%d,\"weight\":%.2f,\"selected\":%s}",
                 res.edgeSteps[i].src, res.edgeSteps[i].dest, res.edgeSteps[i].weight,
                 res.edgeSteps[i].selected ? "true" : "false");
        strcat(jsonBuf, sBuf);
        if (i < res.totalSteps - 1) strcat(jsonBuf, ",");
    }
    strcat(jsonBuf, "]}");
    return jsonBuf;
}
