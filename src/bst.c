#include "bst.h"

BSTNode* createBSTNode(int productId, const char *name, double price) {
    BSTNode *node = (BSTNode*)malloc(sizeof(BSTNode));
    if (!node) return NULL;
    node->productId = productId;
    strncpy(node->name, name ? name : "", sizeof(node->name) - 1);
    node->name[sizeof(node->name) - 1] = '\0';
    node->price = price;
    node->left = NULL;
    node->right = NULL;
    return node;
}

BSTNode* insertBST(BSTNode *root, int productId, const char *name, double price) {
    if (!root) return createBSTNode(productId, name, price);
    if (productId < root->productId) {
        root->left = insertBST(root->left, productId, name, price);
    } else if (productId > root->productId) {
        root->right = insertBST(root->right, productId, name, price);
    }
    return root;
}

BSTNode* searchBST(BSTNode *root, int productId) {
    if (!root || root->productId == productId) return root;
    if (productId < root->productId) return searchBST(root->left, productId);
    return searchBST(root->right, productId);
}

BSTNode* findMinBST(BSTNode *root) {
    if (!root) return NULL;
    while (root->left) root = root->left;
    return root;
}

BSTNode* findMaxBST(BSTNode *root) {
    if (!root) return NULL;
    while (root->right) root = root->right;
    return root;
}

BSTNode* deleteBST(BSTNode *root, int productId) {
    if (!root) return NULL;
    if (productId < root->productId) {
        root->left = deleteBST(root->left, productId);
    } else if (productId > root->productId) {
        root->right = deleteBST(root->right, productId);
    } else {
        if (!root->left) {
            BSTNode *temp = root->right;
            free(root);
            return temp;
        } else if (!root->right) {
            BSTNode *temp = root->left;
            free(root);
            return temp;
        }
        BSTNode *temp = findMinBST(root->right);
        root->productId = temp->productId;
        strcpy(root->name, temp->name);
        root->price = temp->price;
        root->right = deleteBST(root->right, temp->productId);
    }
    return root;
}

int getBSTHeight(BSTNode *root) {
    if (!root) return 0;
    int hLeft = getBSTHeight(root->left);
    int hRight = getBSTHeight(root->right);
    return 1 + (hLeft > hRight ? hLeft : hRight);
}

int countBSTNodes(BSTNode *root) {
    if (!root) return 0;
    return 1 + countBSTNodes(root->left) + countBSTNodes(root->right);
}

void freeBST(BSTNode *root) {
    if (!root) return;
    freeBST(root->left);
    freeBST(root->right);
    free(root);
}

static void serializeBSTHelper(BSTNode *node, char *buf, size_t bufSize) {
    if (!node) {
        strcat(buf, "null");
        return;
    }
    char nodeInfo[256];
    snprintf(nodeInfo, sizeof(nodeInfo),
             "{\"productId\":%d,\"name\":\"%s\",\"price\":%.2f,\"left\":",
             node->productId, node->name, node->price);
    strcat(buf, nodeInfo);
    serializeBSTHelper(node->left, buf, bufSize);
    strcat(buf, ",\"right\":");
    serializeBSTHelper(node->right, buf, bufSize);
    strcat(buf, "}");
}

char* serializeBSTJSON(BSTNode *root) {
    static char jsonBuf[8192];
    jsonBuf[0] = '\0';
    BSTNode *minNode = findMinBST(root);
    BSTNode *maxNode = findMaxBST(root);
    int h = getBSTHeight(root);
    int count = countBSTNodes(root);

    strcat(jsonBuf, "{\"height\":");
    char numStr[16];
    snprintf(numStr, sizeof(numStr), "%d,", h);
    strcat(jsonBuf, numStr);
    strcat(jsonBuf, "\"nodeCount\":");
    snprintf(numStr, sizeof(numStr), "%d,", count);
    strcat(jsonBuf, numStr);
    strcat(jsonBuf, "\"minId\":");
    snprintf(numStr, sizeof(numStr), "%d,", minNode ? minNode->productId : 0);
    strcat(jsonBuf, numStr);
    strcat(jsonBuf, "\"maxId\":");
    snprintf(numStr, sizeof(numStr), "%d,", maxNode ? maxNode->productId : 0);
    strcat(jsonBuf, numStr);
    strcat(jsonBuf, "\"tree\":");
    serializeBSTHelper(root, jsonBuf, sizeof(jsonBuf));
    strcat(jsonBuf, "}");

    return jsonBuf;
}
