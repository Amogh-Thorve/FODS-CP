#include "binary_tree.h"

BinaryNode* createBinaryNode(int id, const char *name, double price) {
    BinaryNode *node = (BinaryNode*)malloc(sizeof(BinaryNode));
    if (!node) return NULL;
    node->id = id;
    strncpy(node->name, name ? name : "", sizeof(node->name) - 1);
    node->name[sizeof(node->name) - 1] = '\0';
    node->price = price;
    node->left = NULL;
    node->right = NULL;
    return node;
}

void freeBinaryTree(BinaryNode *root) {
    if (!root) return;
    freeBinaryTree(root->left);
    freeBinaryTree(root->right);
    free(root);
}

// Helpers for traversals
static void preorderRecHelper(BinaryNode *node, TraversalResult *res) {
    if (!node) return;
    res->ids[res->count] = node->id;
    strcpy(res->names[res->count], node->name);
    res->count++;
    preorderRecHelper(node->left, res);
    preorderRecHelper(node->right, res);
}

TraversalResult preorderRecursive(BinaryNode *root) {
    TraversalResult res;
    res.count = 0;
    preorderRecHelper(root, &res);
    return res;
}

static void inorderRecHelper(BinaryNode *node, TraversalResult *res) {
    if (!node) return;
    inorderRecHelper(node->left, res);
    res->ids[res->count] = node->id;
    strcpy(res->names[res->count], node->name);
    res->count++;
    inorderRecHelper(node->right, res);
}

TraversalResult inorderRecursive(BinaryNode *root) {
    TraversalResult res;
    res.count = 0;
    inorderRecHelper(root, &res);
    return res;
}

static void postorderRecHelper(BinaryNode *node, TraversalResult *res) {
    if (!node) return;
    postorderRecHelper(node->left, res);
    postorderRecHelper(node->right, res);
    res->ids[res->count] = node->id;
    strcpy(res->names[res->count], node->name);
    res->count++;
}

TraversalResult postorderRecursive(BinaryNode *root) {
    TraversalResult res;
    res.count = 0;
    postorderRecHelper(root, &res);
    return res;
}

// Non-recursive Preorder using Stack
TraversalResult preorderNonRecursive(BinaryNode *root) {
    TraversalResult res;
    res.count = 0;
    if (!root) return res;

    BinaryNode* stack[100];
    int top = -1;
    stack[++top] = root;

    while (top >= 0) {
        BinaryNode *curr = stack[top--];
        res.ids[res.count] = curr->id;
        strcpy(res.names[res.count], curr->name);
        res.count++;

        if (curr->right) stack[++top] = curr->right;
        if (curr->left) stack[++top] = curr->left;
    }
    return res;
}

// Non-recursive Inorder using Stack
TraversalResult inorderNonRecursive(BinaryNode *root) {
    TraversalResult res;
    res.count = 0;
    if (!root) return res;

    BinaryNode* stack[100];
    int top = -1;
    BinaryNode *curr = root;

    while (curr != NULL || top >= 0) {
        while (curr != NULL) {
            stack[++top] = curr;
            curr = curr->left;
        }
        curr = stack[top--];
        res.ids[res.count] = curr->id;
        strcpy(res.names[res.count], curr->name);
        res.count++;
        curr = curr->right;
    }
    return res;
}

// Non-recursive Postorder using Two Stacks
TraversalResult postorderNonRecursive(BinaryNode *root) {
    TraversalResult res;
    res.count = 0;
    if (!root) return res;

    BinaryNode* s1[100];
    BinaryNode* s2[100];
    int top1 = -1, top2 = -1;

    s1[++top1] = root;
    while (top1 >= 0) {
        BinaryNode *curr = s1[top1--];
        s2[++top2] = curr;
        if (curr->left) s1[++top1] = curr->left;
        if (curr->right) s1[++top1] = curr->right;
    }

    while (top2 >= 0) {
        BinaryNode *curr = s2[top2--];
        res.ids[res.count] = curr->id;
        strcpy(res.names[res.count], curr->name);
        res.count++;
    }
    return res;
}

// Level Order Traversal using Queue
TraversalResult levelOrderTraversal(BinaryNode *root) {
    TraversalResult res;
    res.count = 0;
    if (!root) return res;

    BinaryNode* queue[100];
    int front = 0, rear = 0;
    queue[rear++] = root;

    while (front < rear) {
        BinaryNode *curr = queue[front++];
        res.ids[res.count] = curr->id;
        strcpy(res.names[res.count], curr->name);
        res.count++;

        if (curr->left) queue[rear++] = curr->left;
        if (curr->right) queue[rear++] = curr->right;
    }
    return res;
}

char* serializeTraversalJSON(TraversalResult res, const char *traversalType) {
    static char jsonBuf[4096];
    jsonBuf[0] = '\0';
    strcat(jsonBuf, "{\"traversalType\":\"");
    strcat(jsonBuf, traversalType ? traversalType : "PREORDER");
    strcat(jsonBuf, "\",\"count\":");
    char countStr[16];
    snprintf(countStr, sizeof(countStr), "%d,", res.count);
    strcat(jsonBuf, countStr);
    strcat(jsonBuf, "\"sequence\":[");

    for (int i = 0; i < res.count; i++) {
        char itemBuf[256];
        snprintf(itemBuf, sizeof(itemBuf),
                 "{\"id\":%d,\"name\":\"%s\"}", res.ids[i], res.names[i]);
        strcat(jsonBuf, itemBuf);
        if (i < res.count - 1) strcat(jsonBuf, ",");
    }
    strcat(jsonBuf, "]}");
    return jsonBuf;
}
