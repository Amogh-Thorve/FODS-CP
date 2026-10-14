#include "ordered_list.h"

void initOrderedList(OrderedList *list) {
    if (!list) return;
    list->head = NULL;
    list->count = 0;
}

void insertSorted(OrderedList *list, int id, const char *name, double key) {
    if (!list) return;
    OrderedNode *node = (OrderedNode*)malloc(sizeof(OrderedNode));
    if (!node) return;
    node->id = id;
    strncpy(node->name, name ? name : "", sizeof(node->name) - 1);
    node->name[sizeof(node->name) - 1] = '\0';
    node->key = key;
    node->next = NULL;

    if (!list->head || key < list->head->key) {
        node->next = list->head;
        list->head = node;
    } else {
        OrderedNode *curr = list->head;
        while (curr->next && curr->next->key <= key) {
            curr = curr->next;
        }
        node->next = curr->next;
        curr->next = node;
    }
    list->count++;
}

OrderedNode* searchOrderedList(OrderedList *list, int id) {
    if (!list) return NULL;
    OrderedNode *curr = list->head;
    while (curr) {
        if (curr->id == id) return curr;
        curr = curr->next;
    }
    return NULL;
}

int deleteOrderedList(OrderedList *list, int id) {
    if (!list || !list->head) return 0;
    if (list->head->id == id) {
        OrderedNode *temp = list->head;
        list->head = list->head->next;
        free(temp);
        list->count--;
        return 1;
    }
    OrderedNode *curr = list->head;
    while (curr->next && curr->next->id != id) {
        curr = curr->next;
    }
    if (curr->next) {
        OrderedNode *temp = curr->next;
        curr->next = temp->next;
        free(temp);
        list->count--;
        return 1;
    }
    return 0;
}

void freeOrderedList(OrderedList *list) {
    if (!list) return;
    OrderedNode *curr = list->head;
    while (curr) {
        OrderedNode *next = curr->next;
        free(curr);
        curr = next;
    }
    list->head = NULL;
    list->count = 0;
}

char* serializeOrderedListJSON(OrderedList *list) {
    static char jsonBuf[4096];
    jsonBuf[0] = '\0';
    strcat(jsonBuf, "[");
    if (list) {
        OrderedNode *curr = list->head;
        while (curr) {
            char itemBuf[256];
            snprintf(itemBuf, sizeof(itemBuf),
                     "{\"id\":%d,\"name\":\"%s\",\"key\":%.2f}",
                     curr->id, curr->name, curr->key);
            strcat(jsonBuf, itemBuf);
            if (curr->next) strcat(jsonBuf, ",");
            curr = curr->next;
        }
    }
    strcat(jsonBuf, "]");
    return jsonBuf;
}
