#include "tree_conversion.h"

LCRSBinaryNode* convertGeneralToBinary(TreeNode *genRoot) {
    if (!genRoot) return NULL;
    LCRSBinaryNode *binNode = (LCRSBinaryNode*)malloc(sizeof(LCRSBinaryNode));
    if (!binNode) return NULL;
    binNode->id = genRoot->id;
    strncpy(binNode->name, genRoot->name, sizeof(binNode->name) - 1);
    binNode->name[sizeof(binNode->name) - 1] = '\0';
    strncpy(binNode->type, genRoot->type, sizeof(binNode->type) - 1);
    binNode->type[sizeof(binNode->type) - 1] = '\0';
    binNode->price = genRoot->price;
    binNode->leftChild = NULL;
    binNode->rightSibling = NULL;

    if (genRoot->childCount > 0) {
        binNode->leftChild = convertGeneralToBinary(genRoot->children[0]);
        LCRSBinaryNode *currSibling = binNode->leftChild;
        for (int i = 1; i < genRoot->childCount; i++) {
            currSibling->rightSibling = convertGeneralToBinary(genRoot->children[i]);
            currSibling = currSibling->rightSibling;
        }
    }
    return binNode;
}

void freeLCRSTree(LCRSBinaryNode *root) {
    if (!root) return;
    freeLCRSTree(root->leftChild);
    freeLCRSTree(root->rightSibling);
    free(root);
}

static void serializeLCRSHelper(LCRSBinaryNode *node, char *buf, size_t bufSize) {
    if (!node) {
        strcat(buf, "null");
        return;
    }
    char nodeInfo[256];
    snprintf(nodeInfo, sizeof(nodeInfo),
             "{\"id\":%d,\"name\":\"%s\",\"type\":\"%s\",\"price\":%.2f,\"leftChild\":",
             node->id, node->name, node->type, node->price);
    strcat(buf, nodeInfo);
    serializeLCRSHelper(node->leftChild, buf, bufSize);
    strcat(buf, ",\"rightSibling\":");
    serializeLCRSHelper(node->rightSibling, buf, bufSize);
    strcat(buf, "}");
}

char* serializeLCRSTreeJSON(LCRSBinaryNode *root) {
    static char jsonBuf[8192];
    jsonBuf[0] = '\0';
    serializeLCRSHelper(root, jsonBuf, sizeof(jsonBuf));
    return jsonBuf;
}
