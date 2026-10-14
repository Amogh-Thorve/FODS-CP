#ifndef BST_H
#define BST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct BSTNode {
    int productId;
    char name[100];
    double price;
    struct BSTNode *left;
    struct BSTNode *right;
} BSTNode;

BSTNode* createBSTNode(int productId, const char *name, double price);
BSTNode* insertBST(BSTNode *root, int productId, const char *name, double price);
BSTNode* searchBST(BSTNode *root, int productId);
BSTNode* findMinBST(BSTNode *root);
BSTNode* findMaxBST(BSTNode *root);
BSTNode* deleteBST(BSTNode *root, int productId);
int getBSTHeight(BSTNode *root);
int countBSTNodes(BSTNode *root);
void freeBST(BSTNode *root);
char* serializeBSTJSON(BSTNode *root);

#endif // BST_H
