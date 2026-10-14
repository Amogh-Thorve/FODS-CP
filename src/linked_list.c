#include "linked_list.h"

void initLinkedList(ConfigLinkedList *list) {
    if (!list) return;
    list->head = NULL;
    list->count = 0;
}

ConfigNode* createNode(int itemId, const char *name, const char *category, double price, int isOption) {
    ConfigNode *node = (ConfigNode*)malloc(sizeof(ConfigNode));
    if (!node) return NULL;
    node->itemId = itemId;
    strncpy(node->name, name ? name : "", sizeof(node->name) - 1);
    node->name[sizeof(node->name) - 1] = '\0';
    strncpy(node->category, category ? category : "", sizeof(node->category) - 1);
    node->category[sizeof(node->category) - 1] = '\0';
    node->price = price;
    node->isOption = isOption;
    node->next = NULL;
    return node;
}

void insertFront(ConfigLinkedList *list, int itemId, const char *name, const char *category, double price, int isOption) {
    if (!list) return;
    ConfigNode *node = createNode(itemId, name, category, price, isOption);
    if (!node) return;
    node->next = list->head;
    list->head = node;
    list->count++;
}

void insertEnd(ConfigLinkedList *list, int itemId, const char *name, const char *category, double price, int isOption) {
    if (!list) return;
    ConfigNode *node = createNode(itemId, name, category, price, isOption);
    if (!node) return;
    if (!list->head) {
        list->head = node;
    } else {
        ConfigNode *curr = list->head;
        while (curr->next) {
            curr = curr->next;
        }
        curr->next = node;
    }
    list->count++;
}

void insertAtPosition(ConfigLinkedList *list, int position, int itemId, const char *name, const char *category, double price, int isOption) {
    if (!list || position < 0 || position > list->count) return;
    if (position == 0) {
        insertFront(list, itemId, name, category, price, isOption);
        return;
    }
    ConfigNode *node = createNode(itemId, name, category, price, isOption);
    if (!node) return;
    ConfigNode *curr = list->head;
    for (int i = 0; i < position - 1 && curr; i++) {
        curr = curr->next;
    }
    if (curr) {
        node->next = curr->next;
        curr->next = node;
        list->count++;
    } else {
        free(node);
    }
}

int deleteFront(ConfigLinkedList *list) {
    if (!list || !list->head) return 0;
    ConfigNode *temp = list->head;
    list->head = list->head->next;
    free(temp);
    list->count--;
    return 1;
}

int deleteEnd(ConfigLinkedList *list) {
    if (!list || !list->head) return 0;
    if (!list->head->next) {
        free(list->head);
        list->head = NULL;
        list->count--;
        return 1;
    }
    ConfigNode *curr = list->head;
    while (curr->next && curr->next->next) {
        curr = curr->next;
    }
    free(curr->next);
    curr->next = NULL;
    list->count--;
    return 1;
}

int deleteByValue(ConfigLinkedList *list, int itemId) {
    if (!list || !list->head) return 0;
    if (list->head->itemId == itemId) {
        return deleteFront(list);
    }
    ConfigNode *curr = list->head;
    while (curr->next && curr->next->itemId != itemId) {
        curr = curr->next;
    }
    if (curr->next) {
        ConfigNode *temp = curr->next;
        curr->next = temp->next;
        free(temp);
        list->count--;
        return 1;
    }
    return 0;
}

ConfigNode* searchNode(ConfigLinkedList *list, int itemId) {
    if (!list) return NULL;
    ConfigNode *curr = list->head;
    while (curr) {
        if (curr->itemId == itemId) return curr;
        curr = curr->next;
    }
    return NULL;
}

int updateNode(ConfigLinkedList *list, int itemId, double newPrice) {
    ConfigNode *node = searchNode(list, itemId);
    if (node) {
        node->price = newPrice;
        return 1;
    }
    return 0;
}

void displayList(ConfigLinkedList *list) {
    if (!list) return;
    ConfigNode *curr = list->head;
    printf("ConfigLinkedList (Count %d): ", list->count);
    while (curr) {
        printf("[%d: %s (%.2f)] -> ", curr->itemId, curr->name, curr->price);
        curr = curr->next;
    }
    printf("NULL\n");
}

int countNodes(ConfigLinkedList *list) {
    return list ? list->count : 0;
}

void clearList(ConfigLinkedList *list) {
    if (!list) return;
    ConfigNode *curr = list->head;
    while (curr) {
        ConfigNode *next = curr->next;
        free(curr);
        curr = next;
    }
    list->head = NULL;
    list->count = 0;
}

char* serializeLinkedListJSON(ConfigLinkedList *list) {
    static char jsonBuf[4096];
    jsonBuf[0] = '\0';
    strcat(jsonBuf, "[");
    if (list) {
        ConfigNode *curr = list->head;
        while (curr) {
            char itemBuf[512];
            snprintf(itemBuf, sizeof(itemBuf),
                     "{\"itemId\":%d,\"name\":\"%s\",\"category\":\"%s\",\"price\":%.2f,\"isOption\":%d}",
                     curr->itemId, curr->name, curr->category, curr->price, curr->isOption);
            strcat(jsonBuf, itemBuf);
            if (curr->next) strcat(jsonBuf, ",");
            curr = curr->next;
        }
    }
    strcat(jsonBuf, "]");
    return jsonBuf;
}
