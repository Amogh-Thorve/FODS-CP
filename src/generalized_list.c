#include "generalized_list.h"

GListNode* createAtomNode(int id, const char *name, double price) {
    GListNode *node = (GListNode*)malloc(sizeof(GListNode));
    if (!node) return NULL;
    node->type = ATOM;
    node->data.atom.id = id;
    strncpy(node->data.atom.name, name ? name : "", sizeof(node->data.atom.name) - 1);
    node->data.atom.name[sizeof(node->data.atom.name) - 1] = '\0';
    node->data.atom.price = price;
    node->next = NULL;
    return node;
}

GListNode* createListNode(const char *categoryName, GListNode *subList) {
    GListNode *node = (GListNode*)malloc(sizeof(GListNode));
    if (!node) return NULL;
    node->type = LIST;
    strncpy(node->data.list.categoryName, categoryName ? categoryName : "", sizeof(node->data.list.categoryName) - 1);
    node->data.list.categoryName[sizeof(node->data.list.categoryName) - 1] = '\0';
    node->data.list.subList = subList;
    node->next = NULL;
    return node;
}

void appendGNode(GListNode **head, GListNode *newNode) {
    if (!head || !newNode) return;
    if (!*head) {
        *head = newNode;
    } else {
        GListNode *curr = *head;
        while (curr->next) curr = curr->next;
        curr->next = newNode;
    }
}

void freeGList(GListNode *head) {
    GListNode *curr = head;
    while (curr) {
        GListNode *next = curr->next;
        if (curr->type == LIST) {
            freeGList(curr->data.list.subList);
        }
        free(curr);
        curr = next;
    }
}

static void serializeGListHelper(GListNode *head, char *buf, size_t bufSize) {
    strcat(buf, "[");
    GListNode *curr = head;
    while (curr) {
        if (curr->type == ATOM) {
            char itemBuf[256];
            snprintf(itemBuf, sizeof(itemBuf),
                     "{\"type\":\"ATOM\",\"id\":%d,\"name\":\"%s\",\"price\":%.2f}",
                     curr->data.atom.id, curr->data.atom.name, curr->data.atom.price);
            strcat(buf, itemBuf);
        } else {
            char listBuf[256];
            snprintf(listBuf, sizeof(listBuf),
                     "{\"type\":\"LIST\",\"categoryName\":\"%s\",\"children\":",
                     curr->data.list.categoryName);
            strcat(buf, listBuf);
            serializeGListHelper(curr->data.list.subList, buf, bufSize);
            strcat(buf, "}");
        }
        if (curr->next) strcat(buf, ",");
        curr = curr->next;
    }
    strcat(buf, "]");
}

char* serializeGListJSON(GListNode *head) {
    static char jsonBuf[8192];
    jsonBuf[0] = '\0';
    serializeGListHelper(head, jsonBuf, sizeof(jsonBuf));
    return jsonBuf;
}
