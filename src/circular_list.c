#include "circular_list.h"

void initCircularList(CircularList *list) {
    if (!list) return;
    list->head = NULL;
    list->tail = NULL;
    list->currentWorker = NULL;
    list->count = 0;
}

void insertWorker(CircularList *list, int id, const char *name) {
    if (!list) return;
    CircularNode *node = (CircularNode*)malloc(sizeof(CircularNode));
    if (!node) return;
    node->workerId = id;
    strncpy(node->workerName, name ? name : "", sizeof(node->workerName) - 1);
    node->workerName[sizeof(node->workerName) - 1] = '\0';
    snprintf(node->currentTask, sizeof(node->currentTask), "Idle Worker %d", id);

    if (!list->head) {
        list->head = node;
        list->tail = node;
        node->next = list->head;
        list->currentWorker = list->head;
    } else {
        node->next = list->head;
        list->tail->next = node;
        list->tail = node;
    }
    list->count++;
}

CircularNode* getNextWorker(CircularList *list) {
    if (!list || !list->currentWorker) return NULL;
    CircularNode *assigned = list->currentWorker;
    list->currentWorker = list->currentWorker->next;
    return assigned;
}

void displayCircularList(CircularList *list) {
    if (!list || !list->head) return;
    CircularNode *curr = list->head;
    printf("Circular Workers: ");
    for (int i = 0; i < list->count; i++) {
        printf("[%d: %s] -> ", curr->workerId, curr->workerName);
        curr = curr->next;
    }
    printf("(head)\n");
}

void clearCircularList(CircularList *list) {
    if (!list || !list->head) return;
    CircularNode *curr = list->head;
    for (int i = 0; i < list->count; i++) {
        CircularNode *next = curr->next;
        free(curr);
        curr = next;
    }
    list->head = NULL;
    list->tail = NULL;
    list->currentWorker = NULL;
    list->count = 0;
}

char* serializeCircularListJSON(CircularList *list) {
    static char jsonBuf[4096];
    jsonBuf[0] = '\0';
    strcat(jsonBuf, "[");
    if (list && list->head) {
        CircularNode *curr = list->head;
        for (int i = 0; i < list->count; i++) {
            char itemBuf[256];
            int isCurrent = (curr == list->currentWorker) ? 1 : 0;
            snprintf(itemBuf, sizeof(itemBuf),
                     "{\"workerId\":%d,\"workerName\":\"%s\",\"currentTask\":\"%s\",\"isCurrent\":%d}",
                     curr->workerId, curr->workerName, curr->currentTask, isCurrent);
            strcat(jsonBuf, itemBuf);
            if (i < list->count - 1) strcat(jsonBuf, ",");
            curr = curr->next;
        }
    }
    strcat(jsonBuf, "]");
    return jsonBuf;
}
