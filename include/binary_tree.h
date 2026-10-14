#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct BinaryNode {
    int id;
    char name[100];
    double price;
    struct BinaryNode *left;
    struct BinaryNode *right;
} BinaryNode;

typedef struct {
    int ids[100];
    char names[100][100];
    int count;
} TraversalResult;

BinaryNode* createBinaryNode(int id, const char *name, double price);
void freeBinaryTree(BinaryNode *root);

// Traversals
TraversalResult preorderRecursive(BinaryNode *root);
TraversalResult inorderRecursive(BinaryNode *root);
TraversalResult postorderRecursive(BinaryNode *root);

TraversalResult preorderNonRecursive(BinaryNode *root);
TraversalResult inorderNonRecursive(BinaryNode *root);
TraversalResult postorderNonRecursive(BinaryNode *root);
TraversalResult levelOrderTraversal(BinaryNode *root);

char* serializeTraversalJSON(TraversalResult res, const char *traversalType);

#endif // BINARY_TREE_H
