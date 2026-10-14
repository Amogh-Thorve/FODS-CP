#include "tree.h"

TreeNode* createTreeNode(int id, const char *name, const char *type, double price) {
    TreeNode *node = (TreeNode*)malloc(sizeof(TreeNode));
    if (!node) return NULL;
    node->id = id;
    strncpy(node->name, name ? name : "", sizeof(node->name) - 1);
    node->name[sizeof(node->name) - 1] = '\0';
    strncpy(node->type, type ? type : "", sizeof(node->type) - 1);
    node->type[sizeof(node->type) - 1] = '\0';
    node->price = price;
    node->childCount = 0;
    for (int i = 0; i < MAX_CHILDREN; i++) node->children[i] = NULL;
    return node;
}

void addChild(TreeNode *parent, TreeNode *child) {
    if (!parent || !child || parent->childCount >= MAX_CHILDREN) return;
    parent->children[parent->childCount++] = child;
}

TreeNode* searchTree(TreeNode *root, int id) {
    if (!root) return NULL;
    if (root->id == id) return root;
    for (int i = 0; i < root->childCount; i++) {
        TreeNode *found = searchTree(root->children[i], id);
        if (found) return found;
    }
    return NULL;
}

void freeTree(TreeNode *root) {
    if (!root) return;
    for (int i = 0; i < root->childCount; i++) {
        freeTree(root->children[i]);
    }
    free(root);
}

static void serializeGeneralTreeHelper(TreeNode *node, char *buf, size_t bufSize) {
    if (!node) {
        strcat(buf, "null");
        return;
    }
    char nodeInfo[256];
    snprintf(nodeInfo, sizeof(nodeInfo),
             "{\"id\":%d,\"name\":\"%s\",\"type\":\"%s\",\"price\":%.2f,\"children\":[",
             node->id, node->name, node->type, node->price);
    strcat(buf, nodeInfo);
    for (int i = 0; i < node->childCount; i++) {
        serializeGeneralTreeHelper(node->children[i], buf, bufSize);
        if (i < node->childCount - 1) strcat(buf, ",");
    }
    strcat(buf, "]}");
}

char* serializeGeneralTreeJSON(TreeNode *root) {
    static char jsonBuf[8192];
    jsonBuf[0] = '\0';
    serializeGeneralTreeHelper(root, jsonBuf, sizeof(jsonBuf));
    return jsonBuf;
}
