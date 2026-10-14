#include "doubly_list.h"
#include <time.h>

static int g_historyIdCounter = 1;

void initHistoryList(DoublyHistoryList *list) {
    if (!list) return;
    list->head = NULL;
    list->tail = NULL;
    list->current = NULL;
    list->count = 0;
}

void insertHistory(DoublyHistoryList *list, const char *operation, int itemId, const char *itemName, double totalPrice) {
    if (!list) return;
    HistoryNode *node = (HistoryNode*)malloc(sizeof(HistoryNode));
    if (!node) return;
    node->historyId = g_historyIdCounter++;
    
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    if (timeinfo) {
        strftime(node->timestamp, sizeof(node->timestamp), "%H:%M:%S", timeinfo);
    } else {
        strcpy(node->timestamp, "00:00:00");
    }

    strncpy(node->operation, operation ? operation : "", sizeof(node->operation) - 1);
    node->operation[sizeof(node->operation) - 1] = '\0';
    node->itemId = itemId;
    strncpy(node->itemName, itemName ? itemName : "", sizeof(node->itemName) - 1);
    node->itemName[sizeof(node->itemName) - 1] = '\0';
    node->currentTotalPrice = totalPrice;

    node->prev = list->tail;
    node->next = NULL;

    if (list->tail) {
        list->tail->next = node;
    } else {
        list->head = node;
    }
    list->tail = node;
    list->current = node;
    list->count++;
}

int deleteHistory(DoublyHistoryList *list, int historyId) {
    if (!list || !list->head) return 0;
    HistoryNode *curr = list->head;
    while (curr && curr->historyId != historyId) {
        curr = curr->next;
    }
    if (!curr) return 0;
    if (curr->prev) curr->prev->next = curr->next;
    else list->head = curr->next;
    if (curr->next) curr->next->prev = curr->prev;
    else list->tail = curr->prev;
    if (list->current == curr) list->current = curr->prev ? curr->prev : curr->next;
    free(curr);
    list->count--;
    return 1;
}

HistoryNode* previousState(DoublyHistoryList *list) {
    if (!list || !list->current || !list->current->prev) return NULL;
    list->current = list->current->prev;
    return list->current;
}

HistoryNode* nextState(DoublyHistoryList *list) {
    if (!list || !list->current || !list->current->next) return NULL;
    list->current = list->current->next;
    return list->current;
}

void displayHistory(DoublyHistoryList *list) {
    if (!list) return;
    HistoryNode *curr = list->head;
    printf("DoublyHistoryList: ");
    while (curr) {
        printf("[%s: %s (%s)] <-> ", curr->timestamp, curr->operation, curr->itemName);
        curr = curr->next;
    }
    printf("NULL\n");
}

void clearHistory(DoublyHistoryList *list) {
    if (!list) return;
    HistoryNode *curr = list->head;
    while (curr) {
        HistoryNode *next = curr->next;
        free(curr);
        curr = next;
    }
    list->head = NULL;
    list->tail = NULL;
    list->current = NULL;
    list->count = 0;
}

char* serializeHistoryJSON(DoublyHistoryList *list) {
    static char jsonBuf[8192];
    jsonBuf[0] = '\0';
    strcat(jsonBuf, "[");
    if (list) {
        HistoryNode *curr = list->head;
        while (curr) {
            char itemBuf[512];
            int isCurrent = (curr == list->current) ? 1 : 0;
            snprintf(itemBuf, sizeof(itemBuf),
                     "{\"historyId\":%d,\"timestamp\":\"%s\",\"operation\":\"%s\",\"itemId\":%d,\"itemName\":\"%s\",\"totalPrice\":%.2f,\"isCurrent\":%d}",
                     curr->historyId, curr->timestamp, curr->operation, curr->itemId, curr->itemName, curr->currentTotalPrice, isCurrent);
            strcat(jsonBuf, itemBuf);
            if (curr->next) strcat(jsonBuf, ",");
            curr = curr->next;
        }
    }
    strcat(jsonBuf, "]");
    return jsonBuf;
}
