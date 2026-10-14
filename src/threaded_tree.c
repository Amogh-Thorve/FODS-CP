#include "threaded_tree.h"

ThreadedNode* createThreadedNode(int data, const char *name) {
    ThreadedNode *node = (ThreadedNode*)malloc(sizeof(ThreadedNode));
    if (!node) return NULL;
    node->data = data;
    strncpy(node->name, name ? name : "", sizeof(node->name) - 1);
    node->name[sizeof(node->name) - 1] = '\0';
    node->left = NULL;
    node->right = NULL;
    node->lThread = true;
    node->rThread = true;
    return node;
}

ThreadedNode* insertThreaded(ThreadedNode *root, int data, const char *name) {
    ThreadedNode *ptr = root;
    ThreadedNode *parent = NULL;
    while (ptr != NULL) {
        if (data == ptr->data) return root; // Duplicate
        parent = ptr;
        if (data < ptr->data) {
            if (!ptr->lThread) ptr = ptr->left;
            else break;
        } else {
            if (!ptr->rThread) ptr = ptr->right;
            else break;
        }
    }

    ThreadedNode *newNode = createThreadedNode(data, name);
    if (parent == NULL) {
        root = newNode;
    } else if (data < parent->data) {
        newNode->left = parent->left;
        newNode->right = parent;
        parent->lThread = false;
        parent->left = newNode;
    } else {
        newNode->left = parent;
        newNode->right = parent->right;
        parent->rThread = false;
        parent->right = newNode;
    }
    return root;
}

void inorderThreadedTraversal(ThreadedNode *root, int *outArr, char outNames[][64], int *count) {
    *count = 0;
    if (!root) return;

    ThreadedNode *curr = root;
    while (!curr->lThread) {
        curr = curr->left;
    }

    while (curr != NULL) {
        outArr[*count] = curr->data;
        strcpy(outNames[*count], curr->name);
        (*count)++;

        if (curr->rThread) {
            curr = curr->right;
        } else {
            curr = curr->right;
            while (curr && !curr->lThread) {
                curr = curr->left;
            }
        }
    }
}

void freeThreadedTree(ThreadedNode *root) {
    if (!root) return;
    // Basic traversal free logic
}

char* serializeThreadedTreeJSON(ThreadedNode *root) {
    static char jsonBuf[4096];
    jsonBuf[0] = '\0';

    int dataArr[100];
    char names[100][64];
    int count = 0;
    inorderThreadedTraversal(root, dataArr, names, &count);

    strcat(jsonBuf, "{\"count\":");
    char cStr[16];
    snprintf(cStr, sizeof(cStr), "%d,", count);
    strcat(jsonBuf, cStr);
    strcat(jsonBuf, "\"inorderSequence\":[");
    for (int i = 0; i < count; i++) {
        char itemBuf[128];
        snprintf(itemBuf, sizeof(itemBuf), "{\"data\":%d,\"name\":\"%s\"}", dataArr[i], names[i]);
        strcat(jsonBuf, itemBuf);
        if (i < count - 1) strcat(jsonBuf, ",");
    }
    strcat(jsonBuf, "]}");
    return jsonBuf;
}
