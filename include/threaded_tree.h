#ifndef THREADED_TREE_H
#define THREADED_TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct ThreadedNode {
    int data;
    char name[64];
    struct ThreadedNode *left;
    struct ThreadedNode *right;
    bool lThread; // true if left is a thread pointer to predecessor
    bool rThread; // true if right is a thread pointer to successor
} ThreadedNode;

ThreadedNode* createThreadedNode(int data, const char *name);
ThreadedNode* insertThreaded(ThreadedNode *root, int data, const char *name);
void inorderThreadedTraversal(ThreadedNode *root, int *outArr, char outNames[][64], int *count);
void freeThreadedTree(ThreadedNode *root);
char* serializeThreadedTreeJSON(ThreadedNode *root);

#endif // THREADED_TREE_H
