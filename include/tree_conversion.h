#ifndef TREE_CONVERSION_H
#define TREE_CONVERSION_H

#include "tree.h"

typedef struct LCRSBinaryNode {
    int id;
    char name[100];
    char type[32];
    double price;
    struct LCRSBinaryNode *leftChild;
    struct LCRSBinaryNode *rightSibling;
} LCRSBinaryNode;

LCRSBinaryNode* convertGeneralToBinary(TreeNode *genRoot);
void freeLCRSTree(LCRSBinaryNode *root);
char* serializeLCRSTreeJSON(LCRSBinaryNode *root);

#endif // TREE_CONVERSION_H
