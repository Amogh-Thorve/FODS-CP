#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_VERTICES 50
#define INF 999999

typedef enum {
    REL_COMPATIBLE,
    REL_INCOMPATIBLE,
    REL_REQUIRES,
    REL_OPTIONAL,
    REL_CONFLICTS
} RelType;

typedef struct EdgeNode {
    int dest;
    RelType rel;
    double weight;
    struct EdgeNode *next;
} EdgeNode;

typedef struct {
    int id;
    char name[64];
    char category[32];
} Vertex;

typedef struct {
    Vertex vertices[MAX_VERTICES];
    int numVertices;
    
    // Adjacency Matrix
    double adjMatrix[MAX_VERTICES][MAX_VERTICES];
    RelType relMatrix[MAX_VERTICES][MAX_VERTICES];
    
    // Adjacency List
    EdgeNode* adjList[MAX_VERTICES];
} Graph;

void initGraph(Graph *g);
int addVertex(Graph *g, int id, const char *name, const char *category);
int findVertexIndex(Graph *g, int id);
void addEdge(Graph *g, int srcId, int destId, RelType rel, double weight);
void removeEdge(Graph *g, int srcId, int destId);
void displayGraph(Graph *g);
void freeGraph(Graph *g);
char* serializeGraphJSON(Graph *g);

#endif // GRAPH_H
