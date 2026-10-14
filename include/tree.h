#ifndef TREE_H
#define TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHILDREN 10

typedef struct TreeNode {
    int id;
    char name[100];
    char type[32]; // "PRODUCT", "CATEGORY", "OPTION"
    double price;
    struct TreeNode* children[MAX_CHILDREN];
    int childCount;
} TreeNode;

TreeNode* createTreeNode(int id, const char *name, const char *type, double price);
void addChild(TreeNode *parent, TreeNode *child);
TreeNode* searchTree(TreeNode *root, int id);
void freeTree(TreeNode *root);
char* serializeGeneralTreeJSON(TreeNode *root);

#endif // TREE_H
